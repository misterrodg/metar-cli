#include "METARParser.h"
#include "Patterns.h"
#include "RegexUtils.h"
#include "Utils.h"

#include <iomanip>
#include <regex>
#include <sstream>
#include <string>

METARParser::METARParser(const std::string& metar)
    : metar_string_(metar), station_id_(""), report_time_(std::nullopt),
      wind_direction_(0), wind_speed_(0), wind_is_variable_(false),
      wind_gust_(std::nullopt), variability_(std::nullopt),
      visibility_(std::nullopt), is_clr_(false), is_skc_(false),
      cloud_coverage_(), temperature_(std::nullopt), dewpoint_(std::nullopt),
      pressure_(std::nullopt) {
    process_station();
    process_report_time();
    process_wind();
    process_variability();
    process_visibility();
    process_cloud_cover();
    process_temp_dewpt();
    process_pressure();
}

std::string METARParser::to_string() const {
    std::ostringstream oss;

    oss << "Report for " << station_id_ << ":\n";

    if (report_time_.has_value()) {
        oss << "\tAt " << std::setfill('0') << std::setw(2) << report_time_->day
            << " " << std::setfill('0') << std::setw(2) << report_time_->hour
            << ":" << std::setfill('0') << std::setw(2) << report_time_->minute
            << "Z\n";
    } else {
        oss << "\tAt unknown time\n";
    }

    oss << "\tWind ";
    if (wind_direction_ == 0 && wind_speed_ == 0 && !wind_is_variable_) {
        oss << "calm\n";
    } else if (wind_is_variable_) {
        oss << "variable at " << wind_speed_;
        if (wind_gust_.has_value()) {
            oss << " gusting " << wind_gust_.value();
        }
        oss << " knots\n";
    } else {
        oss << "from " << wind_direction_ << " at " << wind_speed_;
        if (wind_gust_.has_value()) {
            oss << " gusting " << wind_gust_.value();
        }
        oss << " knots\n";

        if (variability_.has_value()) {
            oss << "\tVariable from " << variability_->from_value << " to "
                << variability_->to_value << "\n";
        }
    }

    oss << "\tVisibility ";
    if (!visibility_.has_value()) {
        oss << "not reported\n";
    } else {
        if (visibility_->less_than) {
            oss << "less than ";
        } else if (visibility_->greater_or_equal) {
            oss << "at least ";
        }
        oss << visibility_->distance_sm << " SM\n";
    }

    if (is_clr_) {
        oss << "\tClear below 12,000 AGL\n";
    }

    if (is_skc_) {
        oss << "\tSky clear\n";
    }

    if (!cloud_coverage_.empty()) {
        oss << "\tCloud cover:\n";
        for (int i = static_cast<int>(cloud_coverage_.size()) - 1; i >= 0;
             --i) {
            const CloudCoverage& layer = cloud_coverage_[i];

            oss << "\t\t" << layer.coverage << " at " << layer.altitude
                << " AGL";
            if (layer.is_ceiling) {
                oss << " --ceiling--";
            }
            oss << "\n";
        }
    }

    if (temperature_.has_value()) {
        oss << "\tTemperature " << temperature_.value() << "C\n";
    } else {
        oss << "\tTemperature not reported\n";
    }

    if (dewpoint_.has_value()) {
        oss << "\tDewpoint " << dewpoint_.value() << "C\n";
    } else {
        oss << "\tDewpoint not reported\n";
    }

    if (pressure_.has_value()) {
        oss << "\tPressure " << pressure_->pressure << " " << pressure_->unit
            << "\n";
    } else {
        oss << "\tPressure not reported\n";
    }

    return oss.str();
}

void METARParser::process_station() {
    std::smatch match_results;
    if (std::regex_search(metar_string_, match_results, station_regex_)) {
        station_id_ = match_results[0].str();
    }
}

void METARParser::process_report_time() {
    std::smatch match_results;
    if (std::regex_search(metar_string_, match_results, timestamp_regex_)) {
        int day = std::stoi(match_results[1].str());
        int hour = std::stoi(match_results[2].str());
        int minute = std::stoi(match_results[3].str());

        report_time_ = ReportTime{day, hour, minute};
    }
}

void METARParser::process_wind() {
    std::smatch match_results;
    if (std::regex_search(metar_string_, match_results, wind_regex_)) {
        std::string direction = match_results[1].str();

        wind_is_variable_ = (direction == "VRB");
        if (!wind_is_variable_) {
            wind_direction_ = std::stoi(direction);
        }

        wind_speed_ = std::stoi(match_results[2].str());

        if (match_results[3].matched) {
            wind_gust_ = std::stoi(match_results[3].str());
        }
    }
}

