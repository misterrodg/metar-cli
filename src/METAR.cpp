#include "METAR.h"

METAR::METAR()
    : type(""), station_id(""), report_time(std::nullopt),
      modifiers(), wind(std::nullopt), is_cavok(false),
      visibility(std::nullopt), rvr(""), weather(""), is_clr(false),
      is_skc(false), is_nsc(false), is_ncd(false), cloud_coverage(),
      temperature(std::nullopt), dewpoint(std::nullopt),
      pressure(std::nullopt), remarks("") {
}
