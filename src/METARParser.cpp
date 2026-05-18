#include "METARParser.h"
#include "Patterns.h"
#include "RegexUtils.h"
#include "TokenStream.h"
#include "Utils.h"

#include <iomanip>
#include <regex>
#include <sstream>
#include <string>

METARParser::METARParser(const std::string& metar)
    : metar_string_(metar), metar_() {
    TokenStream tokens{split_on_whitespace(metar)};

    metar_.visibility =
        Visibility{std::nullopt, VisibilityUnit::UNKNOWN, false, false};
    metar_.wind =
        Wind{0, 0, WindUnit::UNKNOWN, false, std::nullopt, std::nullopt};

    parse_type(tokens);
    parse_station_id(tokens);
    parse_report_time(tokens);
    parse_modifiers(tokens);
    parse_wind(tokens);
    parse_visibility(tokens);
    parse_rvr_block(tokens);
    parse_weather_block(tokens);
    parse_sky_block(tokens);
    parse_temp_dwpt(tokens);
    parse_pressure(tokens);
}

void METARParser::parse_type(TokenStream& ts) {
    std::string_view next = ts.peek();
    if (ts.eof() || !(next == "METAR" || next == "SPECI")) {
        return;
    }
    metar_.type = ts.consume();
}

void METARParser::parse_station_id(TokenStream& ts) {
    if (ts.eof() ||
        !std::regex_match(ts.peek().begin(), ts.peek().end(),
                          RegexUtils::make_token_regex(Patterns::STATION))) {
        return;
    }
    metar_.station_id = ts.consume();
}

void METARParser::parse_report_time(TokenStream& ts) {
    if (ts.eof() ||
        !std::regex_match(ts.peek().begin(), ts.peek().end(),
                          RegexUtils::make_token_regex(Patterns::TIMESTAMP))) {
        return;
    }

    const std::string time_token = ts.consume();
    std::smatch match_results;
    if (std::regex_match(time_token, match_results,
                         RegexUtils::make_token_regex(Patterns::TIMESTAMP))) {
        int day = std::stoi(match_results[1].str());
        int hour = std::stoi(match_results[2].str());
        int minute = std::stoi(match_results[3].str());

        metar_.report_time = ReportTime{day, hour, minute};
    }
}

void METARParser::parse_modifiers(TokenStream& ts) {
    while (!ts.eof()) {
        std::string_view next = ts.peek();
        if (next != "AUTO" && next != "COR" && next != "NIL" && next != "AMD") {
            break;
        }
        metar_.modifiers.push_back(ts.consume());
    }
}

void METARParser::parse_wind(TokenStream& ts) {
    if (ts.eof() ||
        !std::regex_match(ts.peek().begin(), ts.peek().end(),
                          RegexUtils::make_token_regex(Patterns::WIND))) {
        return;
    }

    const std::string wind_token = ts.consume();
    std::smatch match_results;
    if (std::regex_match(wind_token, match_results,
                         RegexUtils::make_token_regex(Patterns::WIND))) {
        const std::string dir = match_results[1].str();
        if (dir == "VRB") {
            metar_.wind->is_variable = true;
        } else {
            metar_.wind->direction = std::stoi(dir);
        }

        metar_.wind->speed = std::stoi(match_results[2].str());

        if (match_results[4].matched) {
            metar_.wind->gust = std::stoi(match_results[4].str());
        }

        const std::string unit = match_results[5].str();
        if (unit == "KT") {
            metar_.wind->unit = WindUnit::KNOTS;
        } else if (unit == "KMH") {
            metar_.wind->unit = WindUnit::KILOMETERS_PER_HOUR;
        } else if (unit == "MPS") {
            metar_.wind->unit = WindUnit::METERS_PER_SECOND;
        } else {
            metar_.wind->unit = WindUnit::UNKNOWN;
        }
    }

    if (!ts.eof() &&
        std::regex_match(ts.peek().begin(), ts.peek().end(),
                         RegexUtils::make_token_regex(Patterns::VARIABILITY))) {
        const std::string var_token = ts.consume();
        if (std::regex_match(
                var_token, match_results,
                RegexUtils::make_token_regex(Patterns::VARIABILITY))) {
            metar_.wind->variability =
                Variability{std::stoi(match_results[1].str()),
                            std::stoi(match_results[2].str())};
        }
    }
}

