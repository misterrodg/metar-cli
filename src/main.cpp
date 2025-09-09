#include "ArgParser.h"
#include "WebHandler.h"
#include <stdio.h>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <airport_id> [airport_ids...]\n", argv[0]);
        return 1;
    }

    ArgParser airport_args = ArgParser(argc, argv);
    std::string airport_string = airport_args.get_airport_string();

    if (airport_string.empty()) {
        fprintf(stderr, "Error: airport_id required.\n");
        return 1;
    }

    std::string metar_string = fetch_metar(airport_string);
    printf("%s",metar_string.c_str());

    return 0;
}
