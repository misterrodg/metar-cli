#include "METARParser.h"
#include "Utils.h"
#include <cstddef>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <regex>
#include <sstream>

METARParser::METARParser(const std::string& metar) :
    m_metar_string(metar),
    m_airport_id(""),
    m_timestamp((time_t)(-1)),
    m_wind_direction(0),
    m_wind_speed(0),
    m_wind_gust(-1),
    m_variability({-1,-1}),
    m_visibility(0.0),
    m_is_clr(false),
    m_is_skc(false),
    m_cloud_coverage(),
    m_temperature(0),
    m_dewpoint(0),
    m_pressure(0.0)
{
    process_airport();
    process_timestamp();
    process_wind();
    process_variability();
    process_visibility();
    process_cloud_cover();
    process_temp_dewpt();
    process_pressure();
}

std::string METARParser::to_string() {
    std::ostringstream oss;

    std::tm tm = *std::localtime(&m_timestamp);

    oss << "Report for " << m_airport_id << ":" << "\n"
        << "\tAt " << std::put_time(&tm, "%Y-%m-%d %H:%M") << "Z" << "\n";
    oss << "\tWind ";
    bool skip_wind = false;
    if (m_wind_direction == 0 && m_wind_speed == 0) {
        oss << "calm" << "\n";
        skip_wind = true;
    }
    if (!skip_wind) {
        oss << "from " << m_wind_direction
            << " at " << m_wind_speed;
        if (m_wind_gust > 0) {
            oss << " gusting " << m_wind_gust;
        }
        oss << " knots" << "\n";
        if (m_variability.from_value != -1 && m_variability.to_value != -1) {
            oss << "\tVariable from " << m_variability.from_value
                << " to " << m_variability.to_value << "\n";
        }
    }
    oss << "\tVisibility ";
    bool skip_vis = false;
    if (m_visibility == 0) {
        oss << "not reported" << "\n";
        skip_vis = true;
    }
    if (m_visibility < 0 && !skip_vis) {
        oss << "less than ";
        m_visibility = -m_visibility;
    }
    if (!skip_vis) {
        oss << m_visibility << " SM" << "\n";
    }
    if (m_is_clr) {
        oss << "\t" << "Clear below 12,000 AGL" << "\n";
    }
    if (m_is_skc) {
        oss << "\t" << "Sky clear" << "\n";
    }
    if (!m_cloud_coverage.empty()) {
        oss << "\tCloud cover:" << "\n";
        int length = m_cloud_coverage.size();
        for (int i = length - 1; i >= 0; i--) {
            std::string coverage = m_cloud_coverage[i].coverage;
            int altitude = m_cloud_coverage[i].altitude;

            oss << "\t\t" << m_cloud_coverage[i].coverage
                << " at " << m_cloud_coverage[i].altitude << " AGL";
            if (m_cloud_coverage[i].is_ceiling) {
                oss << " --ceiling-- ";
            }
            oss << "\n";
        }
    }
    oss << "\tTemperature " << m_temperature << "C" << "\n"
        << "\tDewpoint " << m_dewpoint << "C" << "\n"
        << "\tPressure " << m_pressure << " inHg" << "\n";
    return oss.str();
}

void METARParser::process_airport() {
    std::smatch match_results;
    if (std::regex_search(m_metar_string, match_results, m_r_airport)) {
        m_airport_id = match_results[0].str();
    }
}

void METARParser::process_timestamp() {
    std::smatch match_results;
    if (std::regex_search(m_metar_string, match_results, m_r_timestamp)) {
        int day = std::stoi(match_results[1]);
        int hour = std::stoi(match_results[2]);
        int minute = std::stoi(match_results[3]);

        std::time_t now = std::time(nullptr);
        std::tm tm_now = *std::gmtime(&now);

        std::tm tm_target = tm_now;
        tm_target.tm_mday = day;
        tm_target.tm_hour = hour;
        tm_target.tm_min = minute;
        tm_target.tm_sec = 0;

        m_timestamp = std::mktime(&tm_target);
    }
}

