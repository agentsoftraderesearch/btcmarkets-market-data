#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <criterion/criterion.h>
#include <cjson/cJSON.h>

#include "../include/btcmarkets/market_data.h"

// Run these integration-tests to make sure you have API connectvity with basic response validation

void suite_setup(void) {
    initialise_api_context("https://api.btcmarkets.net/v3");
}

void suite_teardown(void) {
    cleanup_api_context();
}

TestSuite(market_data_tests, .init=suite_setup, .fini=suite_teardown);

#include <criterion/criterion.h>
#include <cjson/cJSON.h>
#include <stdlib.h>
#include <string.h>

#include "../include/btcmarkets/market_data.h"

Test(market_data_tests, list_active_markets_structure_test) {
    char *response = list_active_markets();
    cr_assert_not_null(response, "Expected non-null response from list_active_markets().");

    cJSON *markets_array = cJSON_Parse(response);
    cr_assert_not_null(markets_array, "Failed to parse JSON response: %s", response);
    cr_assert(cJSON_IsArray(markets_array), "Expected JSON array in response.");

    const char *required_keys[] = {
        "marketId", "baseAssetName", "quoteAssetName",
        "minOrderAmount", "maxOrderAmount",
        "amountDecimals", "priceDecimals", "status"
    };
    size_t num_keys = sizeof(required_keys) / sizeof(required_keys[0]);

    cJSON *market = NULL;
    cJSON_ArrayForEach(market, markets_array) {
        cr_assert(cJSON_IsObject(market), "Each market entry must be a JSON object.");

        for (size_t i = 0; i < num_keys; ++i) {
            cr_expect(cJSON_HasObjectItem(market, required_keys[i]),
                      "Missing key '%s' in market object.", required_keys[i]);
        }
    }

    cJSON_Delete(markets_array);
    free(response);
}

Test(market_data_tests, get_market_ticker_structure_test) {
    char *response = get_market_ticker("BTC-AUD");

    cr_assert_not_null(response, "Expected a non-null response from get_market_ticker.");

    cJSON *json = cJSON_Parse(response);
    cr_assert_not_null(json, "Failed to parse response JSON: %s", response);

    const char *expected_keys[] = {
        "marketId", "bestBid", "bestAsk", "lastPrice",
        "volume24h", "volumeQte24h", "price24h", "pricePct24h",
        "low24h", "high24h", "timestamp"
    };

    size_t key_count = sizeof(expected_keys) / sizeof(expected_keys[0]);

    for (size_t i = 0; i < key_count; ++i) {
        cr_expect(cJSON_HasObjectItem(json, expected_keys[i]),
                  "Expected key '%s' not found in JSON response.", expected_keys[i]);
    }

    cJSON_Delete(json);
    free(response);
}

Test(market_data_tests, get_market_trades_structure_test) {
    GetMarketTradesQueryParams query_params = { 
        .before = 78234976,
        .limit = 3
    };

    char *response = get_market_trades("BTC-AUD", &query_params);
    cr_assert_not_null(response, "Expected a non-null response from get_market_trades.");

    cJSON *trades_array = cJSON_Parse(response);
    cr_assert_not_null(trades_array, "Failed to parse response JSON: %s", response);
    cr_assert(cJSON_IsArray(trades_array), "Expected JSON array in response.");

    const char *required_keys[] = { "id", "price", "amount", "timestamp" };
    size_t key_count = sizeof(required_keys) / sizeof(required_keys[0]);

    cJSON *trade = NULL;
    cJSON_ArrayForEach(trade, trades_array) {
        cr_assert(cJSON_IsObject(trade), "Each trade must be a JSON object.");
        for (size_t i = 0; i < key_count; ++i) {
            cr_expect(cJSON_HasObjectItem(trade, required_keys[i]),
                      "Missing key '%s' in trade object.", required_keys[i]);
        }
    }

    cJSON_Delete(trades_array);
    free(response);
}

