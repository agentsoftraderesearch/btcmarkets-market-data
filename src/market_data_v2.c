#include <stdio.h>
#include <string.h>

#include "btcmarkets/market_data_api_init.h"

// Example placeholder for API v2 NOT IMPLEMENTED

char* list_active_markets_v2() {
    printf("V2 API Not implemented: Placeholder example for V2 api.\n");
    char endpoint[30];
    snprintf(endpoint, sizeof(endpoint), "markets");
    char *resp = api_get(&api, endpoint);
    if (resp) {
        printf("V2 List Active Markets Response:\n%s\n", resp);
    } else {
        fprintf(stderr, "V2 List Active Markets request failed.\n");
    }
    return resp;
}

char* get_market_ticker_v2(char *market_id) {
    printf("V2 API Not implemented: Placeholder example for V2 api.\n");
    char endpoint[30];
    snprintf(endpoint, sizeof(endpoint), "markets/%s/ticker", market_id);
    char *resp = api_get(&api, endpoint);
    if (resp) {
        printf("V2 Ticker Response:\n%s\n", resp);
    } else {
        fprintf(stderr, "V2 Ticker request failed.\n");
    }
    return resp;
}
