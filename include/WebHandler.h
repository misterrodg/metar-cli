#ifndef WEB_HANDLER_H
#define WEB_HANDLER_H

#include <cstddef>
#include <string>

size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp);
std::string fetch_metar(const std::string &airport_id);

#endif
