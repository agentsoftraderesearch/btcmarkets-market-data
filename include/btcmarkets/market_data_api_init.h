#ifndef __BTCMARKETS_INTERNAL_H__
#define __BTCMARKETS_INTERNAL_H__

#include <curl/curl.h>

#include "btcmarkets/market_data.h"
#include "btcmarkets/market_data_types.h"

// API versions
typedef enum {
    API_V2,
    API_V3,
    API_UNKNOWN
} ApiVersion;

extern ApiVersion api_version;

// API context
typedef struct {
    const char *base_url;
    struct curl_slist *default_headers;
    CURL *curl;
} ApiContext;

extern ApiContext api;

int initialise_api_context(const char *base_url);
void cleanup_api_context();

// // HTTP request functions
char* api_get(ApiContext *api, const char *endpoint);
char* api_post(ApiContext *api, const char *endpoint, const char *post_data);

#endif
