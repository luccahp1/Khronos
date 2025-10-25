/* khronos/gregorian.cpp */

#include <khronos/gregorian_calendar.hpp>

#include <khronos/calendar.hpp>

#include <cmath>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace khronos {

namespace {

struct GregorianFields {
    year_t year;
    month_t month;
    day_t day;
    hour_t hour;
    minute_t minute;
    second_t second;
};

GregorianFields split(jd_t jd) {
    GregorianFields out{};
    jd_to_gregorian(jd, out.year, out.month, out.day, out.hour, out.minute, out.second);
    return out;
}

jd_t compose(year_t year, month_t month, day_t day, hour_t hour, minute_t minute, second_t second) {
    return gregorian_to_jd(year, month, day, hour, minute, second);
}

day_t clamp_day(year_t year, month_t month, day_t day) {
    const bool leap = is_gregorian_leapyear(year);
    const day_t dim = gregorian_days_in_month(month, leap);
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
        return gregorian_to_jd(year, month, day);
    }
    const hour_t hour = static_cast<hour_t>(tmLocal.tm_hour);
    const minute_t minute = static_cast<minute_t>(tmLocal.tm_min);
    const second_t second = static_cast<second_t>(tmLocal.tm_sec);
    return gregorian_to_jd(year, month, day, hour, minute, second);
}

long long rounded_years(years delta) {
    const long double rounded = std::llround(delta.v);
    if (std::fabsl(delta.v - rounded) > 1e-9L) {
        throw std::invalid_argument("Gregorian year adjustments must be integral");
    }
    return static_cast<long long>(rounded);
}

std::string two_digits(int value) {
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << value;
    return oss.str();
}

}  // namespace

Gregorian::Gregorian() : jd_(local_now(WTIMEOFDAY)) {}

Gregorian::Gregorian(now_t mode) : jd_(local_now(mode)) {}

Gregorian::Gregorian(year_t year, month_t month, day_t day)
    : jd_(gregorian_to_jd(year, month, day)) {}

Gregorian::Gregorian(year_t year, month_t month, day_t day, hour_t hour, minute_t minute, second_t second)
    : jd_(gregorian_to_jd(year, month, day, hour, minute, second)) {}

Gregorian::Gregorian(Jd jd) : jd_(jd.jd()) {}

year_t Gregorian::year() const {
    return split(jd_).year;
}

month_t Gregorian::month() const {
    return split(jd_).month;
}

day_t Gregorian::day() const {
    return split(jd_).day;
}

hour_t Gregorian::hour() const {
    return split(jd_).hour;
}

minute_t Gregorian::minute() const {
    return split(jd_).minute;
}

second_t Gregorian::second() const {
    return split(jd_).second;
}

Gregorian::operator Jd() const {
    return Jd(jd_);
}

Gregorian Gregorian::operator+(months delta) const {
    GregorianFields parts = split(jd_);
    const long long totalMonths = static_cast<long long>(parts.month) - 1 + delta.v;
    year_t newYear = static_cast<year_t>(parts.year + totalMonths / 12);
    month_t newMonth = static_cast<month_t>((totalMonths % 12 + 12) % 12 + 1);
    const day_t newDay = clamp_day(newYear, newMonth, parts.day);
    const jd_t newJd = compose(newYear, newMonth, newDay, parts.hour, parts.minute, parts.second);
    return Gregorian(Jd(newJd));
}

Gregorian Gregorian::operator+(years delta) const {
    GregorianFields parts = split(jd_);
    const long long wholeYears = rounded_years(delta);
    year_t newYear = static_cast<year_t>(parts.year + wholeYears);
    const day_t newDay = clamp_day(newYear, parts.month, parts.day);
    const jd_t newJd = compose(newYear, parts.month, newDay, parts.hour, parts.minute, parts.second);
    return Gregorian(Jd(newJd));
}

std::string Gregorian::to_string() const {
    const GregorianFields parts = split(jd_);
    const int weekday = day_of_week(jd_);
    static const char* const weekNames[7] = {
        "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
    const std::string monthName = gregorian_month_name(parts.month);
    const bool isBce = parts.year <= 0;
    const long long displayYear = isBce ? 1 - static_cast<long long>(parts.year) : static_cast<long long>(parts.year);
    int hour12 = parts.hour % 12;
    if (hour12 == 0) {
        hour12 = 12;
    }
    const char* const ampm = parts.hour < 12 ? "am" : "pm";
    const int roundedSeconds = static_cast<int>(std::floor(parts.second + 0.5));

    std::ostringstream oss;
    oss << weekNames[weekday] << ", " << monthName << ' ' << parts.day << ' ' << displayYear
        << (isBce ? " BCE, " : " CE, ") << hour12 << ':' << two_digits(parts.minute) << ':'
        << two_digits(roundedSeconds) << ' ' << ampm;
    return oss.str();
}

}  // namespace khronos