void METARParser::parse_visibility(TokenStream& ts) {
    if (ts.eof()) {
        return;
    }

    const std::string_view s = ts.peek();

    if (s == "CAVOK") {
        ts.consume();
        metar_.is_cavok = true;
        metar_.visibility->value = 10.0;
        metar_.visibility->unit = VisibilityUnit::STATUTE_MILES;
        metar_.visibility->greater_or_equal = true;
        return;
    }

    if (std::regex_match(s.begin(), s.end(),
                         RegexUtils::make_token_regex(Patterns::VIS_METERS))) {
        const std::string vis_token = ts.consume();
        std::smatch match_results;
        if (std::regex_match(
                vis_token, match_results,
                RegexUtils::make_token_regex(Patterns::VIS_METERS))) {
            int meters = std::stoi(match_results[1].str());
            if (meters == 9999) {
                metar_.visibility->value = 10.0;
                metar_.visibility->unit = VisibilityUnit::KILOMETERS;
                metar_.visibility->greater_or_equal = true;
            } else {
                metar_.visibility->value = meters;
                metar_.visibility->unit = VisibilityUnit::METERS;
            }
        }
        return;
    }

    if (std::regex_match(
            s.begin(), s.end(),
            RegexUtils::make_token_regex(Patterns::VIS_WHOLE_SM)) ||
        std::regex_match(
            s.begin(), s.end(),
            RegexUtils::make_token_regex(Patterns::VIS_FRACT_SM))) {
        const std::string vis_token = ts.consume();
        std::smatch match_results;
        if (std::regex_match(
                vis_token, match_results,
                RegexUtils::make_token_regex(Patterns::VIS_WHOLE_SM))) {
            metar_.visibility->value = std::stod(match_results[1].str());
            metar_.visibility->unit = VisibilityUnit::STATUTE_MILES;
        } else if (std::regex_match(
                       vis_token, match_results,
                       RegexUtils::make_token_regex(Patterns::VIS_FRACT_SM))) {
            const bool less_than = match_results[1].matched;
            const double numerator = std::stod(match_results[2].str());
            const double denominator = std::stod(match_results[3].str());
            metar_.visibility->value = numerator / denominator;
            metar_.visibility->less_than = less_than;
            metar_.visibility->unit = VisibilityUnit::STATUTE_MILES;
        }
        return;
    }

    if (std::regex_match(s.begin(), s.end(),
                         RegexUtils::make_token_regex(Patterns::INTEGER)) &&
        std::regex_match(
            ts.peek(1).begin(), ts.peek(1).end(),
            RegexUtils::make_token_regex(Patterns::VIS_FRACT_SM))) {
        const double whole = std::stod(std::string(ts.consume()));
        const std::string fraction_token = ts.consume();
        std::smatch match_results;
        if (std::regex_match(
                fraction_token, match_results,
                RegexUtils::make_token_regex(Patterns::VIS_FRACT_SM))) {
            const bool less_than = match_results[1].matched;
            const double numerator = std::stod(match_results[2].str());
            const double denominator = std::stod(match_results[3].str());
            metar_.visibility->value = whole + (numerator / denominator);
            metar_.visibility->less_than = less_than;
            metar_.visibility->unit = VisibilityUnit::STATUTE_MILES;
        }
    }
}

void METARParser::parse_rvr_block(TokenStream& ts) {
    while (!ts.eof() &&
           std::regex_match(ts.peek().begin(), ts.peek().end(),
                            RegexUtils::make_token_regex(Patterns::RVR))) {
        if (!metar_.rvr.empty()) {
            metar_.rvr += " ";
        }
        metar_.rvr += ts.consume();
    }
}

void METARParser::parse_weather_block(TokenStream& ts) {
    while (!ts.eof()) {
        const std::string_view next = ts.peek();
        if (!std::regex_match(
                next.begin(), next.end(),
                RegexUtils::make_exact_regex(Patterns::WEATHER))) {
            break;
        }

        if (!metar_.weather.empty()) {
            metar_.weather += " ";
        }
        metar_.weather += ts.consume();
    }
}

void METARParser::parse_sky_block(TokenStream& ts) {
    if (metar_.is_cavok) {
        metar_.is_skc = true;
        return;
    }

    bool ceiling_set = false;
    while (!ts.eof() &&
           std::regex_match(ts.peek().begin(), ts.peek().end(),
                            RegexUtils::make_token_regex(Patterns::SKY))) {
        const std::string sky_token = ts.consume();
        std::smatch match_results;
        if (!std::regex_match(sky_token, match_results,
                              RegexUtils::make_token_regex(Patterns::SKY))) {
            continue;
        }

        const std::string amount = match_results[1].str();
        if (amount == "CLR") {
            metar_.is_clr = true;
            continue;
        }
        if (amount == "SKC") {
            metar_.is_skc = true;
            continue;
        }
        if (amount == "NSC") {
            metar_.is_nsc = true;
            continue;
        }
        if (amount == "NCD") {
            metar_.is_ncd = true;
            continue;
        }

        if (!match_results[2].matched) {
            continue;
        }

        const int altitude = std::stoi(match_results[2].str()) * 100;
        bool is_ceiling = false;
        if ((amount == "BKN" || amount == "OVC") && !ceiling_set) {
            is_ceiling = true;
            ceiling_set = true;
        }

        metar_.cloud_coverage.push_back(
            CloudCoverage{altitude, amount, is_ceiling});
    }
}

