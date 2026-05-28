#ifndef UNITS_H
#define UNITS_H

#include <ostream>

enum class PressureUnit { HPA, INHG, UNKNOWN };
std::ostream& operator<<(std::ostream& os, PressureUnit unit);

enum class VisibilityUnit { FEET, KILOMETERS, METERS, STATUTE_MILES, UNKNOWN };
std::ostream& operator<<(std::ostream& os, VisibilityUnit unit);

enum class WindUnit { KNOTS, KILOMETERS_PER_HOUR, METERS_PER_SECOND, UNKNOWN };
std::ostream& operator<<(std::ostream& os, WindUnit unit);

#endif