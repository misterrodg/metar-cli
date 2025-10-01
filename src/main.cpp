#include "ArgParser.h"
#include "METARParser.h"
#include "WebHandler.h"
#include "Utils.h"
#include <stdio.h>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <airport_id> [airport_ids...]\n", argv[0]);
        return 1;
    }

    ArgParser airport_args = ArgParser(argc, argv);
    std::string airport_string = airport_args.get_airport_string();
    bool use_translation = airport_args.get_use_translation();

    if (airport_string.empty()) {
        fprintf(stderr, "Error: airport_id required.\n");
        return 1;
    }

    std::string metar_string = fetch_metar(airport_string);

    printf("%s\n",metar_string.c_str());

    if (use_translation) {
        std::vector<std::string> metars = split_strings(metar_string, '\n');
        int length = metars.size();
        for (int i = 0; i < length; i++) {
            METARParser metar = METARParser(metars[i]);
            printf("\n%s", metar.to_string().c_str());
        }
    }

    return 0;
}
