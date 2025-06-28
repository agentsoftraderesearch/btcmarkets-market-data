#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#include "btcmarkets/market_data.h"

ApiVersion api_version = API_UNKNOWN;
ApiContext api;

// Call from main to initialise API-context
int initialise_api_context(const char *base_url) {
    curl_global_init(CURL_GLOBAL_DEFAULT);

    api.base_url = base_url;
    api.curl = curl_easy_init();
    if (api.curl == NULL) {
        curl_global_cleanup();
        return -1;
    }
    api.default_headers = curl_slist_append(api.default_headers, "Content-Type: application/json");

    if (strstr(base_url, "/v3")) {
        api_version = API_V3;
    } else if (strstr(base_url, "/v2")) {
        api_version = API_V2;
    } else {
        api_version = API_UNKNOWN;
    }

    setup_market_data_api_dispatch();

    return 0;
}

void cleanup_api_context(void) {
    curl_slist_free_all(api.default_headers);
    curl_easy_cleanup(api.curl);
    curl_global_cleanup();
}

typedef struct {
    char *data;
    size_t size;
} MemoryBuffer;

static size_t write_callback(void *data, size_t size, size_t num_chunks, void *user_pointer) {
    size_t real_size = size * num_chunks;
    MemoryBuffer *callback_buffer = (MemoryBuffer *)user_pointer;

    char *ptr = realloc(callback_buffer->data, callback_buffer->size + real_size + 1);
    if (!ptr) return 0;

    callback_buffer->data = ptr;
    memcpy(&(callback_buffer->data[callback_buffer->size]), data, real_size);
    callback_buffer->size += real_size;
    callback_buffer->data[callback_buffer->size] = 0;

    return real_size;
}

char *api_get(ApiContext *api, const char *endpoint) {
    char url[512];
    snprintf(url, sizeof(url), "%s/%s", api->base_url, endpoint);

    MemoryBuffer chunk = {0};

    curl_easy_setopt(api->curl, CURLOPT_URL, url);
    curl_easy_setopt(api->curl, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(api->curl, CURLOPT_HTTPHEADER, api->default_headers);
    curl_easy_setopt(api->curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(api->curl, CURLOPT_WRITEDATA, (void *)&chunk);

    if (curl_easy_perform(api->curl) != CURLE_OK) {
        return NULL;
    }

    return chunk.data;
}

char *api_post(ApiContext *api, const char *endpoint, const char *post_data) {
    char url[512];
    snprintf(url, sizeof(url), "%s/%s", api->base_url, endpoint);

    MemoryBuffer chunk = {0};

    curl_easy_setopt(api->curl, CURLOPT_URL, url);
    curl_easy_setopt(api->curl, CURLOPT_POST, 1L);
    curl_easy_setopt(api->curl, CURLOPT_POSTFIELDS, post_data);
    curl_easy_setopt(api->curl, CURLOPT_HTTPHEADER, api->default_headers);
    curl_easy_setopt(api->curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(api->curl, CURLOPT_WRITEDATA, (void *)&chunk);

    if (curl_easy_perform(api->curl) != CURLE_OK) {
        return NULL;
    }

    return chunk.data;
}
