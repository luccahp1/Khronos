/* khronos/hebrew_to_jd.cpp */
#include <khronos/hebrew_calendar.hpp>
#include <cmath>
namespace khronos {
	static constexpr long double HEBREW_EPOCH = 347995.5L;
	static inline bool hebrew_leap(year_t y) { return ((7 * y + 1) % 19) < 7; }
	static long double molad_tishri(year_t y) { long long months = (235 * y - 234) / 19; long long parts = 204 + 793 * (months % 1080); long long hours = 5 + 12 * months + (793 * (months / 1080) + parts) / 1080; long long days = 2 + 29 * months + hours / 24; long long chalakim = (hours % 24) * 1080 + parts % 1080; return HEBREW_EPOCH + days + (chalakim / (24.0L * 1080.0L)); }
	static long double rosh_hashana(year_t y) { long double t = molad_tishri(y); long long d = (long long)std::floor(t); int dow = int(std::floor(d + 1.5)) % 7; if (dow == 1 || dow == 4 || dow == 6) d += 1; return (long double)d + 0.5L; }
	static int hebrew_year_days(year_t y) { return int(rosh_hashana(y + 1) - rosh_hashana(y)); }
	static int hebrew_months_in_year(year_t y) { return hebrew_leap(y) ? 13 : 12; }
	static int hebrew_last_day_of_month(year_t y, month_t m) { bool leap = hebrew_leap(y); if (m == 2 || m == 4 || m == 6 || m == 10 || m == 13) return 29; if (m == 12 && !leap) return 29; int yearDays = hebrew_year_days(y); if (m == 8) return (yearDays % 10 == 5 || yearDays % 10 == 3) ? 30 : 29; if (m == 9) return (yearDays % 10 == 5) ? 29 : 30; return 30; }
	jd_t hebrew_to_jd(year_t y, month_t m, day_t d) { return hebrew_to_jd(y, m, d, hour_t(12), minute_t(0), second_t(0)); }
	jd_t hebrew_to_jd(year_t y, month_t m, day_t d, hour_t h, minute_t mi, second_t s) { long double jd = rosh_hashana(y); int mm = 7; while (mm != m) { jd += hebrew_last_day_of_month(y, (month_t)mm); mm = (mm == 13 ? 1 : mm + 1); } jd += (d - 1); long double frac = ((long double(h) * 60.0L + long double(mi)) * 60.0L + long double(s)) / 86400.0L; return jd_t(jd + frac); }
	void jd_to_hebrew(jd_t JD, year_t& y, month_t& m, day_t& d) { hour_t h; minute_t mi; second_t s; jd_to_hebrew(JD, y, m, d, h, mi, s); }
	void jd_to_hebrew(jd_t JD, year_t& y, month_t& m, day_t& d, hour_t& h, minute_t& mi, second_t& s) {
		long double jd = std::floor((long double)JD) + 0.5L; year_t yy = (year_t)std::floor(((jd - HEBREW_EPOCH) / 365.2468222L)) - 1; while (jd >= rosh_hashana(yy + 1)) ++yy; while (jd < rosh_hashana(yy)) --yy; long double start = rosh_hashana(yy); int mm = 7; while (true) { int len = hebrew_last_day_of_month(yy, (month_t)mm); if (jd < start + len) { m = (month_t)mm; d = day_t(int(jd - start) + 1); break; } start += len; mm = (mm == 13 ? 1 : mm + 1); }
		long double frac = (long double)JD - std::floor((long double)JD); long double totalSeconds = frac * 86400.0L; h = hour_t(totalSeconds / 3600.0L); totalSeconds -= h * 3600.0L; mi = minute_t(totalSeconds / 60.0L); totalSeconds -= mi * 60.0L; s = second_t(totalSeconds); y = yy;
	}
}