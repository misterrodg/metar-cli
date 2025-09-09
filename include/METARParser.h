#ifndef METAR_PARSER_H
#define METAR_PARSER_H

#include <ctime>
#include <regex>
#include <string>
#include <vector>

struct CloudCoverage {
    int altitude;
    std::string coverage;
    bool is_ceiling;
};

struct Variability {
    int from_value;
    int to_value;
};

class METARParser {
public:
    METARParser(const std::string& metar);

    std::string to_string();

private:
    static const std::regex m_r_airport;
    static const std::regex m_r_timestamp;
    static const std::regex m_r_wind;
    static const std::regex m_r_variability;
    static const std::regex m_r_visibility;
    static const std::regex m_r_clear_skies;
    static const std::regex m_r_cloud_cover;
    static const std::regex m_r_temp_dewpt;
    static const std::regex m_r_pressure;

    std::string m_metar_string;
    std::string m_airport_id;
    time_t m_timestamp;
    int m_wind_direction;
    int m_wind_speed;
    int m_wind_gust;
    Variability m_variability;
    float m_visibility;
    bool m_is_clr;
    bool m_is_skc;
    std::vector<CloudCoverage> m_cloud_coverage;
    int m_temperature;
    int m_dewpoint;
    float m_pressure;

    void process_airport();
    void process_timestamp();
    void process_wind();
    void process_variability();
    void process_visibility();
    void process_cloud_cover();
    void process_temp_dewpt();
    void process_pressure();
};

#endif
