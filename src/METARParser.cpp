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
    parse_report_modifier(tokens);
    parse_wind(tokens);
    parse_visibility(tokens);
    parse_rvr_block(tokens);
    parse_weather_block(tokens);
    parse_sky_block(tokens);
    parse_temp_dwpt(tokens);
    parse_pressure(tokens);
    parse_remark(tokens);
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

        ReportTime report_time{hour, minute};
        metar_.report_date_time = ReportDateTime{day, report_time};
    }
}

void METARParser::parse_report_modifier(TokenStream& ts) {
    if (ts.eof() || !std::regex_match(ts.peek().begin(), ts.peek().end(),
                                      RegexUtils::make_exact_regex(
                                          Patterns::REPORT_MODIFIER))) {
        return;
    }

    const std::string modifier_token = ts.consume();
    if (modifier_token == "AUTO") {
        metar_.report_modifier = ReportModifier::AUTO;
    } else if (modifier_token == "COR") {
        metar_.report_modifier = ReportModifier::COR;
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
    while (!ts.eof()) {
        const std::string_view next = ts.peek();

        if (!std::regex_match(next.begin(), next.end(),
                              RegexUtils::make_token_regex(Patterns::RVR))) {
            break;
        }

        const std::string rvr_token = ts.consume();
        std::smatch match_results;
        if (!std::regex_match(rvr_token, match_results,
                              RegexUtils::make_token_regex(Patterns::RVR))) {
            continue;
        }

        std::string runway = match_results[2].str();
        std::string side = match_results[3].str();
        if (!side.empty()) {
            runway += side;
        }

        std::optional<BoundaryStatus> boundary_status = std::nullopt;
        std::string boundary_status_str = match_results[4].str();
        if (boundary_status_str == "P") {
            boundary_status = BoundaryStatus::PLUS;
        } else if (boundary_status_str == "M") {
            boundary_status = BoundaryStatus::MINUS;
        }

        std::optional<int> constant_distance = std::nullopt;
        if (match_results[5].matched) {
            constant_distance = std::stoi(match_results[5].str());
        }

        std::optional<int> variable_min_distance = std::nullopt;
        std::optional<int> variable_max_distance = std::nullopt;
        if (match_results[6].matched && match_results[7].matched) {
            variable_min_distance = std::stoi(match_results[6].str());
            variable_max_distance = std::stoi(match_results[7].str());
        }

        // ICAO METAR RVR defaults to meters when no explicit unit suffix is
        // present.
        std::optional<VisibilityUnit> unit = VisibilityUnit::METERS;
        std::string unit_str = match_results[8].str();
        if (unit_str == "FT") {
            unit = VisibilityUnit::FEET;
        }

        std::optional<VisibilityTendency> tendency = std::nullopt;
        std::string tendency_str = match_results[9].str();
        if (tendency_str == "U") {
            tendency = VisibilityTendency::INCREASING;
        } else if (tendency_str == "D") {
            tendency = VisibilityTendency::DECREASING;
        } else if (tendency_str == "N") {
            tendency = VisibilityTendency::STEADY;
        }

        metar_.rvr.push_back(RVR{runway, boundary_status, constant_distance,
                                 variable_min_distance, variable_max_distance,
                                 unit, tendency});
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

        const std::string weather_token = ts.consume();
        std::smatch match_results;
        if (!std::regex_match(
                weather_token, match_results,
                RegexUtils::make_exact_regex(Patterns::WEATHER))) {
            continue;
        }

        std::optional<Intensity> intensity = std::nullopt;
        const std::string intensity_str = match_results[1].str();
        if (intensity_str == "-") {
            intensity = Intensity::LIGHT;
        } else if (intensity_str == "+") {
            intensity = Intensity::HEAVY;
        } else if (intensity_str == "VC") {
            intensity = Intensity::VICINITY;
        }

        std::optional<Descriptor> descriptor = std::nullopt;
        const std::string descriptor_str = match_results[2].str();
        if (descriptor_str == "MI") {
            descriptor = Descriptor::SHALLOW;
        } else if (descriptor_str == "PR") {
            descriptor = Descriptor::PARTIAL;
        } else if (descriptor_str == "BC") {
            descriptor = Descriptor::PATCHES;
        } else if (descriptor_str == "DR") {
            descriptor = Descriptor::LOW_DRIFTING;
        } else if (descriptor_str == "BL") {
            descriptor = Descriptor::BLOWING;
        } else if (descriptor_str == "SH") {
            descriptor = Descriptor::SHOWERS;
        } else if (descriptor_str == "TS") {
            descriptor = Descriptor::THUNDERSTORM;
        } else if (descriptor_str == "FZ") {
            descriptor = Descriptor::FREEZING;
        }

        std::optional<Precipitation> precipitation = std::nullopt;
        const std::string precip_str = match_results[3].str();
        if (precip_str == "DZ") {
            precipitation = Precipitation::DRIZZLE;
        } else if (precip_str == "RA") {
            precipitation = Precipitation::RAIN;
        } else if (precip_str == "SN") {
            precipitation = Precipitation::SNOW;
        } else if (precip_str == "SG") {
            precipitation = Precipitation::SNOW_GRAINS;
        } else if (precip_str == "IC") {
            precipitation = Precipitation::ICE_CRYSTALS;
        } else if (precip_str == "PL") {
            precipitation = Precipitation::ICE_PELLETS;
        } else if (precip_str == "GR") {
            precipitation = Precipitation::HAIL;
        } else if (precip_str == "GS") {
            precipitation = Precipitation::SMALL_HAIL;
        } else if (precip_str == "UP") {
            precipitation = Precipitation::UNKNOWN_PRECIP;
        }

        std::optional<Obscuration> obscuration = std::nullopt;
        const std::string obscuration_str = match_results[3].str();
        if (obscuration_str == "BR") {
            obscuration = Obscuration::MIST;
        } else if (obscuration_str == "FG") {
            obscuration = Obscuration::FOG;
        } else if (obscuration_str == "FU") {
            obscuration = Obscuration::SMOKE;
        } else if (obscuration_str == "VA") {
            obscuration = Obscuration::VOLCANIC_ASH;
        } else if (obscuration_str == "DU") {
            obscuration = Obscuration::WIDESPREAD_DUST;
        } else if (obscuration_str == "SA") {
            obscuration = Obscuration::SAND;
        } else if (obscuration_str == "HZ") {
            obscuration = Obscuration::HAZE;
        } else if (obscuration_str == "PY") {
            obscuration = Obscuration::SPRAY;
        }

        std::optional<OtherPhenomenon> other_phenomenon = std::nullopt;
        const std::string other_phenomenon_str = match_results[3].str();
        if (other_phenomenon_str == "PO") {
            other_phenomenon = OtherPhenomenon::DUST_WHIRLS;
        } else if (other_phenomenon_str == "SQ") {
            other_phenomenon = OtherPhenomenon::SQUALLS;
        } else if (other_phenomenon_str == "FC") {
            other_phenomenon = OtherPhenomenon::FUNNEL_CLOUD;
        } else if (other_phenomenon_str == "SS") {
            other_phenomenon = OtherPhenomenon::SANDSTORM;
        } else if (other_phenomenon_str == "DS") {
            other_phenomenon = OtherPhenomenon::DUSTSTORM;
        }

        metar_.weather.push_back(Weather{intensity, descriptor, precipitation,
                                         obscuration, other_phenomenon});
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

void METARParser::parse_remark(TokenStream& ts) {
    if (ts.eof() ||
        !std::regex_match(ts.peek().begin(), ts.peek().end(),
                          RegexUtils::make_token_regex(Patterns::REMARK))) {
        return;
    }

    ts.consume();
    metar_.has_remarks = true;

    while (!ts.eof()) {
        if (parse_station_type(ts)) {
            continue;
        }
        ts.consume();
    }
}

bool METARParser::parse_station_type(TokenStream& ts) {
    if (ts.eof() || !std::regex_match(
                        ts.peek().begin(), ts.peek().end(),
                        RegexUtils::make_token_regex(Patterns::STATION_TYPE))) {
        return false;
    }

    const std::string station_type_token = ts.consume();
    std::smatch match_results;
    if (!std::regex_match(
            station_type_token, match_results,
            RegexUtils::make_token_regex(Patterns::STATION_TYPE))) {
        return false;
    }

    const std::string type = match_results[0].str();
    if (type == "AO1") {
        metar_.station_type = StationType::AO1;
    } else if (type == "AO2") {
        metar_.station_type = StationType::AO2;
    }
    return true;
}

std::string METARParser::to_string() const {
    std::ostringstream oss;

    oss << metar_.type << " Report for " << metar_.station_id << ":\n";

    if (metar_.report_date_time.has_value()) {
        oss << "\t" << metar_.report_date_time.value() << "Z\n";
    } else {
        oss << "\tAt unknown time\n";
    }

    if (metar_.report_modifier.has_value()) {
        oss << "\t" << metar_.report_modifier.value() << "\n";
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

    if (!metar_.rvr.empty()) {
        oss << "\tRunway Visual Range:\n";
        for (const RVR& rvr : metar_.rvr) {
            oss << "\t\tRunway " << rvr.runway << ": ";
            if (rvr.boundary_status.has_value()) {
                oss << rvr.boundary_status.value() << " ";
            }
            if (rvr.constant_distance.has_value()) {
                oss << rvr.constant_distance.value();
                if (rvr.unit.has_value()) {
                    oss << " " << rvr.unit.value();
                }
            } else if (rvr.variable_min_distance.has_value() &&
                       rvr.variable_max_distance.has_value()) {
                oss << "Variable from " << rvr.variable_min_distance.value()
                    << " to " << rvr.variable_max_distance.value();
                if (rvr.unit.has_value()) {
                    oss << " " << rvr.unit.value();
                }
            } else {
                oss << "Distance not reported\n";
            }
            if (rvr.tendency.has_value()) {
                oss << ", " << rvr.tendency.value();
            }
            oss << "\n";
        }
    }

    if (!metar_.weather.empty()) {
        oss << "\tWeather:\n";
        for (const Weather& weather : metar_.weather) {
            oss << "\t\t";
            if (weather.intensity.has_value()) {
                oss << weather.intensity.value() << " ";
            }
            if (weather.descriptor.has_value()) {
                oss << weather.descriptor.value() << " ";
            }
            if (weather.precipitation.has_value()) {
                oss << weather.precipitation.value() << " ";
            }
            if (weather.obscuration.has_value()) {
                oss << weather.obscuration.value() << " ";
            }
            if (weather.other_phenomenon.has_value()) {
                oss << weather.other_phenomenon.value() << " ";
            }
            oss << "\n";
        }
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

            std::string preposition = " at ";
            std::string unit = " AGL";
            if (layer.coverage == "VV") {
                preposition = " ";
                unit = " feet";
            }
            oss << "\t\t" << layer.coverage << preposition << layer.altitude
                << unit;
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
        oss << "\tPressure ";
        if (metar_.pressure->unit == PressureUnit::INHG) {
            std::ostringstream pressure_stream;
            pressure_stream << std::fixed << std::setprecision(2)
                            << metar_.pressure->pressure;
            oss << pressure_stream.str();
        } else {
            oss << metar_.pressure->pressure;
        }
        oss << " " << metar_.pressure->unit << "\n";
    } else {
        oss << "\tPressure not reported\n";
    }

    if (metar_.has_remarks) {
        oss << "\n\tRemarks:\n";
    }

    if (metar_.station_type.has_value()) {
        oss << "\t\t" << metar_.station_type.value() << "\n";
    }

    return oss.str();
}
