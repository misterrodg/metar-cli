#include "METAR.h"
#include "Ordinals.h"

#include <iomanip>

METAR::METAR()
    : type(""), station_id(""), report_date_time(std::nullopt),
      report_modifier(std::nullopt), wind(std::nullopt), is_cavok(false),
      visibility(std::nullopt), rvr(), weather(), is_clr(false), is_skc(false),
      is_nsc(false), is_ncd(false), cloud_coverage(), temperature(std::nullopt),
      dewpoint(std::nullopt), pressure(std::nullopt), has_remarks(false),
      station_type(std::nullopt) {
}

std::ostream& operator<<(std::ostream& os, Descriptor descriptor) {
    switch (descriptor) {
    case Descriptor::SHALLOW:
        return os << "Shallow";
    case Descriptor::PARTIAL:
        return os << "Partial";
    case Descriptor::PATCHES:
        return os << "Patches";
    case Descriptor::LOW_DRIFTING:
        return os << "Low drifting";
    case Descriptor::BLOWING:
        return os << "Blowing";
    case Descriptor::SHOWERS:
        return os << "Showers";
    case Descriptor::THUNDERSTORM:
        return os << "Thunderstorm";
    case Descriptor::FREEZING:
        return os << "Freezing";
    default:
        return os << "unknown descriptor";
    }
}

std::ostream& operator<<(std::ostream& os, ReportModifier modifier) {
    switch (modifier) {
    case ReportModifier::AUTO:
        return os << "Automated";
    case ReportModifier::COR:
        return os << "Corrected";
    default:
        return os << "Unknown";
    }
}

std::ostream& operator<<(std::ostream& os, BoundaryStatus status) {
    switch (status) {
    case BoundaryStatus::PLUS:
        return os << "more than";
    case BoundaryStatus::MINUS:
        return os << "less than";
    default:
        return os << "unknown";
    }
}

std::ostream& operator<<(std::ostream& os, VisibilityTendency tendency) {
    switch (tendency) {
    case VisibilityTendency::INCREASING:
        return os << "increasing";
    case VisibilityTendency::STEADY:
        return os << "steady";
    case VisibilityTendency::DECREASING:
        return os << "decreasing";
    default:
        return os << "unknown";
    }
}

std::ostream& operator<<(std::ostream& os, Intensity intensity) {
    switch (intensity) {
    case Intensity::LIGHT:
        return os << "Light";
    case Intensity::HEAVY:
        return os << "Heavy";
    case Intensity::VICINITY:
        return os << "In the vicinity: ";
    default:
        return os << "unknown intensity";
    }
}

std::ostream& operator<<(std::ostream& os, Precipitation precipitation) {
    switch (precipitation) {
    case Precipitation::DRIZZLE:
        return os << "Drizzle";
    case Precipitation::RAIN:
        return os << "Rain";
    case Precipitation::SNOW:
        return os << "Snow";
    case Precipitation::SNOW_GRAINS:
        return os << "Snow Grains";
    case Precipitation::ICE_CRYSTALS:
        return os << "Ice Crystals";
    case Precipitation::ICE_PELLETS:
        return os << "Ice Pellets";
    case Precipitation::HAIL:
        return os << "Hail";
    case Precipitation::SMALL_HAIL:
        return os << "Small Hail";
    default:
        return os << "unknown precipitation type";
    }
}

std::ostream& operator<<(std::ostream& os, Obscuration obscuration) {
    switch (obscuration) {
    case Obscuration::MIST:
        return os << "Mist";
    case Obscuration::FOG:
        return os << "Fog";
    case Obscuration::SMOKE:
        return os << "Smoke";
    case Obscuration::VOLCANIC_ASH:
        return os << "Volcanic Ash";
    case Obscuration::WIDESPREAD_DUST:
        return os << "Widespread Dust";
    case Obscuration::SAND:
        return os << "Sand";
    case Obscuration::HAZE:
        return os << "Haze";
    case Obscuration::SPRAY:
        return os << "Spray";
    default:
        return os << "unknown obscuration type";
    }
}

std::ostream& operator<<(std::ostream& os, OtherPhenomenon phenomenon) {
    switch (phenomenon) {
    case OtherPhenomenon::DUST_WHIRLS:
        return os << "Dust Whirls";
    case OtherPhenomenon::SQUALLS:
        return os << "Squalls";
    case OtherPhenomenon::FUNNEL_CLOUD:
        return os << "Funnel Cloud";
    case OtherPhenomenon::SANDSTORM:
        return os << "Sandstorm";
    case OtherPhenomenon::DUSTSTORM:
        return os << "Duststorm";
    default:
        return os << "unknown other phenomenon";
    }
}

std::ostream& operator<<(std::ostream& os, StationType type) {
    switch (type) {
    case StationType::AO1:
        return os << "Automated station";
    case StationType::AO2:
        return os << "Automated station with precipitation discriminator";
    default:
        return os << "unknown type";
    }
}

std::ostream& operator<<(std::ostream& os, ReportTime report_time) {
    if (report_time.hour == std::nullopt) {
        return os << std::setfill('0') << std::setw(2) << report_time.minute
                  << " minutes past the hour";
    }
    return os << std::setfill('0') << std::setw(2) << *report_time.hour << ":"
              << std::setfill('0') << std::setw(2) << report_time.minute;
}

std::ostream& operator<<(std::ostream& os, ReportDateTime report_date_time) {
    if (report_date_time.date == std::nullopt) {
        return os << report_date_time.time;
    }
    return os << "On the " << std::setfill('0') << std::setw(2)
              << Ordinals::to_ordinal_date(*report_date_time.date) << " "
              << report_date_time.time;
}
