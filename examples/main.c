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

    char *list_active_markets_response = list_active_markets();
    if (list_active_markets_response) {
        printf("List Active Markets Response:\n%s\n", list_active_markets_response);
        free(list_active_markets_response);
    }

    char *get_market_ticker_response = get_market_ticker("BTC-AUD");
    if (get_market_ticker_response) {
        printf("Get Market Ticker Response:\n%s\n", get_market_ticker_response);
        free(get_market_ticker_response);
    }

    GetMarketTradesQueryParams get_market_trades_params = { 
        .before = 78234976,
        .limit = 3 
        };
    char *get_market_trades_response = get_market_trades("BTC-AUD", &get_market_trades_params);
    if (get_market_trades_response) {
        printf("Get Market Trades Response:\n%s\n", get_market_trades_response);
        free(get_market_trades_response);
    }

    GetMarketOrderbookQueryParams get_market_orderbook_params = { .level = 0 };
    char *get_market_orderbook_response = get_market_orderbook("BTC-AUD", &get_market_orderbook_params);
    if (get_market_orderbook_response) {
        printf("Get Market Orderbook Response:\n%s\n", get_market_orderbook_response);
        free(get_market_orderbook_response);
    }

    GetMarketCandlesQueryParams get_market_candles_params = {
        .time_window="1w",
        .from="2025-05-01T00:00:00.000000Z",
        .to="2025-05-05T00:00:00.000000Z",
        .limit=1
    };
    char *get_market_candles_response = get_market_candles("BTC-AUD", &get_market_candles_params);
    if (get_market_candles_response) {
        printf("Get Market Candles Response:\n%s\n", get_market_candles_response);
        free(get_market_candles_response);
    }

    char *multiple_tickers_ids[3] = {"BTC-AUD", "XRP-AUD", "ETH-AUD"};
    GetMultipleTickersQueryParams get_multiple_tickers_params = {
        .market_ids = multiple_tickers_ids,
        .num_market_ids = 3
        };
    char *get_multiple_tickers_response = get_multiple_tickers(&get_multiple_tickers_params);
    if (get_multiple_tickers_response) {
        printf("Get Multiple Tickers Response:\n%s\n", get_multiple_tickers_response);
        free(get_multiple_tickers_response);
    }

    char *multiple_orderbooks_ids[3] = {"BTC-AUD", "XRP-AUD", "ETH-AUD"};
    GetMultipleOrderbooksQueryParams get_multiple_orderbooks_params = {
        .market_ids = multiple_orderbooks_ids,
        .num_market_ids = 3
        };
    char *get_multiple_orderbooks_response = get_multiple_orderbooks(&get_multiple_orderbooks_params);
    if (get_multiple_orderbooks_response) {
        printf("Get Multiple Orderbooks Response:\n%s\n", get_multiple_orderbooks_response);
        free(get_multiple_orderbooks_response);
    }

    cleanup_api_context();

    return 0;
}
