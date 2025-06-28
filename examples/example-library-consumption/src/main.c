#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "btcmarkets/market_data.h"
// #include "btcmarkets/market_data_types.h"

int main() {
    // Example calls

    if (initialise_api_context("https://api.btcmarkets.net/v3") != 0) {
        fprintf(stderr, "Failed to initialize API context.\n");
        return 1;
    }

    GetMarketCandlesQueryParams candle_params = {
        .time_window="1w",
        .from="2025-05-01T00:00:00.000000Z",
        .to="2025-05-05T00:00:00.000000Z",
        .limit=1
    };
    char *candle_response = get_market_candles("BTC-AUD", &candle_params);
    if (candle_response) {
        free(candle_response);
    }

    char *multiple_tickers_ids[3] = {"BTC-AUD", "XRP-AUD", "ETH-AUD"};
    GetMultipleTickersQueryParams multiple_tickers_params = {
        .market_ids = multiple_tickers_ids,
        .num_market_ids = 3
        };
    char *multiple_tickers_response = get_multiple_tickers(&multiple_tickers_params);
    if (multiple_tickers_response) {
        free(multiple_tickers_response);
    }

    char *multiple_orderbooks_ids[3] = {"BTC-AUD", "XRP-AUD", "ETH-AUD"};
    GetMultipleOrderbooksQueryParams multiple_orderbooks_params = {
        .market_ids = multiple_orderbooks_ids,
        .num_market_ids = 3
        };
    char *multiple_orderbooks_response = get_multiple_orderbooks(&multiple_orderbooks_params);
    if (multiple_orderbooks_response) {
        free(multiple_orderbooks_response);
    }

    char *ticker_response = get_market_ticker("BTC-AUD");
    if (ticker_response) {
        free(ticker_response);
    }

    GetMarketTradesQueryParams trade_query_params = { 
        .before = 78234976,
        .limit = 3 
        };
    char *trade_response = get_market_trades("BTC-AUD", &trade_query_params);
    if (trade_response) {
        free(trade_response);
    }

    GetMarketOrderbookQueryParams orderbook_params = { .level = 0 };
    char *orderbook_response = get_market_orderbook("BTC-AUD", &orderbook_params);
    if (orderbook_response) {
        free(orderbook_response);
    }

    cleanup_api_context();

    return 0;
}
