#include "Units.h"

std::ostream& operator<<(std::ostream& os, PressureUnit unit) {
    switch (unit) {
    case PressureUnit::HPA:
        os << "hPa";
        break;
    case PressureUnit::INHG:
        os << "inHg";
        break;
    default:
        os << "unknown unit";
        break;
    }
    return os;
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