#ifndef PATTERNS_H
#define PATTERNS_H

#include <string_view>

namespace Patterns {

// Station/Airport ID
// 1 = station token
inline constexpr std::string_view STATION = R"(([A-Z0-9]{4}))";

// Timestamp
// 1 = day, 2 = hour, 3 = minute
inline constexpr std::string_view TIMESTAMP = R"((\d{2})(\d{2})(\d{2})Z)";

// Report Modifier
// 1 = modifier token
inline constexpr std::string_view REPORT_MODIFIER = R"((AUTO|COR))";

// Wind
// 1 = direction
// 2 = speed
// 3 = full gust token (e.g. G21), optional
// 4 = gust digits, optional
// 5 = unit
inline constexpr std::string_view WIND =
    R"((VRB|\d{3})(\d{2,3})(G(\d{2,3}))?(KT|MPS|KMH))";

// Variability
// 1 = from direction
// 2 = to direction
inline constexpr std::string_view VARIABILITY = R"((\d{3})V(\d{3}))";

// CAVOK (Ceiling And Visibility OK)
inline constexpr std::string_view CAVOK = R"(CAVOK)";

// Whole-mile Visibility
// 1 = whole miles
inline constexpr std::string_view VIS_WHOLE_SM = R"((\d+)SM)";

// Fractional Visibility
// 1 = M if present
// 2 = numerator
// 3 = denominator
inline constexpr std::string_view VIS_FRACT_SM = R"((M)?(\d+)/(\d+)SM)";

// Integer Matching for Visibility
// 1 = integer
inline constexpr std::string_view INTEGER = R"((\d+))";

// Metric Visibility
// 1 = meters
inline constexpr std::string_view VIS_METERS = R"((\d{4}))";

// Visibility (combined pattern)
inline constexpr std::string_view VISIBILITY = R"((M)?(\d+(?:/\d+)?)SM)";

// Sky/Cloud Cover and Clear Skies
// Matches SKC, CLR, NSC, NCD
inline constexpr std::string_view CLEAR_SKIES = R"((SKC|CLR|NSC|NCD))";

inline constexpr std::string_view CLOUD_COVER =
    R"((FEW|SCT|BKN|OVC|VV)(\d{3})(CB|TCU)?)";

// RVR
// 1 = whole token
// 2 = runway number
// 3 = runway side (L/C/R), optional
// 4 = constant prefix (M/P), optional
// 5 = constant distance, optional
// 6 = variable minimum distance, optional
// 7 = variable maximum distance, optional
// 8 = units (FT), optional
// 9 = tendency (U/D/N), optional (optionally preceded by /)
inline constexpr std::string_view RVR =
    R"((R(\d{2})([LCR])?/(?:([MP])?(\d{4})|(\d{4})V(\d{4}))(FT)?(?:/)?([UDN])?))";

// Weather
// 1 = intensity/proximity (VC / - / +), optional
// 2 = descriptor, optional
// 3 = full concatenated phenomena sequence, optional for vicinity forms
// 4 = last individual phenomenon matched
inline constexpr std::string_view WEATHER =
    R"((VC|\-|\+)?(MI|PR|BC|DR|BL|SH|TS|FZ)?((DZ|RA|SN|SG|IC|PL|GR|GS|UP|BR|FG|FU|VA|DU|SA|HZ|PY|PO|SQ|FC|SS|DS)+)?)";

// 1 = amount/token
// 2 = height, optional
// 3 = convective suffix, optional
inline constexpr std::string_view SKY =
    R"((SKC|CLR|NSC|NCD|FEW|SCT|BKN|OVC|VV)(\d{3})?(CB|TCU)?)";

// Temperature / Dewpoint
// 1 = temp sign (M), optional
// 2 = temp value or //
// 3 = dew sign (M), optional
// 4 = dew value or //
inline constexpr std::string_view TEMP_DWPT =
    R"((M)?(\d{2}|//)/(M)?(\d{2}|//))";

// Pressure
// 1 = unit (A for inHg, Q for mb)
// 2 = value
inline constexpr std::string_view PRESSURE = R"((A|Q)(\d{4}))";

inline constexpr std::string_view PRESSURE_IN = R"(A(\d{4}))";

inline constexpr std::string_view PRESSURE_MB = R"(Q(\d{4}))";

} // namespace Patterns

#endif
