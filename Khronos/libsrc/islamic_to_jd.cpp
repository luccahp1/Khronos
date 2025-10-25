/* khronos/islamic_to_jd.cpp (tabular Islamic) */
#include <khronos/islamic_calendar.hpp>
#include <cmath>
namespace khronos {
	static constexpr long double ISLAMIC_EPOCH = 1948439.5L;
	static inline long double islamic_month_start(year_t y, month_t m) { long double months = std::ceil(29.5L * (m - 1)); long double days = (y - 1) * 354.0L + std::floor((3 + 11 * y) / 30.0L); return months + days; }
	jd_t islamic_to_jd(year_t y, month_t m, day_t d) { return islamic_to_jd(y, m, d, hour_t(12), minute_t(0), second_t(0)); }
	jd_t islamic_to_jd(year_t y, month_t m, day_t d, hour_t h, minute_t mi, second_t s) { long double n = islamic_month_start(y, m) + (long double)d - 1.0L; long double frac = ((long double(h) * 60.0L + long double(mi)) * 60.0L + long double(s)) / 86400.0L; return jd_t(ISLAMIC_EPOCH + n + frac); }
	void jd_to_islamic(jd_t JD, year_t& y, month_t& m, day_t& d) { hour_t h; minute_t mi; second_t s; jd_to_islamic(JD, y, m, d, h, mi, s); }
	void jd_to_islamic(jd_t JD, year_t& y, month_t& m, day_t& d, hour_t& h, minute_t& mi, second_t& s) {
		long double j = std::floor(JD) + 0.0L; long double days = j - ISLAMIC_EPOCH + 0.0L; year_t yy = (year_t)std::floor((30.0L * days + 10646.0L) / 10631.0L); year_t y1 = yy; long double m1 = std::ceil((days - ((y1 - 1) * 354.0L + std::floor((3 + 11 * y1) / 30.0L)) + 1.0L) / 29.5L); if (m1 < 1) m1 = 1; if (m1 > 12) m1 = 12; month_t mm = (month_t)m1; long double start = islamic_month_start(y1, mm); long double day = days - start + 1.0L; if (day < 1) { mm = (month_t)(mm - 1); if (mm < 1) { mm = 12; y1 -= 1; } start = islamic_month_start(y1, mm); day = days - start + 1.0L; }
		long double frac = (long double)JD - std::floor((long double)JD);
		long double totalSeconds = frac * 86400.0L; h = hour_t(totalSeconds / 3600.0L); totalSeconds -= h * 3600.0L; mi = minute_t(totalSeconds / 60.0L); totalSeconds -= mi * 60.0L; s = second_t(totalSeconds);
		y = y1; m = mm; d = day_t((int)std::floor(day));
	}
}