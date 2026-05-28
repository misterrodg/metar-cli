#include "Ordinals.h"

#include <cassert>
#include <cmath>

namespace Ordinals {

const std::string to_ordinal_date(int value) {
    assert(value >= 1 && value <= 31);
    value = std::abs(value);
    std::string string_value = std::to_string(value);
    int ones = value % 10;
    if ((value < 10 || value > 20) && ones < 4) {
        if (ones == 1) {
            return string_value += "st";
        } else if (ones == 2) {
            return string_value += "nd";
        } else if (ones == 3) {
            return string_value += "rd";
        }
    }
    return string_value += "th";
}

}; // namespace Ordinals
