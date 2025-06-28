#ifndef __BTCMARKETS_MARKET_DATA_H__
#define __BTCMARKETS_MARKET_DATA_H__

#include "btcmarkets/market_data_api_init.h"
// #include "btcmarkets/market_data_types.h"
#include "btcmarkets/market_data_v2.h" // Placeholder example for V2 API
#include "btcmarkets/market_data_v3.h"

// API endpoints for version dispatch
typedef struct {
    char* (*list_active_markets)(void);
    char* (*get_market_ticker)(char *market_id);
    char* (*get_market_trades)(char *market_id, GetMarketTradesQueryParams *query_params);
    char* (*get_market_orderbook)(char *market_id, GetMarketOrderbookQueryParams *query_params);
    char* (*get_market_candles)(char *market_id, GetMarketCandlesQueryParams *query_params);
    char* (*get_multiple_tickers)(GetMultipleTickersQueryParams *query_params);
    char* (*get_multiple_orderbooks)(GetMultipleOrderbooksQueryParams *query_params);
} MarketDataApi;

extern MarketDataApi market_data_api;

void setup_market_data_api_dispatch(void);

char* list_active_markets(void);
char* get_market_ticker(char *market_id);
char* get_market_trades(char *market_id, GetMarketTradesQueryParams *query_params);
char* get_market_orderbook(char *market_id, GetMarketOrderbookQueryParams *query_params);
char* get_market_candles(char *market_id, GetMarketCandlesQueryParams *query_params);
char* get_multiple_tickers(GetMultipleTickersQueryParams *query_params);
char* get_multiple_orderbooks(GetMultipleOrderbooksQueryParams *query_params);

#endif
