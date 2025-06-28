#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#include "btcmarkets/market_data_api_init.h"

// https://docs.btcmarkets.net/#tag/Market-Data-APIs

char* list_active_markets_v3() {
    char endpoint[30];
    snprintf(endpoint, sizeof(endpoint), "markets");
    char *response = api_get(&api, endpoint);
    if (response) {
        printf("V3 List Active Markets Response (%s):\n%s\n", endpoint, response);
    } else {
        fprintf(stderr, "V3 List Active Markets request failed (%s):\n", endpoint);
        return NULL;
    }
    return response;
}

char* get_market_ticker_v3(char *market_id) {
    char endpoint[30];
    snprintf(endpoint, sizeof(endpoint), "markets/%s/ticker", market_id);
    char *response = api_get(&api, endpoint);
    if (response) {
        printf("V3 Get Market Ticker Response (%s):\n%s\n", endpoint, response);
    } else {
        fprintf(stderr, "V3 Get Market Ticker request failed (%s):\n", endpoint);
    }
    return response;
}

char* get_market_trades_v3(char *market_id, GetMarketTradesQueryParams *query_params) {
    char endpoint[256];
    snprintf(endpoint, sizeof(endpoint), "markets/%s/trades", market_id);
    int first_param_added = 0;

    if (query_params->before) {
        snprintf(endpoint + strlen(endpoint), sizeof(endpoint) - strlen(endpoint), "%sbefore=%" PRId64,
                 first_param_added ? "&" : "?",
                 query_params->before);
        first_param_added = 1;
    }
    if (query_params->after) {
        snprintf(endpoint + strlen(endpoint), sizeof(endpoint) - strlen(endpoint), "%safter=%" PRId64,
                 first_param_added ? "&" : "?",
                 query_params->after);
        first_param_added = 1;
    }
    if (query_params->limit) {
        snprintf(endpoint + strlen(endpoint), sizeof(endpoint) - strlen(endpoint), "%slimit=%d",
                 first_param_added ? "&" : "?",
                 query_params->limit);
    }

    char *response = api_get(&api, endpoint);
    if (response) {
        printf("V3 Get Market Trades Response (%s):\n%s\n", endpoint, response);
    } else {
        fprintf(stderr, "V3 Get Market Trades request failed (%s).\n", endpoint);
    }
    return response;
}

char* get_market_orderbook_v3(char *market_id, GetMarketOrderbookQueryParams *query_params) {
    char endpoint[128];
    snprintf(endpoint, sizeof(endpoint), "markets/%s/orderbook?level=%d", market_id, query_params->level);
    char *response = api_get(&api, endpoint);
    if (response) {
        printf("V3 Get Market Orderbook Response (%s):\n%s\n", endpoint, response);
    } else {
        fprintf(stderr, "V3 Get Market Orderbook request failed ( %s ).\n", endpoint);
    }
    return response;
}

char* get_market_candles_v3(char *market_id, GetMarketCandlesQueryParams *query_params) {
    char endpoint[256];
    snprintf(endpoint, sizeof(endpoint), "markets/%s/candles", market_id);
    int first_param_added = 0;

    if (query_params->time_window) {
        snprintf(endpoint + strlen(endpoint), sizeof(endpoint) - strlen(endpoint), "%stimeWindow=%s",
                 first_param_added ? "&" : "?",
                 query_params->time_window);
        first_param_added = 1;
    }
    if (query_params->from) {
        snprintf(endpoint + strlen(endpoint), sizeof(endpoint) - strlen(endpoint), "%sfrom=%s",
                 first_param_added ? "&" : "?",
                 query_params->from);
        first_param_added = 1;
    }
    if (query_params->to) {
        snprintf(endpoint + strlen(endpoint), sizeof(endpoint) - strlen(endpoint), "%sto=%s",
                 first_param_added ? "&" : "?",
                 query_params->to);
    }
    if (query_params->before) {
        snprintf(endpoint + strlen(endpoint), sizeof(endpoint) - strlen(endpoint), "%sbefore=%" PRId64,
                 first_param_added ? "&" : "?",
                 query_params->before);
        first_param_added = 1;
    }
    if (query_params->after) {
        snprintf(endpoint + strlen(endpoint), sizeof(endpoint) - strlen(endpoint), "%safter=%" PRId64,
                 first_param_added ? "&" : "?",
                 query_params->after);
        first_param_added = 1;
    }
    if (query_params->limit) {
        snprintf(endpoint + strlen(endpoint), sizeof(endpoint) - strlen(endpoint), "%slimit=%d",
                 first_param_added ? "&" : "?",
                 query_params->limit);
    }

    char *response = api_get(&api, endpoint);
    if (response) {
        printf("V3 Get Market Candles Response (%s):\n%s\n", endpoint, response);
    } else {
        fprintf(stderr, "V3 Get Market Candles request failed ( %s ).\n", endpoint);
    }
    return response;
}

char* get_multiple_tickers_v3(GetMultipleTickersQueryParams *query_params) {
    char endpoint[1024];
    size_t offset = snprintf(endpoint, sizeof(endpoint), "markets/tickers?");

    for (size_t i = 0; i < query_params->num_market_ids; i++) {
        int written = snprintf(
            endpoint + offset, sizeof(endpoint) - offset,
            "%smarketId=%s", (i > 0 ? "&" : ""),
            query_params->market_ids[i]
            );
        if (written < 0 || offset + (size_t)written >= sizeof(endpoint)) {
            fprintf(stderr, "Endpoint string too long!\n");
            return NULL;
        }
        offset += (size_t)written;
    }

    char *response = api_get(&api, endpoint);
    if (response) {
        printf("V3 Get Multiple Tickers Response (%s):\n%s\n", endpoint, response);
    } else {
        fprintf(stderr, "V3 Get Multiple Tickers request failed (%s).\n", endpoint);
    }

    return response;
}

char* get_multiple_orderbooks_v3(GetMultipleOrderbooksQueryParams *query_params) {
    char endpoint[1024];
    size_t offset = snprintf(endpoint, sizeof(endpoint), "markets/orderbooks?");

    for (size_t i = 0; i < query_params->num_market_ids; i++) {
        int written = snprintf(
            endpoint + offset, sizeof(endpoint) - offset,
            "%smarketId=%s", (i > 0 ? "&" : ""),
            query_params->market_ids[i]
            );
        if (written < 0 || offset + (size_t)written >= sizeof(endpoint)) {
            fprintf(stderr, "Endpoint string too long!\n");
            return NULL;
        }
        offset += (size_t)written;
    }

    char *response = api_get(&api, endpoint);
    if (response) {
        printf("V3 Get Multiple Orderbooks Response (%s):\n%s\n", endpoint, response);
    } else {
        fprintf(stderr, "V3 Get Multiple Orderbooks request failed ( %s ).\n", endpoint);
    }
    return response;
}
