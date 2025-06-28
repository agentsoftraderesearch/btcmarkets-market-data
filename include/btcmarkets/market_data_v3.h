#ifndef __BTCMARKETS_MARKET_DATA_V3_H__
#define __BTCMARKETS_MARKET_DATA_V3_H__

#include "btcmarkets/market_data.h"
#include "btcmarkets/market_data_types.h"

// https://docs.btcmarkets.net/#tag/Market-Data-APIs

char* list_active_markets_v3(void);
char* get_market_ticker_v3(char *market_id);
char* get_market_trades_v3(char *market_id, GetMarketTradesQueryParams *query_params);
char* get_market_orderbook_v3(char *market_id, GetMarketOrderbookQueryParams *query_params);
char* get_market_candles_v3(char *market_id, GetMarketCandlesQueryParams *query_params);
char* get_multiple_tickers_v3(GetMultipleTickersQueryParams *query_params);
char* get_multiple_orderbooks_v3(GetMultipleOrderbooksQueryParams *query_params);

#endif