void METARParser::process_wind() {
    std::smatch match_results;
    if (std::regex_search(m_metar_string, match_results, m_r_wind)) {
        m_wind_direction = std::stoi(match_results[1].str());
        m_wind_speed = std::stoi(match_results[2]);
        m_wind_gust = (
            match_results[3].matched ? std::stoi(match_results[3].str()) : -1
        );
    }
}

void METARParser::process_variability() {
    std::smatch match_results;
    if (std::regex_search(m_metar_string, match_results, m_r_variability)) {
        if (match_results[0].matched) {
            Variability v = {
                std::stoi(match_results[1].str()),
                std::stoi(match_results[2].str())
            };
            m_variability = v;
        }
    }
}

void METARParser::process_visibility() {
    std::smatch match_results;
    if (std::regex_search(m_metar_string, match_results, m_r_visibility)) {
        m_visibility = parse_fractional_number(match_results[2].str());
        if (match_results[1].str() == "M") {
            m_visibility = -m_visibility;
        }
    }
}

void METARParser::process_cloud_cover() {
    std::smatch match_results;
    if (std::regex_search(m_metar_string, match_results, m_r_clear_skies)) {
        if (match_results[1].str() == "CLR") {
            m_is_clr = true;
            return;
        }
        if (match_results[1].str() == "SKC") {
            m_is_skc = true;
            return;
        }
    }

    std::sregex_iterator next(
        m_metar_string.begin(), m_metar_string.end(), m_r_cloud_cover
    );
    std::sregex_iterator end;

    bool ceiling_set = false;
    while (next != end) {
        std::smatch match = *next;

        std::string cover = match.str(1);
        bool is_ceiling = false;

        if ((cover == "BKN" || cover == "OVC") && !ceiling_set) {
            is_ceiling = true;
            ceiling_set = true;
        }

        CloudCoverage cc = {
            std::stoi(match.str(2)) * 100,
            cover,
            is_ceiling
        };

        m_cloud_coverage.push_back(cc);
        next++;
    }
}

void METARParser::process_temp_dewpt() {
    std::smatch match_results;
    if (std::regex_search(m_metar_string, match_results, m_r_temp_dewpt)) {
        bool is_negative_t = match_results[1].matched;
        bool is_negative_d = match_results[3].matched;
        int temp = std::stoi(match_results[2].str());
        int dewpt = std::stoi(match_results[4].str());
        m_temperature = (is_negative_t ? -temp : temp);
        m_dewpoint = ( is_negative_d ? -dewpt : dewpt);
    }
}

void METARParser::process_pressure() {
    std::smatch match_results;
    if (std::regex_search(m_metar_string, match_results, m_r_pressure)) {
        int pressure = std::stoi(match_results[1].str());
        m_pressure = (float)pressure / 100;
    }
}

const std::regex METARParser::m_r_airport("K[A-Z0-9]{3}");
const std::regex METARParser::m_r_timestamp("([0-9]{2})([0-9]{2})([0-9]{2})Z");
const std::regex METARParser::m_r_wind("([0-9]{3})([0-9]{2})(?:G([0-9]{2}))?KT");
const std::regex METARParser::m_r_variability("([0-9]{3})V([0-9]{3})");
const std::regex METARParser::m_r_visibility("(M)?([0-9]{1,2}|[13]/[24]|[12]\\s[13]/[24])SM");
const std::regex METARParser::m_r_clear_skies("(SKC|CLR)");
const std::regex METARParser::m_r_cloud_cover("(FEW|SCT|BKN|OVC)([0-9]{3})");
const std::regex METARParser::m_r_temp_dewpt("(M)?([0-9]{2})/(M)?([0-9]{2})");
const std::regex METARParser::m_r_pressure("A([0-9]{4})");

