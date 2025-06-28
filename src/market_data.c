#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>

#include "btcmarkets/market_data_api_init.h"
#include "btcmarkets/market_data_types.h"
#include "btcmarkets/market_data.h"

MarketDataApi market_data_api;

// Called from initialise_api_context to map API-functions
void setup_market_data_api_dispatch() {
    switch (api_version) {
        case API_V3:
            market_data_api.list_active_markets = list_active_markets_v3;
            market_data_api.get_market_ticker = get_market_ticker_v3;
            market_data_api.get_market_trades = get_market_trades_v3;
            market_data_api.get_market_orderbook = get_market_orderbook_v3;
            market_data_api.get_market_candles = get_market_candles_v3;
            market_data_api.get_multiple_tickers = get_multiple_tickers_v3;
            market_data_api.get_multiple_orderbooks = get_multiple_orderbooks_v3;

            break;
        case API_V2:
            market_data_api.list_active_markets = list_active_markets_v2;
            market_data_api.get_market_ticker = get_market_ticker_v2;
            break;
        default:
            fprintf(stderr, "Unsupported Market Data API version\n");
            exit(EXIT_FAILURE);
    }
}

char* list_active_markets() {
    return market_data_api.list_active_markets();
}

char* get_market_ticker(char *market_id) {
    return market_data_api.get_market_ticker(market_id);
}

char* get_market_trades(char *market_id, GetMarketTradesQueryParams *query_params) {
    return market_data_api.get_market_trades(market_id, query_params);
}

char* get_market_orderbook(char *market_id, GetMarketOrderbookQueryParams *query_params) {
    return market_data_api.get_market_orderbook(market_id, query_params);
}

char* get_market_candles(char *market_id, GetMarketCandlesQueryParams *query_params) {
    return market_data_api.get_market_candles(market_id, query_params);
}

char* get_multiple_tickers(GetMultipleTickersQueryParams *query_params) {
    return market_data_api.get_multiple_tickers(query_params);
}

char* get_multiple_orderbooks(GetMultipleOrderbooksQueryParams *query_params) {
    return market_data_api.get_multiple_orderbooks(query_params);
}
