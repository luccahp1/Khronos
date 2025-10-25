/* khronos/jd_to_gregorian.cpp */

#include <khronos/gregorian_calendar.hpp>

#include <cmath>

namespace khronos {

namespace {

void adjust_forward(year_t& year, month_t& month, day_t& day) {
    bool leap = is_gregorian_leapyear(year);
    day_t dim = gregorian_days_in_month(month, leap);
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
    bool leap = is_gregorian_leapyear(year);
    day = gregorian_days_in_month(month, leap);
}

}  // namespace

void jd_to_gregorian(jd_t jd, year_t& Y, month_t& M, day_t& D) {
    hour_t hour;
    minute_t minute;
    second_t second;
    jd_to_gregorian(jd, Y, M, D, hour, minute, second);
}

void jd_to_gregorian(jd_t jd, year_t& Y, month_t& M, day_t& D, hour_t& h, minute_t& mi, second_t& s) {
    long double Z = std::floor(jd + 0.5L);
    long double F = (jd + 0.5L) - Z;

    long double A = Z;
    if (Z >= 2299161.0L) {
        const long double alpha = std::floor((Z - 1867216.25L) / 36524.25L);
        A += 1 + alpha - std::floor(alpha / 4.0L);
    }

    const long double B = A + 1524.0L;
    const long double C = std::floor((B - 122.1L) / 365.25L);
    const long double Dd = std::floor(365.25L * C);
    const long double E = std::floor((B - Dd) / 30.6001L);

    long double day = B - Dd - std::floor(30.6001L * E) + F;
    int month = (E < 14.0L) ? static_cast<int>(E - 1.0L) : static_cast<int>(E - 13.0L);
    long long year = (month > 2) ? static_cast<long long>(C - 4716.0L) : static_cast<long long>(C - 4715.0L);

    day_t dayInt = static_cast<day_t>(std::floor(day));
    long double fraction = day - std::floor(day);

    hour_t hour = 0;
    minute_t minute = 0;
    second_t second = 0;
    int carry = jd_to_hms(fraction, hour, minute, second);

    year_t yearOut = static_cast<year_t>(year);
    month_t monthOut = static_cast<month_t>(month);

    if (carry == 1) {
        ++dayInt;
        adjust_forward(yearOut, monthOut, dayInt);
        hour = 0;
        minute = 0;
        second = 0;
    } else if (carry == -1) {
        --dayInt;
        adjust_backward(yearOut, monthOut, dayInt);
        hour = 23;
        minute = 59;
        second = 59;
    }

    Y = yearOut;
    M = monthOut;
    D = dayInt;
    h = hour;
    mi = minute;
    s = second;
}

}  // namespace khronos

