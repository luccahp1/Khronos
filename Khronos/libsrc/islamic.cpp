/* khronos/islamic.cpp */

#include <khronos/islamic_calendar.hpp>

#include <khronos/calendar.hpp>

#include <cmath>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace khronos {

namespace {

struct IslamicFields {
    year_t year;
    month_t month;
    day_t day;
    hour_t hour;
    minute_t minute;
    second_t second;
};

IslamicFields split(jd_t jd) {
    IslamicFields out{};
    jd_to_islamic(jd, out.year, out.month, out.day, out.hour, out.minute, out.second);
    return out;
}

jd_t compose(year_t year, month_t month, day_t day, hour_t hour, minute_t minute, second_t second) {
    return islamic_to_jd(year, month, day, hour, minute, second);
}

day_t clamp_day(year_t year, month_t month, day_t day) {
    const day_t dim = islamic_days_in_month(year, month);
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

std::string two_digits(int value) {
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << value;
    return oss.str();
}

long long rounded_years(years delta) {
    const long long rounded = std::llround(delta.v);
    if (std::fabsl(delta.v - static_cast<long double>(rounded)) > 1e-9L) {
        throw std::invalid_argument("Islamic year adjustments must be integral");
    }
    return rounded;
}

}  // namespace

Islamic::Islamic() : jd_(local_now(WTIMEOFDAY)) {}

Islamic::Islamic(now_t mode) : jd_(local_now(mode)) {}

Islamic::Islamic(year_t year, month_t month, day_t day) : jd_(islamic_to_jd(year, month, day)) {}

Islamic::Islamic(year_t year, month_t month, day_t day, hour_t hour, minute_t minute, second_t second)
    : jd_(islamic_to_jd(year, month, day, hour, minute, second)) {}

Islamic::Islamic(Jd jd) : jd_(jd.jd()) {}

Islamic& Islamic::operator=(Jd jd) {
    jd_ = jd.jd();
    return *this;
}

year_t Islamic::year() const {
    return split(jd_).year;
}

month_t Islamic::month() const {
    return split(jd_).month;
}

day_t Islamic::day() const {
    return split(jd_).day;
}

hour_t Islamic::hour() const {
    return split(jd_).hour;
}

minute_t Islamic::minute() const {
    return split(jd_).minute;
}

second_t Islamic::second() const {
    return split(jd_).second;
}

Islamic::operator Jd() const {
    return Jd(jd_);
}

Islamic Islamic::operator+(months delta) const {
    IslamicFields parts = split(jd_);
    const long long totalMonths = static_cast<long long>(parts.month) - 1 + delta.v;
    year_t newYear = static_cast<year_t>(parts.year + totalMonths / 12);
    month_t newMonth = static_cast<month_t>((totalMonths % 12 + 12) % 12 + 1);
    const day_t newDay = clamp_day(newYear, newMonth, parts.day);
    const jd_t newJd = compose(newYear, newMonth, newDay, parts.hour, parts.minute, parts.second);
    return Islamic(Jd(newJd));
}

Islamic Islamic::operator+(years delta) const {
    IslamicFields parts = split(jd_);
    const long long wholeYears = rounded_years(delta);
    year_t newYear = static_cast<year_t>(parts.year + wholeYears);
    const day_t newDay = clamp_day(newYear, parts.month, parts.day);
    const jd_t newJd = compose(newYear, parts.month, newDay, parts.hour, parts.minute, parts.second);
    return Islamic(Jd(newJd));
}

std::string Islamic::to_string() const {
    const IslamicFields parts = split(jd_);
    const int weekday = day_of_week(jd_);
    static const char* const weekNames[7] = {
        "al-ithnayn", "ath-thulatha'", "al-arbi'a'", "al-khamis", "al-jum'a", "as-sabt", "al-ahad"};
    const std::string monthName = islamic_month_name(parts.month);
    const long long displayYear = static_cast<long long>(parts.year);
    int hour12 = parts.hour % 12;
    if (hour12 == 0) {
        hour12 = 12;
    }
    const char* const ampm = parts.hour < 12 ? "am" : "pm";
    const int roundedSeconds = static_cast<int>(std::floor(parts.second + 0.5));

    std::ostringstream oss;
    oss << weekNames[(weekday + 1) % 7] << ", " << monthName << ' ' << parts.day << ' ' << displayYear
        << ", " << hour12 << ':' << two_digits(parts.minute) << ':' << two_digits(roundedSeconds) << ' ' << ampm;
    return oss.str();
}

}  // namespace khronos