void METARParser::parse_temp_dwpt(TokenStream& ts) {
    if (ts.eof() ||
        !std::regex_match(ts.peek().begin(), ts.peek().end(),
                          RegexUtils::make_token_regex(Patterns::TEMP_DWPT))) {
        return;
    }

    const std::string temp_token = ts.consume();
    std::smatch match_results;
    if (!std::regex_match(temp_token, match_results,
                          RegexUtils::make_token_regex(Patterns::TEMP_DWPT))) {
        return;
    }

    if (match_results[2].str() != "//") {
        const int value = std::stoi(match_results[2].str());
        metar_.temperature = match_results[1].matched ? -value : value;
    }

    if (match_results[4].str() != "//") {
        const int value = std::stoi(match_results[4].str());
        metar_.dewpoint = match_results[3].matched ? -value : value;
    }
}

void METARParser::parse_pressure(TokenStream& ts) {
    if (ts.eof() ||
        !std::regex_match(ts.peek().begin(), ts.peek().end(),
                          RegexUtils::make_token_regex(Patterns::PRESSURE))) {
        return;
    }

    const std::string pressure_token = ts.consume();
    std::smatch match_results;
    if (!std::regex_match(pressure_token, match_results,
                          RegexUtils::make_token_regex(Patterns::PRESSURE))) {
        return;
    }

    const std::string unit = match_results[1].str();
    const int value = std::stoi(match_results[2].str());
    if (unit == "A") {
        metar_.pressure =
            Pressure{static_cast<double>(value) / 100.0, PressureUnit::INHG};
    } else {
        metar_.pressure =
            Pressure{static_cast<double>(value), PressureUnit::HPA};
    }
}



std::string METARParser::to_string() const {
    std::ostringstream oss;

    oss << "Report for " << metar_.station_id << ":\n";

    if (metar_.report_time.has_value()) {
        oss << "\tAt " << std::setfill('0') << std::setw(2)
            << metar_.report_time->day << " " << std::setfill('0')
            << std::setw(2) << metar_.report_time->hour << ":"
            << std::setfill('0') << std::setw(2) << metar_.report_time->minute
            << "Z\n";
    } else {
        oss << "\tAt unknown time\n";
    }

    oss << "\tWind ";
    if (metar_.wind.has_value()) {
        if (metar_.wind->direction == 0 && metar_.wind->speed == 0 &&
            !metar_.wind->is_variable) {
            oss << "calm\n";
        } else if (metar_.wind->is_variable) {
            oss << "variable at " << metar_.wind->speed;
            if (metar_.wind->gust.has_value()) {
                oss << " gusting " << metar_.wind->gust.value();
            }
            oss << " " << metar_.wind->unit << "\n";
        } else {
            oss << "from " << metar_.wind->direction << " at "
                << metar_.wind->speed;
            if (metar_.wind->gust.has_value()) {
                oss << " gusting " << metar_.wind->gust.value();
            }
            oss << " " << metar_.wind->unit << "\n";

            if (metar_.wind->variability.has_value()) {
                oss << "\tVariable from "
                    << metar_.wind->variability->from_value << " to "
                    << metar_.wind->variability->to_value << "\n";
            }
        }
    } else {
        oss << "not reported\n";
    }

    oss << "\tVisibility ";
    if (!metar_.visibility.has_value() ||
        !metar_.visibility->value.has_value()) {
        oss << "not reported\n";
    } else {
        if (metar_.visibility->less_than) {
            oss << "less than ";
        } else if (metar_.visibility->greater_or_equal) {
            oss << "at least ";
        }
        oss << metar_.visibility->value.value() << " "
            << metar_.visibility->unit << "\n";
    }

    if (metar_.is_clr) {
        oss << "\tClear below 12,000 AGL\n";
    }

    if (metar_.is_skc) {
        oss << "\tSky clear\n";
    }

    if (metar_.is_nsc) {
        oss << "\tNo significant cloud\n";
    }

    if (metar_.is_ncd) {
        oss << "\tNo cloud detected\n";
    }

    if (!metar_.cloud_coverage.empty()) {
        oss << "\tCloud cover:\n";
        for (int i = static_cast<int>(metar_.cloud_coverage.size()) - 1; i >= 0;
             --i) {
            const CloudCoverage& layer = metar_.cloud_coverage[i];

            oss << "\t\t" << layer.coverage << " at " << layer.altitude
                << " AGL";
            if (layer.is_ceiling) {
                oss << " --ceiling--";
            }
            oss << "\n";
        }
    }

    if (metar_.temperature.has_value()) {
        oss << "\tTemperature " << metar_.temperature.value() << "C\n";
    } else {
        oss << "\tTemperature not reported\n";
    }

    if (metar_.dewpoint.has_value()) {
        oss << "\tDewpoint " << metar_.dewpoint.value() << "C\n";
    } else {
        oss << "\tDewpoint not reported\n";
    }

    if (metar_.pressure.has_value()) {
        oss << "\tPressure " << metar_.pressure->pressure << " "
            << metar_.pressure->unit << "\n";
    } else {
        oss << "\tPressure not reported\n";
    }

    return oss.str();
}
