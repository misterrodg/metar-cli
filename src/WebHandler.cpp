#include "WebHandler.h"

#include <cstdio>
#include <curl/curl.h>

size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    auto* buffer = static_cast<std::string*>(userp);
    buffer->append(static_cast<char*>(contents), size * nmemb);
    return size * nmemb;
}

std::string fetch_metar(const std::string& airport_id) {
    CURL* curl = curl_easy_init();
    std::string read_buffer;

    if (!curl) {
        return "";
    }

    std::string base_url = "https://aviationweather.gov/api/data/metar?ids=";
    std::string full_url = base_url + airport_id;

    curl_easy_setopt(curl, CURLOPT_URL, full_url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "metar-cli/1.0");
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 8L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        fprintf(stderr, "cURL failed: %s\n", curl_easy_strerror(res));
        read_buffer.clear();
    } else {
        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        if (http_code >= 400) {
            fprintf(stderr, "Request failed with HTTP status %ld\n", http_code);
            read_buffer.clear();
        }
    }

    curl_easy_cleanup(curl);

    return read_buffer;
}
