#include "METAR.h"

METAR::METAR()
    : type(""), station_id(""), report_time(std::nullopt),
      modifiers(), wind(std::nullopt), is_cavok(false),
      visibility(std::nullopt), rvr(""), weather(""), is_clr(false),
      is_skc(false), is_nsc(false), is_ncd(false), cloud_coverage(),
      temperature(std::nullopt), dewpoint(std::nullopt),
      pressure(std::nullopt), remarks("") {
}

std::ostream& operator<<(std::ostream& os, VisibilityUnit unit) {
    switch (unit) {
    case VisibilityUnit::FEET:
        os << "feet";
        break;
    case VisibilityUnit::KILOMETERS:
        os << "kilometers";
        break;
    case VisibilityUnit::METERS:
        os << "meters";
        break;
    case VisibilityUnit::STATUTE_MILES:
        os << "statute miles";
        break;
    default:
        os << "unknown unit";
        break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, WindUnit unit) {
    switch (unit) {
    case WindUnit::KNOTS:
        os << "knots";
        break;
    case WindUnit::KILOMETERS_PER_HOUR:
        os << "kilometers per hour";
        break;
    case WindUnit::METERS_PER_SECOND:
        os << "meters per second";
        break;
    default:
        os << "unknown unit";
        break;
    }
    return os;
}