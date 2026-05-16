#ifndef METAR_PARSER_H
#define METAR_PARSER_H

#include "METAR.h"
#include "TokenStream.h"

#include <regex>
#include <string>

class METARParser {
public:
    explicit METARParser(const std::string& metar);

    std::string to_string() const;

private:
    const std::string metar_string_;
    METAR metar_;

    static bool is_report_type(std::string_view s);
    static bool is_station_id(std::string_view s);
    static bool is_report_time(std::string_view s);
    static bool is_modifier(std::string_view s);
    static bool is_wind(std::string_view s);
    static bool is_variable_wind(std::string_view s);
    static bool is_sm_single_token(std::string_view s);
    static bool is_integer_token(std::string_view s);
    static bool is_fraction_sm_token(std::string_view s);
    static bool is_metric_visibility(std::string_view s);
    static bool is_rvr(std::string_view s);
    static bool is_weather(std::string_view s);
    static bool is_sky(std::string_view s);
    static bool is_temp_dwpt(std::string_view s);
    static bool is_pressure(std::string_view s);
    static bool is_remark(std::string_view s);

    void parse_type(TokenStream& ts);
    void parse_station_id(TokenStream& ts);
    void parse_report_time(TokenStream& ts);
    void parse_modifiers(TokenStream& ts);
    void parse_wind(TokenStream& ts);
    void parse_visibility(TokenStream& ts);
    void parse_rvr_block(TokenStream& ts);
    void parse_weather_block(TokenStream& ts);
    void parse_sky_block(TokenStream& ts);
    void parse_temp_dwpt(TokenStream& ts);
    void parse_pressure(TokenStream& ts);
    void parse_remark(TokenStream& ts);
};

#endif
