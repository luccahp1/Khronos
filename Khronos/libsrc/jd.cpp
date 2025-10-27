/* khronos/jd.cpp */

#include <khronos/julian_day.hpp>
#include <khronos/gregorian_calendar.hpp>

#include <cmath>
#include <ctime>
#include <sstream>

namespace khronos {

namespace {

constexpr long double SECONDS_PER_DAY_L = 86400.0L;
constexpr long double HALF_DAY_SECONDS_L = SECONDS_PER_DAY_L / 2.0L;

jd_t make_local_jd(now_t mode) {
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

}  // namespace

tod_t hms_to_days(hour_t hour, minute_t minute, second_t second) noexcept {
    long double seconds = static_cast<long double>(hour) * 3600.0L;
    seconds += static_cast<long double>(minute) * 60.0L;
    seconds += static_cast<long double>(second);
    seconds = std::fmod(seconds, SECONDS_PER_DAY_L);
    if (seconds < 0.0L) {
        seconds += SECONDS_PER_DAY_L;
    }
    seconds -= HALF_DAY_SECONDS_L;
    return static_cast<tod_t>(seconds / SECONDS_PER_DAY_L);
}

long double jd_day_fraction(jd_t jd) noexcept {
    long double fraction = std::fmod(static_cast<long double>(jd) + 0.5L, 1.0L);
    if (fraction < 0.0L) {
        fraction += 1.0L;
    }
    return fraction;
}

int jd_to_hms(long double fraction, hour_t& hour, minute_t& minute, second_t& second) noexcept {
    long double totalSeconds = fraction * SECONDS_PER_DAY_L;
    long long rounded = static_cast<long long>(std::floor(totalSeconds + 0.5L));
    int carry = 0;
    if (rounded >= static_cast<long long>(SECONDS_PER_DAY_L)) {
        rounded -= static_cast<long long>(SECONDS_PER_DAY_L);
        carry = 1;
    } else if (rounded < 0) {
        rounded += static_cast<long long>(SECONDS_PER_DAY_L);
        carry = -1;
    }

    hour = static_cast<hour_t>(rounded / 3600);
    rounded %= 3600;
    minute = static_cast<minute_t>(rounded / 60);
    rounded %= 60;
    second = static_cast<second_t>(rounded);

    return carry;
}

Jd::Jd() : jd_(make_local_jd(WTIMEOFDAY)) {}

Jd::Jd(now_t mode) : jd_(make_local_jd(mode)) {}

Jd::Jd(jd_t jd) : jd_(jd) {}

std::string Jd::to_string() const {
    std::ostringstream oss;
    long double integerPart = 0.0L;
    const long double fractional = std::modfl(static_cast<long double>(jd_), &integerPart);
    if (std::fabsl(fractional) < 1e-12L) {
        oss << "JD " << static_cast<long long>(integerPart);
    } else {
        oss.setf(std::ios::fmtflags(0), std::ios::floatfield);
        oss.precision(15);
        oss << "JD " << static_cast<long double>(jd_);
    }
    return oss.str();
}

Jd operator+(days d, const Jd& jd) noexcept { return jd + d; }

Jd operator+(weeks w, const Jd& jd) noexcept { return jd + w; }

Jd operator+(hours h, const Jd& jd) noexcept { return jd + h; }

Jd operator+(minutes m, const Jd& jd) noexcept { return jd + m; }

Jd operator+(seconds s, const Jd& jd) noexcept { return jd + s; }

Jd operator+(years y, const Jd& jd) noexcept { return jd + y; }

std::ostream& operator<<(std::ostream& os, const Jd& jd) {
    return os << jd.to_string();
}

}  // namespace khronos

