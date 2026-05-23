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

    void parse_type(TokenStream& ts);
    void parse_station_id(TokenStream& ts);
    void parse_report_time(TokenStream& ts);
    void parse_report_modifier(TokenStream& ts);
    void parse_wind(TokenStream& ts);
    void parse_visibility(TokenStream& ts);
    void parse_rvr_block(TokenStream& ts);
    void parse_weather_block(TokenStream& ts);
    void parse_sky_block(TokenStream& ts);
    void parse_temp_dwpt(TokenStream& ts);
    void parse_pressure(TokenStream& ts);
    void parse_remark(TokenStream& ts);
    bool parse_station_type(TokenStream& ts);
};

#endif
