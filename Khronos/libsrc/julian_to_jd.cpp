/* khronos/julian_to_jd.cpp */

#include <khronos/julian_calendar.hpp>

#include <cmath>

namespace khronos {

namespace {

void adjust_forward(year_t& year, month_t& month, day_t& day) {
    const day_t dim = julian_days_in_month(month, is_julian_leapyear(year));
    if (day > dim) {
        day = 1;
        if (month == 12) {
            month = 1;
            ++year;
        } else {
            ++month;
        }
    }
}

void adjust_backward(year_t& year, month_t& month, day_t& day) {
    if (day >= 1) {
        return;
    }
    if (month == 1) {
        month = 12;
        --year;
    } else {
        --month;
    }
    day = julian_days_in_month(month, is_julian_leapyear(year));
}

}  // namespace

jd_t julian_to_jd(year_t year, month_t month, day_t day) {
    return julian_to_jd(year, month, day, hour_t(12), minute_t(0), second_t(0));
}

jd_t julian_to_jd(year_t year, month_t month, day_t day, hour_t hour, minute_t minute, second_t second) {
    long long y = year;
    int m = month;
    if (m <= 2) {
        --y;
        m += 12;
    }
    const long double base = std::floor(365.25L * (y + 4716))
            + std::floor(30.6001L * (m + 1))
            + static_cast<long double>(day) - 1524.5L;
    return jd_t(base + static_cast<long double>(hms_to_days(hour, minute, second)));
}

void jd_to_julian(jd_t jd, year_t& year, month_t& month, day_t& day) {
    hour_t hour;
    minute_t minute;
    second_t second;
    jd_to_julian(jd, year, month, day, hour, minute, second);
}

void jd_to_julian(jd_t jd, year_t& year, month_t& month, day_t& day, hour_t& hour, minute_t& minute, second_t& second) {
    const long double Z = std::floor(jd + 0.5L);
    const long double F = (jd + 0.5L) - Z;

    const long double A = Z;
    const long double B = A + 1524.0L;
    const long double C = std::floor((B - 122.1L) / 365.25L);
    const long double D = std::floor(365.25L * C);
    const long double E = std::floor((B - D) / 30.6001L);

    long double dayValue = B - D - std::floor(30.6001L * E) + F;
    int monthValue = (E < 14.0L) ? static_cast<int>(E - 1.0L) : static_cast<int>(E - 13.0L);
    long long yearValue = (monthValue > 2) ? static_cast<long long>(C - 4716.0L) : static_cast<long long>(C - 4715.0L);

    day_t dayInt = static_cast<day_t>(std::floor(dayValue));
    const long double fraction = dayValue - std::floor(dayValue);

    hour_t h = 0;
    minute_t mi = 0;
    second_t s = 0;
    const int carry = jd_to_hms(fraction, h, mi, s);

    year = static_cast<year_t>(yearValue);
    month = static_cast<month_t>(monthValue);
    day = dayInt;

    if (carry == 1) {
        ++day;
        adjust_forward(year, month, day);
        h = 0;
        mi = 0;
        s = 0;
    } else if (carry == -1) {
        --day;
        adjust_backward(year, month, day);
        h = 23;
        mi = 59;
        s = 59;
    }

    hour = h;
    minute = mi;
    second = s;
}

}  // namespace khronos