void METARParser::process_variability() {
    std::smatch match_results;
    if (std::regex_search(metar_string_, match_results, variability_regex_)) {
        variability_ = Variability{std::stoi(match_results[1].str()),
                                   std::stoi(match_results[2].str())};
    }
}

void METARParser::process_visibility() {
    std::smatch match_results;
    if (std::regex_search(metar_string_, match_results, visibility_regex_)) {
        std::optional<double> parsed =
            parse_fractional_number(match_results[2].str());

        if (parsed.has_value()) {
            visibility_ =
                Visibility{parsed.value(), match_results[1].matched, false};
        }
        return;
    }

    if (std::regex_search(metar_string_, match_results,
                          visibility_meters_regex_)) {
        int meters = std::stoi(match_results[0].str());

        if (meters == 9999) {
            visibility_ = Visibility{6.0, false, true};
            return;
        }

        double m_to_sm = 1609.344;
        double sm = static_cast<double>(meters) / m_to_sm;
        visibility_ = Visibility{sm, false, false};
    }
}

void METARParser::process_cloud_cover() {
    std::smatch match_results;
    if (std::regex_search(metar_string_, match_results, clear_skies_regex_)) {
        if (match_results[1].str() == "CLR") {
            is_clr_ = true;
            return;
        }
        if (match_results[1].str() == "SKC") {
            is_skc_ = true;
            return;
        }
    }

    std::sregex_iterator next(metar_string_.begin(), metar_string_.end(),
                              cloud_cover_regex_);
    std::sregex_iterator end;

    bool ceiling_set = false;
    while (next != end) {
        std::smatch match = *next;

        std::string cover = match[1].str();
        bool is_ceiling = false;

        if ((cover == "BKN" || cover == "OVC") && !ceiling_set) {
            is_ceiling = true;
            ceiling_set = true;
        }

        cloud_coverage_.push_back(
            CloudCoverage{std::stoi(match[2].str()) * 100, cover, is_ceiling});

        ++next;
    }
}

void METARParser::process_temp_dewpt() {
    std::smatch match_results;
    if (std::regex_search(metar_string_, match_results, temp_dewpt_regex_)) {
        bool is_negative_temp = match_results[1].matched;
        bool is_negative_dwpt = match_results[3].matched;

        int temp = std::stoi(match_results[2].str());
        int dwpt = std::stoi(match_results[4].str());

        temperature_ = (is_negative_temp ? -temp : temp);
        dewpoint_ = (is_negative_dwpt ? -dwpt : dwpt);
    }
}

void METARParser::process_pressure() {
    std::smatch match_results;
    if (std::regex_search(metar_string_, match_results, pressure_in_regex_)) {
        int pressure_raw = std::stoi(match_results[1].str());
        pressure_ = Pressure{static_cast<double>(pressure_raw) / 100.0, "inHg"};
        return;
    }
    if (std::regex_search(metar_string_, match_results, pressure_mb_regex_)) {
        int pressure_raw = std::stoi(match_results[1].str());
        pressure_ = Pressure{static_cast<double>(pressure_raw), "mb"};
    }
}

const std::regex METARParser::station_regex_(
    RegexUtils::make_token_regex(Patterns::STATION));

const std::regex METARParser::timestamp_regex_(
    RegexUtils::make_token_regex(Patterns::TIMESTAMP));

const std::regex
    METARParser::wind_regex_(RegexUtils::make_token_regex(Patterns::WIND));

const std::regex METARParser::variability_regex_(
    RegexUtils::make_token_regex(Patterns::VARIABILITY));

const std::regex METARParser::visibility_regex_(
    RegexUtils::make_token_regex(Patterns::VISIBILITY));

const std::regex METARParser::visibility_meters_regex_(
    RegexUtils::make_token_regex(Patterns::VISIBILITY_METERS));

const std::regex METARParser::clear_skies_regex_(
    RegexUtils::make_token_regex(Patterns::CLEAR_SKIES));

const std::regex METARParser::cloud_cover_regex_(
    RegexUtils::make_token_regex(Patterns::CLOUD_COVER));

const std::regex METARParser::temp_dewpt_regex_(
    RegexUtils::make_token_regex(Patterns::TEMP_DEWPT));

const std::regex METARParser::pressure_in_regex_(
    RegexUtils::make_token_regex(Patterns::PRESSURE_IN));

const std::regex METARParser::pressure_mb_regex_(
    RegexUtils::make_token_regex(Patterns::PRESSURE_MB));
