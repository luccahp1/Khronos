/* khronos/islamic_to_jd.cpp */

#include <khronos/islamic_calendar.hpp>

#include <cmath>

namespace khronos {

namespace {

constexpr long double ISLAMIC_EPOCH = 1948439.5L;

long double months_elapsed(month_t month) {
    return std::ceil(29.5L * (static_cast<long double>(month) - 1.0L));
}

}  // namespace

day_t islamic_days_in_month(year_t year, month_t month) {
    static const day_t base[12] = {30, 29, 30, 29, 30, 29, 30, 29, 30, 29, 30, 29};
    if (month < 1 || month > 12) {
        return 29;
    }
    day_t length = base[month - 1];
    if (month == 12 && is_islamic_leapyear(year)) {
        length = 30;
    }
    return length;
}

std::string islamic_month_name(month_t month) {
    static const char* const names[13] = {
        "", "Muharram", "Safar", "Rabi'al-Awwal", "Rabi'ath-Thani", "Jumada I-Ula",
        "Jumada t-Tania", "Rajab", "Sha'ban", "Ramadan", "Shawwal", "Dhu I-Qa'da", "Dhu I-Hijja"};
    if (month < 1 || month > 12) {
        return "";
    }
    return names[month];
}

jd_t islamic_to_jd(year_t year, month_t month, day_t day) {
    return islamic_to_jd(year, month, day, hour_t(12), minute_t(0), second_t(0));
}

jd_t islamic_to_jd(year_t year, month_t month, day_t day, hour_t hour, minute_t minute, second_t second) {
    const long double yearPart = (static_cast<long double>(year) - 1.0L) * 354.0L;
    const long double leapDays = std::floor((3.0L + 11.0L * static_cast<long double>(year)) / 30.0L);
    const long double monthPart = months_elapsed(month);
    const long double jd = static_cast<long double>(day) + monthPart + yearPart + leapDays + ISLAMIC_EPOCH - 1.0L;
    return jd_t(jd + static_cast<long double>(hms_to_days(hour, minute, second)));
}

void jd_to_islamic(jd_t jd, year_t& year, month_t& month, day_t& day) {
    hour_t hour;
    minute_t minute;
    second_t second;
    jd_to_islamic(jd, year, month, day, hour, minute, second);
}

void jd_to_islamic(jd_t jd, year_t& year, month_t& month, day_t& day, hour_t& hour, minute_t& minute, second_t& second) {
    const long double fraction = jd_day_fraction(jd);
    hour_t h = 0;
    minute_t mi = 0;
    second_t s = 0;
    int carry = jd_to_hms(fraction, h, mi, s);

    long double dayPortion = static_cast<long double>(hms_to_days(h, mi, s));
    long double jdDate = static_cast<long double>(jd) - dayPortion;
    if (carry == 1) {
        jdDate += 1.0L;
        h = 0;
        mi = 0;
        s = 0;
    } else if (carry == -1) {
        jdDate -= 1.0L;
        h = 23;
        mi = 59;
        s = 59;
    }

    const long double jdFloor = std::floor(jdDate);
    const long double yearCalc = std::floor((30.0L * (jdFloor - ISLAMIC_EPOCH) + 10646.0L) / 10631.0L);
    year_t islamicYear = static_cast<year_t>(yearCalc);

    const long double firstOfYear = static_cast<long double>(islamic_to_jd(islamicYear, 1, 1));
    long double monthCalc = std::ceil((jdDate - firstOfYear) / 29.5L) + 1.0L;
    if (monthCalc < 1.0L) {
        monthCalc = 1.0L;
    }
    if (monthCalc > 12.0L) {
        monthCalc = 12.0L;
    }
    month_t islamicMonth = static_cast<month_t>(monthCalc);

    const long double firstOfMonth = static_cast<long double>(islamic_to_jd(islamicYear, islamicMonth, 1));
    day_t islamicDay = static_cast<day_t>(std::floor(jdDate - firstOfMonth + 1.0L));
    if (islamicDay < 1) {
        islamicDay = 1;
    }

    year = islamicYear;
    month = islamicMonth;
    day = islamicDay;
    hour = h;
    minute = mi;
    second = s;
}

}  // namespace khronos

