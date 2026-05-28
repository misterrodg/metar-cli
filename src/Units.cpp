#include "Units.h"

std::ostream& operator<<(std::ostream& os, PressureUnit unit) {
    switch (unit) {
    case PressureUnit::HPA:
        return os << "hPa";
    case PressureUnit::INHG:
        return os << "inHg";
    default:
        return os << "unknown unit";
    }
}

std::ostream& operator<<(std::ostream& os, VisibilityUnit unit) {
    switch (unit) {
    case VisibilityUnit::FEET:
        return os << "feet";
    case VisibilityUnit::KILOMETERS:
        return os << "kilometers";
    case VisibilityUnit::METERS:
        return os << "meters";
    case VisibilityUnit::STATUTE_MILES:
        return os << "statute miles";
    default:
        return os << "unknown unit";
    }
}

std::ostream& operator<<(std::ostream& os, WindUnit unit) {
    switch (unit) {
    case WindUnit::KNOTS:
        return os << "knots";
    case WindUnit::KILOMETERS_PER_HOUR:
        return os << "kilometers per hour";
    case WindUnit::METERS_PER_SECOND:
        return os << "meters per second";
    default:
        return os << "unknown unit";
    }
}
