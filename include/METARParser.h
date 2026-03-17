#ifndef METAR_PARSER_H
#define METAR_PARSER_H

#include <optional>
#include <regex>
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

struct Visibility {
    double distance_sm;
    bool less_than;
    bool greater_or_equal;
};

class METARParser {
public:
    explicit METARParser(const std::string& metar);

    std::string to_string() const;

private:
    static const std::regex station_regex_;
    static const std::regex timestamp_regex_;
    static const std::regex wind_regex_;
    static const std::regex variability_regex_;
    static const std::regex cavok_regex_;
    static const std::regex visibility_regex_;
    static const std::regex visibility_meters_regex_;
    static const std::regex clear_skies_regex_;
    static const std::regex cloud_cover_regex_;
    static const std::regex temp_dewpt_regex_;
    static const std::regex pressure_in_regex_;
    static const std::regex pressure_mb_regex_;

    std::string metar_string_;
    std::string station_id_;

    std::optional<ReportTime> report_time_;

    int wind_direction_;
    int wind_speed_;
    bool wind_is_variable_;
    std::optional<int> wind_gust_;

    std::optional<Variability> variability_;

    bool is_cavok_;
    std::optional<Visibility> visibility_;

    bool is_clr_;
    bool is_skc_;
    bool is_nsc_;
    bool is_ncd_;
    std::vector<CloudCoverage> cloud_coverage_;

    std::optional<int> temperature_;
    std::optional<int> dewpoint_;
    std::optional<Pressure> pressure_;

    void process_station();
    void process_report_time();
    void process_wind();
    void process_variability();
    void process_visibility();
    void process_cloud_cover();
    void process_temp_dewpt();
    void process_pressure();
};

#endif
