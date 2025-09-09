#include "WebHandler.h"

#include <cstdio>
#include <curl/curl.h>
#include <curl/easy.h>

size_t write_callback(
    void* contents, size_t size, size_t nmemb, void* userp
) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string fetch_metar(const std::string &airport_id) {
    CURL* curl;
    CURLcode res;
    std::string read_buffer;

    std::string result = "";

    std::string base_url = "https://aviationweather.gov/api/data/metar?ids=";
    std::string full_url = base_url + airport_id;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, full_url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "cURL failed: %s\n", curl_easy_strerror(res));
        } else {
            result = read_buffer;
        }
    }
    curl_easy_cleanup(curl);
    return result;
}