Test(market_data_tests, get_market_orderbook_structure_test) {
    GetMarketOrderbookQueryParams query_params = { .level = 1 };
    
    char *response = get_market_orderbook("BTC-AUD", &query_params);
    cr_assert_not_null(response, "Expected a non-null response from get_market_orderbook.");

    cJSON *json = cJSON_Parse(response);
    cr_assert_not_null(json, "Failed to parse response JSON: %s", response);

    // Validate that the top-level keys are present
    cr_expect(cJSON_HasObjectItem(json, "marketId"), "Missing key 'marketId'");
    cr_expect(cJSON_HasObjectItem(json, "snapshotId"), "Missing key 'snapshotId'");
    cr_expect(cJSON_HasObjectItem(json, "asks"), "Missing key 'asks'");
    cr_expect(cJSON_HasObjectItem(json, "bids"), "Missing key 'bids'");

    // Validate that 'asks' is an array
    cJSON *asks = cJSON_GetObjectItem(json, "asks");
    cr_assert(cJSON_IsArray(asks), "'asks' should be an array.");

    // Validate that 'bids' is an array
    cJSON *bids = cJSON_GetObjectItem(json, "bids");
    cr_assert(cJSON_IsArray(bids), "'bids' should be an array.");

    // Validate that each item in 'asks' is an array with exactly 2 items (price, amount)
    cJSON *ask_item = NULL;
    cJSON_ArrayForEach(ask_item, asks) {
        cr_assert(cJSON_IsArray(ask_item), "Each 'ask' entry should be an array.");
        cr_assert(cJSON_GetArraySize(ask_item) == 2, "Each 'ask' should have exactly two elements.");
        
        // Validate price and amount in asks
        cr_assert(cJSON_IsString(cJSON_GetArrayItem(ask_item, 0)), "Price in 'ask' should be a string.");
        cr_assert(cJSON_IsString(cJSON_GetArrayItem(ask_item, 1)), "Amount in 'ask' should be a string.");
    }

    // Validate that each item in 'bids' is an array with exactly 2 items (price, amount)
    cJSON *bid_item = NULL;
    cJSON_ArrayForEach(bid_item, bids) {
        cr_assert(cJSON_IsArray(bid_item), "Each 'bid' entry should be an array.");
        cr_assert(cJSON_GetArraySize(bid_item) == 2, "Each 'bid' should have exactly two elements.");
        
        // Validate price and amount in bids
        cr_assert(cJSON_IsString(cJSON_GetArrayItem(bid_item, 0)), "Price in 'bid' should be a string.");
        cr_assert(cJSON_IsString(cJSON_GetArrayItem(bid_item, 1)), "Amount in 'bid' should be a string.");
    }

    cJSON_Delete(json);
    free(response);
}

Test(market_data_tests, get_market_candles_structure_test) {
    GetMarketCandlesQueryParams params = {
        .time_window = "1d",
        .from = "2025-05-01T00:00:00.000000Z",
        .to = "2025-05-05T00:00:00.000000Z",
        .limit = 3
    };
    char *response = get_market_candles("BTC-AUD", &params);
    cr_assert_not_null(response, "Expected a non-null response from get_market_candles.");

    cJSON *json = cJSON_Parse(response);
    cr_assert_not_null(json, "Failed to parse response JSON: %s", response);

    cr_assert(cJSON_IsArray(json), "Expected JSON array in response.");

    cJSON *candle = NULL;
    cJSON_ArrayForEach(candle, json) {
        cr_assert(cJSON_IsArray(candle), "Each 'candle' entry should be an array.");
        cr_assert(cJSON_GetArraySize(candle) == 6, "Each 'candle' should have exactly six elements.");
        
        // Validate timestamp (string)
        cr_assert(cJSON_IsString(cJSON_GetArrayItem(candle, 0)), "Timestamp should be a string.");

        // Validate and convert open, high, low, close (numbers)
        cJSON *open_price = cJSON_GetArrayItem(candle, 1);
        cr_assert(cJSON_IsString(open_price), "Open price should be a string.");
        const char *open_price_str = open_price->valuestring;
        char *endptr;
        double open_price_num = strtod(open_price_str, &endptr);
        cr_assert(*endptr == '\0', "Open price string could not be fully converted to a number.");
        cr_assert(open_price_num > 0, "Open price should be greater than 0, but got: %f", open_price_num);

        cJSON *high_price = cJSON_GetArrayItem(candle, 2);
        cr_assert(cJSON_IsString(high_price), "High price should be a string.");
        const char *high_price_str = high_price->valuestring;
        double high_price_num = strtod(high_price_str, &endptr);
        cr_assert(*endptr == '\0', "High price string could not be fully converted to a number.");
        cr_assert(high_price_num > 0, "High price should be greater than 0, but got: %f", high_price_num);

        cJSON *low_price = cJSON_GetArrayItem(candle, 3);
        cr_assert(cJSON_IsString(low_price), "Low price should be a string.");
        const char *low_price_str = low_price->valuestring;
        double low_price_num = strtod(low_price_str, &endptr);
        cr_assert(*endptr == '\0', "Low price string could not be fully converted to a number.");
        cr_assert(low_price_num > 0, "Low price should be greater than 0, but got: %f", low_price_num);

        cJSON *close_price = cJSON_GetArrayItem(candle, 4);
        cr_assert(cJSON_IsString(close_price), "Close price should be a string.");
        const char *close_price_str = close_price->valuestring;
        double close_price_num = strtod(close_price_str, &endptr);
        cr_assert(*endptr == '\0', "Close price string could not be fully converted to a number.");
        cr_assert(close_price_num > 0, "Close price should be greater than 0, but got: %f", close_price_num);

        // Validate volume (number)
        cJSON *volume = cJSON_GetArrayItem(candle, 5);
        cr_assert(cJSON_IsString(volume), "Volume should be a string.");
        const char *volume_str = volume->valuestring;
        double volume_num = strtod(volume_str, &endptr);
        cr_assert(*endptr == '\0', "Volume string could not be fully converted to a number.");
        cr_assert(volume_num > 0, "Volume should be greater than 0, but got: %f", volume_num);
    }

    cJSON_Delete(json);
    free(response);
}

