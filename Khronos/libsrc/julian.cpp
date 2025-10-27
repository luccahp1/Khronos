/* khronos/julian.cpp */

#include <khronos/julian_calendar.hpp>

#include <khronos/calendar.hpp>

#include <cmath>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace khronos {

namespace {

struct JulianFields {
    year_t year;
    month_t month;
    day_t day;
    hour_t hour;
    minute_t minute;
    second_t second;
};

JulianFields split(jd_t jd) {
    JulianFields out{};
    jd_to_julian(jd, out.year, out.month, out.day, out.hour, out.minute, out.second);
    return out;
}

jd_t compose(year_t year, month_t month, day_t day, hour_t hour, minute_t minute, second_t second) {
    return julian_to_jd(year, month, day, hour, minute, second);
}

day_t clamp_day(year_t year, month_t month, day_t day) {
    const day_t dim = julian_days_in_month(month, is_julian_leapyear(year));
    if (day > dim) {
        return dim;
    }
    if (day < 1) {
        return 1;
    }
    return day;
}

jd_t local_now(now_t mode) {
    time_t raw = time(nullptr);
    tm tmLocal{};
#if defined(_WIN32)
    localtime_s(&tmLocal, &raw);
#else
    localtime_r(&raw, &tmLocal);
#endif
    const year_t year = static_cast<year_t>(tmLocal.tm_year + 1900);
    const month_t month = static_cast<month_t>(tmLocal.tm_mon + 1);
    const day_t day = static_cast<day_t>(tmLocal.tm_mday);
    if (mode == NOTIMEOFDAY) {
        return julian_to_jd(year, month, day);
    }
    const hour_t hour = static_cast<hour_t>(tmLocal.tm_hour);
    const minute_t minute = static_cast<minute_t>(tmLocal.tm_min);
    const second_t second = static_cast<second_t>(tmLocal.tm_sec);
    return julian_to_jd(year, month, day, hour, minute, second);
}

long long rounded_years(years delta) {
    const long long rounded = std::llround(delta.v);
    if (std::fabsl(delta.v - static_cast<long double>(rounded)) > 1e-9L) {
        throw std::invalid_argument("Julian year adjustments must be integral");
    }
    return rounded;
}

std::string two_digits(int value) {
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << value;
    return oss.str();
}

}  // namespace

Julian::Julian() : jd_(local_now(WTIMEOFDAY)) {}

Julian::Julian(now_t mode) : jd_(local_now(mode)) {}

Julian::Julian(year_t year, month_t month, day_t day) : jd_(julian_to_jd(year, month, day)) {}

Julian::Julian(year_t year, month_t month, day_t day, hour_t hour, minute_t minute, second_t second)
    : jd_(julian_to_jd(year, month, day, hour, minute, second)) {}

Julian::Julian(Jd jd) : jd_(jd.jd()) {}

Julian& Julian::operator=(Jd jd) {
    jd_ = jd.jd();
    return *this;
}

year_t Julian::year() const {
    return split(jd_).year;
}

month_t Julian::month() const {
    return split(jd_).month;
}

day_t Julian::day() const {
    return split(jd_).day;
}

hour_t Julian::hour() const {
    return split(jd_).hour;
}

minute_t Julian::minute() const {
    return split(jd_).minute;
}

second_t Julian::second() const {
    return split(jd_).second;
}

Julian::operator Jd() const {
    return Jd(jd_);
}

Julian Julian::operator+(months delta) const {
    JulianFields parts = split(jd_);
    const long long totalMonths = static_cast<long long>(parts.month) - 1 + delta.v;
    year_t newYear = static_cast<year_t>(parts.year + totalMonths / 12);
    month_t newMonth = static_cast<month_t>((totalMonths % 12 + 12) % 12 + 1);
    const day_t newDay = clamp_day(newYear, newMonth, parts.day);
    const jd_t newJd = compose(newYear, newMonth, newDay, parts.hour, parts.minute, parts.second);
    return Julian(Jd(newJd));
}

Julian Julian::operator+(years delta) const {
    JulianFields parts = split(jd_);
    const long long wholeYears = rounded_years(delta);
    year_t newYear = static_cast<year_t>(parts.year + wholeYears);
    const day_t newDay = clamp_day(newYear, parts.month, parts.day);
    const jd_t newJd = compose(newYear, parts.month, newDay, parts.hour, parts.minute, parts.second);
    return Julian(Jd(newJd));
}

std::string Julian::to_string() const {
    const JulianFields parts = split(jd_);
    const int weekday = day_of_week(jd_);
    static const char* const weekNames[7] = {
        "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
    const std::string monthName = civil_month_name(parts.month);
    const bool isBc = parts.year <= 0;
    const long long displayYear = isBc ? 1 - static_cast<long long>(parts.year) : static_cast<long long>(parts.year);
    int hour12 = parts.hour % 12;
    if (hour12 == 0) {
        hour12 = 12;
    }
    const char* const ampm = parts.hour < 12 ? "am" : "pm";
    const int roundedSeconds = static_cast<int>(std::floor(parts.second + 0.5));

    std::ostringstream oss;
    oss << weekNames[weekday] << ", " << monthName << ' ' << parts.day << ' ' << displayYear
        << (isBc ? " BC, " : " AD, ") << hour12 << ':' << two_digits(parts.minute) << ':'
        << two_digits(roundedSeconds) << ' ' << ampm;
    return oss.str();
}

}  // namespace khronos

