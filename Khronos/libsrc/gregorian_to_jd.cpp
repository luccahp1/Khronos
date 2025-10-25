/* khronos/gregorian_to_jd.cpp */
#include <khronos/gregorian_calendar.hpp>
#include <khronos/timeofday.hpp>
#include <cmath>
namespace khronos {
	jd_t gregorian_to_jd(year_t Y, month_t M, day_t D, hour_t h, minute_t m, second_t s) { long long y = Y; int mth = M; if (mth <= 2) { y -= 1; mth += 12; } long long A = y / 100; long long B = 2 - A + A / 4; long double day = (long double)D; long double frac = ((long double(h) * 60.0L + long double(m)) * 60.0L + long double(s)) / 86400.0L; long double jd = std::floor(365.25L * (y + 4716)) + std::floor(30.6001L * (mth + 1)) + day + B - 1524.5L + frac; return jd_t(jd); }
	jd_t gregorian_to_jd(year_t Y, month_t M, day_t D) { return gregorian_to_jd(Y, M, D, hour_t(12), minute_t(0), second_t(0)); }
}