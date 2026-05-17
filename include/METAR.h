#ifndef METAR_H
#define METAR_H

#include <optional>
#include <ostream>
#include <string>
#include <vector>

struct CloudCoverage {
    int altitude;
    std::string coverage;
    bool is_ceiling;
};

struct Pressure {
    double pressure;
    std::string unit;
};

struct ReportTime {
    int day;
    int hour;
    int minute;
};

struct Variability {
    int from_value;
    int to_value;
};

enum class VisibilityUnit { FEET, KILOMETERS, METERS, STATUTE_MILES, UNKNOWN };

struct Visibility {
    std::optional<double> value;
    VisibilityUnit unit;
    bool less_than;
    bool greater_or_equal;
};

std::ostream& operator<<(std::ostream& os, VisibilityUnit unit);

enum class WindUnit { KNOTS, KILOMETERS_PER_HOUR, METERS_PER_SECOND, UNKNOWN };

struct Wind {
    int direction;
    int speed;
    WindUnit unit;
    bool is_variable;
    std::optional<int> gust;
    std::optional<Variability> variability;
};

std::ostream& operator<<(std::ostream& os, WindUnit unit);

struct METAR {
    METAR();
    std::string type;
    std::string station_id;
    std::optional<ReportTime> report_time;
    std::vector<std::string> modifiers;
    std::optional<Wind> wind;
    bool is_cavok;
    std::optional<Visibility> visibility;
    std::string rvr;
    std::string weather;
    bool is_clr;
    bool is_skc;
    bool is_nsc;
    bool is_ncd;
    std::vector<CloudCoverage> cloud_coverage;
    std::optional<int> temperature;
    std::optional<int> dewpoint;
    std::optional<Pressure> pressure;
    std::string remarks;
};

#endif
