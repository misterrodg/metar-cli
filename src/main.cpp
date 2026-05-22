#include "ArgParser.h"
#include "METARParser.h"
#include "Spinner.h"
#include "Utils.h"
#include "WebHandler.h"

#include <cstdio>
#include <stdio.h>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <airport_id> [airport_ids...]\n", argv[0]);
        return 1;
    }

    ArgParser airport_args = ArgParser(argc, argv);
    std::string airport_string = airport_args.get_airport_string();
    std::string metar_string = airport_args.get_metar_string();
    bool use_translation = airport_args.should_translate();
    bool use_metar_string = airport_args.should_use_metar();

    if (airport_string.empty() && !use_metar_string) {
        fprintf(stderr, "Error: airport_id required.\n");
        return 1;
    }

    if (metar_string.empty() && use_metar_string) {
        fprintf(stderr, "Error: METAR switch provided with no METAR string.\n");
        return 1;
    }

    if (airport_args.has_invalid_args()) {
        fprintf(stderr, "Error: unrecognized arg(s): %s\n",
                airport_args.get_invalid_arg_string().c_str());
        return 1;
    }

    if (metar_string.empty()) {
        Spinner spinner;

        spinner.start();
        metar_string = fetch_metar(airport_string);
        spinner.stop();

        if (metar_string.empty()) {
            fprintf(stderr,
                    "Error: failed to fetch METAR data for %s.\n",
                    airport_string.c_str());
            return 1;
        }
    }

    printf("%s\n", metar_string.c_str());

    if (use_translation) {
        std::vector<std::string> metars = split_strings(metar_string, '\n');

        for (const std::string& metar_text : metars) {
            if (metar_text.empty()) {
                continue;
            }

            METARParser metar(metar_text);
            printf("\n%s", metar.to_string().c_str());
        }
    }

    return 0;
}