Test(market_data_tests, get_multiple_tickers_structure_test) {
    const char *market_ids[] = {"BTC-AUD", "XRP-AUD", "ETH-AUD"};
    GetMultipleTickersQueryParams params = {
        .market_ids = (char **)market_ids,
        .num_market_ids = sizeof(market_ids) / sizeof(market_ids[0])
    };

    char *response = get_multiple_tickers(&params);
    cr_assert_not_null(response, "Expected a non-null response from get_multiple_tickers.");

    cJSON *json = cJSON_Parse(response);
    cr_assert_not_null(json, "Failed to parse response JSON: %s", response);

    // Validate that the response is an array
    cr_assert(cJSON_IsArray(json), "Expected JSON array in response.");

    // Iterate over the array of tickers
    cJSON *ticker = NULL;
    cJSON_ArrayForEach(ticker, json) {
        cr_assert(cJSON_IsObject(ticker), "Each 'ticker' entry should be an object.");
        
        // Validate marketId (string)
        cJSON *marketId = cJSON_GetObjectItem(ticker, "marketId");
        cr_assert(cJSON_IsString(marketId), "marketId should be a string.");
        
        // Validate bestBid (string -> number)
        cJSON *bestBid = cJSON_GetObjectItem(ticker, "bestBid");
        cr_assert(cJSON_IsString(bestBid), "bestBid should be a string.");
        char *endptr;
        double bestBid_num = strtod(bestBid->valuestring, &endptr);
        cr_assert(*endptr == '\0', "bestBid string could not be fully converted to a number.");
        cr_assert(bestBid_num > 0, "bestBid should be greater than 0, but got: %f", bestBid_num);

        // Validate bestAsk (string -> number)
        cJSON *bestAsk = cJSON_GetObjectItem(ticker, "bestAsk");
        cr_assert(cJSON_IsString(bestAsk), "bestAsk should be a string.");
        double bestAsk_num = strtod(bestAsk->valuestring, &endptr);
        cr_assert(*endptr == '\0', "bestAsk string could not be fully converted to a number.");
        cr_assert(bestAsk_num > 0, "bestAsk should be greater than 0, but got: %f", bestAsk_num);

        // Validate lastPrice (string -> number)
        cJSON *lastPrice = cJSON_GetObjectItem(ticker, "lastPrice");
        cr_assert(cJSON_IsString(lastPrice), "lastPrice should be a string.");
        double lastPrice_num = strtod(lastPrice->valuestring, &endptr);
        cr_assert(*endptr == '\0', "lastPrice string could not be fully converted to a number.");
        cr_assert(lastPrice_num > 0, "lastPrice should be greater than 0, but got: %f", lastPrice_num);

        // Validate volume24h (string -> number)
        cJSON *volume24h = cJSON_GetObjectItem(ticker, "volume24h");
        cr_assert(cJSON_IsString(volume24h), "volume24h should be a string.");
        double volume24h_num = strtod(volume24h->valuestring, &endptr);
        cr_assert(*endptr == '\0', "volume24h string could not be fully converted to a number.");
        cr_assert(volume24h_num > 0, "volume24h should be greater than 0, but got: %f", volume24h_num);

        // Validate price24h (string -> number)
        cJSON *price24h = cJSON_GetObjectItem(ticker, "price24h");
        cr_assert(cJSON_IsString(price24h), "price24h should be a string.");
        double price24h_num = strtod(price24h->valuestring, &endptr);
        cr_assert(*endptr == '\0', "price24h string could not be fully converted to a number.");
        cr_assert(price24h_num != 0, "price24h should not be zero.");

        // Validate pricePct24h (string -> number)
        cJSON *pricePct24h = cJSON_GetObjectItem(ticker, "pricePct24h");
        cr_assert(cJSON_IsString(pricePct24h), "pricePct24h should be a string.");
        strtod(pricePct24h->valuestring, &endptr);  // Only parsing, no need for validation here

        // Validate low24h (string -> number)
        cJSON *low24h = cJSON_GetObjectItem(ticker, "low24h");
        cr_assert(cJSON_IsString(low24h), "low24h should be a string.");
        double low24h_num = strtod(low24h->valuestring, &endptr);
        cr_assert(*endptr == '\0', "low24h string could not be fully converted to a number.");
        cr_assert(low24h_num > 0, "low24h should be greater than 0, but got: %f", low24h_num);

        // Validate high24h (string -> number)
        cJSON *high24h = cJSON_GetObjectItem(ticker, "high24h");
        cr_assert(cJSON_IsString(high24h), "high24h should be a string.");
        double high24h_num = strtod(high24h->valuestring, &endptr);
        cr_assert(*endptr == '\0', "high24h string could not be fully converted to a number.");
        cr_assert(high24h_num > 0, "high24h should be greater than 0, but got: %f", high24h_num);

        // Validate timestamp (string)
        cJSON *timestamp = cJSON_GetObjectItem(ticker, "timestamp");
        cr_assert(cJSON_IsString(timestamp), "timestamp should be a string.");
    }

    cJSON_Delete(json);
    free(response);
}

