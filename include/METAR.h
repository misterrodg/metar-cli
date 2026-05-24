#ifndef METAR_H
#define METAR_H

#include "Units.h"

#include <optional>
#include <string>
#include <vector>

struct CloudCoverage {
    int altitude;
    std::string coverage;
    bool is_ceiling;
};

struct Pressure {
    double pressure;
    PressureUnit unit;
};

struct ReportTime {
    std::optional<int> hour;
    int minute;
};

std::ostream& operator<<(std::ostream& os, ReportTime report_time);

struct ReportDateTime {
    std::optional<int> date;
    ReportTime time;
};

std::ostream& operator<<(std::ostream& os, ReportDateTime report_date_time);

struct Variability {
    int from_value;
    int to_value;
};

struct Visibility {
    std::optional<double> value;
    VisibilityUnit unit;
    bool less_than;
    bool greater_or_equal;
};

enum class Intensity { LIGHT, HEAVY, VICINITY, UNKNOWN };

std::ostream& operator<<(std::ostream& os, Intensity intensity);

enum class Descriptor {
    SHALLOW,
    PARTIAL,
    PATCHES,
    LOW_DRIFTING,
    BLOWING,
    SHOWERS,
    THUNDERSTORM,
    FREEZING,
    UNKNOWN
};

std::ostream& operator<<(std::ostream& os, Descriptor descriptor);

enum class Precipitation {
    DRIZZLE,
    RAIN,
    SNOW,
    SNOW_GRAINS,
    ICE_CRYSTALS,
    ICE_PELLETS,
    HAIL,
    SMALL_HAIL,
    UNKNOWN_PRECIP,
    UNKNOWN
};

std::ostream& operator<<(std::ostream& os, Precipitation precipitation);

enum class Obscuration {
    MIST,
    FOG,
    SMOKE,
    VOLCANIC_ASH,
    WIDESPREAD_DUST,
    SAND,
    HAZE,
    SPRAY,
    UNKNOWN
};

std::ostream& operator<<(std::ostream& os, Obscuration obscuration);

enum class OtherPhenomenon {
    DUST_WHIRLS,
    SQUALLS,
    FUNNEL_CLOUD,
    SANDSTORM,
    DUSTSTORM,
    UNKNOWN
};

std::ostream& operator<<(std::ostream& os, OtherPhenomenon phenomenon);

struct Weather {
    std::optional<Intensity> intensity;
    std::optional<Descriptor> descriptor;
    std::optional<Precipitation> precipitation;
    std::optional<Obscuration> obscuration;
    std::optional<OtherPhenomenon> other_phenomenon;
};

struct Wind {
    int direction;
    int speed;
    WindUnit unit;
    bool is_variable;
    std::optional<int> gust;
    std::optional<Variability> variability;
};

enum class BoundaryStatus { PLUS, MINUS, UNKNOWN };

std::ostream& operator<<(std::ostream& os, BoundaryStatus status);

enum class VisibilityTendency { INCREASING, STEADY, DECREASING, UNKNOWN };

std::ostream& operator<<(std::ostream& os, VisibilityTendency tendency);

struct RVR {
    std::string runway;
    std::optional<BoundaryStatus> boundary_status;
    std::optional<int> constant_distance;
    std::optional<BoundaryStatus> variable_min_distance_status;
    std::optional<int> variable_min_distance;
    std::optional<BoundaryStatus> variable_max_distance_status;
    std::optional<int> variable_max_distance;
    std::optional<VisibilityUnit> unit;
    std::optional<VisibilityTendency> tendency;
};

enum class ReportModifier { AUTO, COR, UNKNOWN };

std::ostream& operator<<(std::ostream& os, ReportModifier modifier);

enum class StationType { AO1, AO2, UNKNOWN };

std::ostream& operator<<(std::ostream& os, StationType type);

struct PeakWind {
    int direction;
    int speed;
    ReportTime time;
};

struct WindShift {
    ReportTime time;
    bool frontal_passage;
};

enum class SurfaceVisType { SFC, TWR, UNKNOWN };

std::ostream& operator<<(std::ostream& os, SurfaceVisType type);

struct SurfaceVisibility {
    SurfaceVisType type;
    std::optional<double> value;
    bool less_than;
    bool greater_or_equal;
};

struct METAR {
    METAR();
    std::string type;
    std::string station_id;
    std::optional<ReportDateTime> report_date_time;
    std::optional<ReportModifier> report_modifier;
    std::optional<Wind> wind;
    bool is_cavok;
    std::optional<Visibility> visibility;
    std::vector<RVR> rvr;
    std::vector<Weather> weather;
    bool is_clr;
    bool is_skc;
    bool is_nsc;
    bool is_ncd;
    std::vector<CloudCoverage> cloud_coverage;
    std::optional<int> temperature;
    std::optional<int> dewpoint;
    std::optional<Pressure> pressure;
    bool has_remarks;
    std::optional<StationType> station_type;
    std::optional<PeakWind> peak_wind;
    std::optional<WindShift> wind_shift;
    std::optional<SurfaceVisibility> surface_visibility;
};

#endif
