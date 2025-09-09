#include "WebHandler.h"
#include <stdio.h>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <airport_id>\n", argv[0]);
        return 1;
    }

    std::string airport_id = "";
    airport_id = argv[1];

    if (airport_id.empty()) {
        fprintf(stderr, "Error: airport_id required.\n");
        return 1;
    }

    std::string metar_string = fetch_metar(airport_id);
    printf("%s",metar_string.c_str());

    return 0;
}