Test(market_data_tests, get_multiple_orderbooks_structure_test) {
    char *ids[] = {"BTC-AUD", "XRP-AUD", "ETH-AUD"};
    GetMultipleOrderbooksQueryParams params = {
        .market_ids = ids,
        .num_market_ids = 3
    };

    char *response = get_multiple_orderbooks(&params);
    cr_assert_not_null(response, "Expected a non-null response from get_multiple_orderbooks.");

    cJSON *json = cJSON_Parse(response);
    cr_assert_not_null(json, "Failed to parse response JSON: %s", response);
    cr_assert(cJSON_IsArray(json), "Expected JSON array in response.");

    // Loop through each market orderbook object
    cJSON *orderbook = NULL;
    cJSON_ArrayForEach(orderbook, json) {
        cr_assert(cJSON_IsObject(orderbook), "Each orderbook should be a JSON object.");

        cJSON *marketId = cJSON_GetObjectItem(orderbook, "marketId");
        cr_assert(cJSON_IsString(marketId), "marketId should be a string.");

        cJSON *snapshotId = cJSON_GetObjectItem(orderbook, "snapshotId");
        cr_assert(cJSON_IsNumber(snapshotId), "snapshotId should be a number.");

        // Validate 'asks' array
        cJSON *asks = cJSON_GetObjectItem(orderbook, "asks");
        cr_assert(cJSON_IsArray(asks), "asks should be a JSON array.");
        cJSON *ask = NULL;
        cJSON_ArrayForEach(ask, asks) {
            cr_assert(cJSON_IsArray(ask), "Each ask entry should be an array [price, amount].");
            cr_assert(cJSON_GetArraySize(ask) == 2, "Each ask must contain exactly two elements.");
            cJSON *price = cJSON_GetArrayItem(ask, 0);
            cJSON *amount = cJSON_GetArrayItem(ask, 1);
            cr_assert(cJSON_IsString(price), "Ask price should be a string.");
            cr_assert(cJSON_IsString(amount), "Ask amount should be a string.");
        }

        // Validate 'bids' array
        cJSON *bids = cJSON_GetObjectItem(orderbook, "bids");
        cr_assert(cJSON_IsArray(bids), "bids should be a JSON array.");
        cJSON *bid = NULL;
        cJSON_ArrayForEach(bid, bids) {
            cr_assert(cJSON_IsArray(bid), "Each bid entry should be an array [price, amount].");
            cr_assert(cJSON_GetArraySize(bid) == 2, "Each bid must contain exactly two elements.");
            cJSON *price = cJSON_GetArrayItem(bid, 0);
            cJSON *amount = cJSON_GetArrayItem(bid, 1);
            cr_assert(cJSON_IsString(price), "Bid price should be a string.");
            cr_assert(cJSON_IsString(amount), "Bid amount should be a string.");
        }
    }

    cJSON_Delete(json);
    free(response);
}
