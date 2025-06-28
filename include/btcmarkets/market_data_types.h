#ifndef __BTCMARKETS_MARKET_DATA_TYPES_H__
#define __BTCMARKETS_MARKET_DATA_TYPES_H__

#include <stdint.h>

typedef struct {
    int64_t before;
    int64_t after;
    int32_t limit;
} GetMarketTradesQueryParams;

typedef struct {
    int level;
} GetMarketOrderbookQueryParams;

typedef struct {
    char *time_window;
    char *from;
    char *to;
    int64_t before;
    int64_t after;
    int32_t limit;
} GetMarketCandlesQueryParams;

typedef struct {
    char **market_ids;
    size_t num_market_ids;
} GetMultipleTickersQueryParams;

typedef struct {
    char **market_ids;
    size_t num_market_ids;
} GetMultipleOrderbooksQueryParams;

#endif
