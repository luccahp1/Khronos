/* khronos/gregorian_to_jd.cpp */
#include <khronos/gregorian_calendar.hpp>
#include <khronos/timeofday.hpp>
#include <cmath>
namespace khronos {
        jd_t gregorian_to_jd(year_t Y, month_t M, day_t D, hour_t h, minute_t m, second_t s) {
                long long y = Y;
                int month = M;
                if (month <= 2) {
                        y -= 1;
                        month += 12;
                }
                const long long a = y / 100;
                const long long b = 2 - a + a / 4;
                const long double base = std::floor(365.25L * (y + 4716))
                        + std::floor(30.6001L * (month + 1))
                        + static_cast<long double>(D) + b - 1524.5L;
                return jd_t(base + static_cast<long double>(hms_to_days(h, m, s)));
        }

        jd_t gregorian_to_jd(year_t Y, month_t M, day_t D) {
                return gregorian_to_jd(Y, M, D, hour_t(12), minute_t(0), second_t(0));
        }
}