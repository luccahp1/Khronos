/* khronos/hebrew.cpp */

#include <khronos/hebrew_calendar.hpp>

#include <khronos/julian_day.hpp>

#include <cmath>
#include <iomanip>
#include <sstream>

namespace khronos {

namespace {

struct HebrewFields {
    year_t year{};
    month_t month{};
    day_t day{};
    hour_t hour{};
    minute_t minute{};
    second_t second{};
};

HebrewFields split(jd_t jd) {
    HebrewFields out{};
    jd_to_hebrew(jd, out.year, out.month, out.day, out.hour, out.minute, out.second);
    return out;
}

jd_t compose(year_t year, month_t month, day_t day, hour_t hour, minute_t minute, second_t second) {
    return hebrew_to_jd(year, month, day, hour, minute, second);
}

jd_t local_now(now_t mode) {
    return Jd(mode).jd();
}

std::string two_digits(int value) {
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << value;
    return oss.str();
}

}  // namespace

Hebrew::Hebrew() : jd_(local_now(WTIMEOFDAY)) {}

Hebrew::Hebrew(now_t mode) : jd_(local_now(mode)) {}

Hebrew::Hebrew(year_t year, month_t month, day_t day)
    : jd_(hebrew_to_jd(year, month, day)) {}

Hebrew::Hebrew(year_t year, month_t month, day_t day, hour_t hour, minute_t minute, second_t second)
    : jd_(compose(year, month, day, hour, minute, second)) {}

Hebrew::Hebrew(Jd jd) : jd_(jd.jd()) {}

Hebrew& Hebrew::operator=(Jd jd) {
    jd_ = jd.jd();
    return *this;
}

year_t Hebrew::year() const {
    return split(jd_).year;
}

month_t Hebrew::month() const {
    return split(jd_).month;
}

day_t Hebrew::day() const {
    return split(jd_).day;
}

hour_t Hebrew::hour() const {
    return split(jd_).hour;
}

minute_t Hebrew::minute() const {
    return split(jd_).minute;
}

second_t Hebrew::second() const {
    return split(jd_).second;
}

Hebrew::operator Jd() const {
    return Jd(jd_);
}

std::string Hebrew::to_string() const {
    const HebrewFields parts = split(jd_);

    int hour12 = parts.hour % 12;
    if (hour12 == 0) {
        hour12 = 12;
    }
    const char* const ampm = parts.hour < 12 ? "am" : "pm";
    const int displaySeconds = static_cast<int>(std::floor(parts.second + 0.5L));

    std::ostringstream oss;
    oss << hebrew_month_name(parts.month) << ' ' << parts.day << ' ' << parts.year
        << ", " << hour12 << ':' << two_digits(parts.minute) << ':' << two_digits(displaySeconds) << ' ' << ampm;
    return oss.str();
}

}  // namespace khronos

