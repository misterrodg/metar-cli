#ifndef PATTERNS_H
#define PATTERNS_H

#include <string_view>

namespace Patterns {

inline constexpr std::string_view STATION = R"([A-Z0-9]{4})";

inline constexpr std::string_view TIMESTAMP = R"((\d{2})(\d{2})(\d{2})Z)";
inline constexpr std::string_view WIND =
    R"((VRB|[0-9]{3})([0-9]{2,3})(?:G([0-9]{2,3}))?KT)";
inline constexpr std::string_view VARIABILITY = R"((\d{3})V(\d{3}))";
inline constexpr std::string_view CAVOK = R"(CAVOK)";
inline constexpr std::string_view VISIBILITY =
    R"((M)?((?:\d+\s+)?\d+/\d+|\d+)SM)";
inline constexpr std::string_view VISIBILITY_METERS = R"((\d{4}))";
inline constexpr std::string_view CLEAR_SKIES = R"((SKC|CLR|NSC|NCD))";
inline constexpr std::string_view CLOUD_COVER = R"((FEW|SCT|BKN|OVC)(\d{3}))";
inline constexpr std::string_view TEMP_DEWPT = R"((M)?(\d{2})/(M)?(\d{2}))";
inline constexpr std::string_view PRESSURE_IN = R"(A(\d{4}))";
inline constexpr std::string_view PRESSURE_MB = R"(Q(\d{4}))";

} // namespace Patterns

#endif
