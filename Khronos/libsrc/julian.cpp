/* khronos/julian.cpp */
#include <khronos/julian_calendar.hpp>
#include <khronos/calendar.hpp>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cmath>
namespace khronos {
	static void ymdhms_from_julian(jd_t jd, year_t& y, month_t& m, day_t& d, hour_t& h, minute_t& mi, second_t& s) { jd_to_julian(jd, y, m, d, h, mi, s); }
	Julian::Julian() { time_t t = time(nullptr); tm tmLocal{}; localtime_s(&tmLocal, &t); jd_ = julian_to_jd(year_t(tmLocal.tm_year + 1900), month_t(tmLocal.tm_mon + 1), day_t(tmLocal.tm_mday), hour_t(tmLocal.tm_hour), minute_t(tmLocal.tm_min), second_t(tmLocal.tm_sec)); }
	Julian::Julian(now_t mode) { time_t t = time(nullptr); tm tmLocal{}; localtime_s(&tmLocal, &t); if (mode == NOTIMEOFDAY) jd_ = julian_to_jd(year_t(tmLocal.tm_year + 1900), month_t(tmLocal.tm_mon + 1), day_t(tmLocal.tm_mday)); else jd_ = julian_to_jd(year_t(tmLocal.tm_year + 1900), month_t(tmLocal.tm_mon + 1), day_t(tmLocal.tm_mday), hour_t(tmLocal.tm_hour), minute_t(tmLocal.tm_min), second_t(tmLocal.tm_sec)); }
	Julian::Julian(year_t y, month_t m, day_t d) { jd_ = julian_to_jd(y, m, d); } Julian::Julian(year_t y, month_t m, day_t d, hour_t h, minute_t mi, second_t s) { jd_ = julian_to_jd(y, m, d, h, mi, s); } Julian::Julian(Jd jd) :jd_((jd_t)jd.jd()) {}
	year_t Julian::year() const { year_t y; month_t m; day_t d; hour_t h; minute_t mi; second_t s; ymdhms_from_julian(jd_, y, m, d, h, mi, s); return y; }
	month_t Julian::month() const { year_t y; month_t m; day_t d; hour_t h; minute_t mi; second_t s; ymdhms_from_julian(jd_, y, m, d, h, mi, s); return m; }
	day_t Julian::day() const { year_t y; month_t m; day_t d; hour_t h; minute_t mi; second_t s; ymdhms_from_julian(jd_, y, m, d, h, mi, s); return d; }
	hour_t Julian::hour() const { year_t y; month_t m; day_t d; hour_t h; minute_t mi; second_t s; ymdhms_from_julian(jd_, y, m, d, h, mi, s); return h; }
	minute_t Julian::minute() const { year_t y; month_t m; day_t d; hour_t h; minute_t mi; second_t s; ymdhms_from_julian(jd_, y, m, d, h, mi, s); return mi; }
	second_t Julian::second() const { year_t y; month_t m; day_t d; hour_t h; minute_t mi; second_t s; ymdhms_from_julian(jd_, y, m, d, h, mi, s); return s; }
	Julian::operator Jd() const { return Jd(jd_); }
	Julian Julian::operator+(months mm) const { year_t y; month_t m; day_t d; hour_t h; minute_t mi; second_t s; ymdhms_from_julian(jd_, y, m, d, h, mi, s); long long total = (long long)m - 1 + mm.v; long long newY = y + total / 12; long long newM = (total % 12 + 12) % 12 + 1; day_t maxd = julian_days_in_month((month_t)newM, is_julian_leapyear((year_t)newY)); if (d > maxd) d = maxd; jd_t newjd = julian_to_jd((year_t)newY, (month_t)newM, d, h, mi, s); return Julian(Jd(newjd)); }
	Julian Julian::operator+(years yy) const { year_t y; month_t m; day_t d; hour_t h; minute_t mi; second_t s; ymdhms_from_julian(jd_, y, m, d, h, mi, s); year_t ny = y + (year_t)yy.v; day_t maxd = julian_days_in_month(m, is_julian_leapyear(ny)); if (d > maxd) d = maxd; jd_t newjd = julian_to_jd(ny, m, d, h, mi, s); return Julian(Jd(newjd)); }
	static std::string two(int v) { std::ostringstream o; o << std::setw(2) << std::setfill('0') << v; return o.str(); }
	std::string Julian::to_string() const { year_t y; month_t m; day_t d; hour_t h24; minute_t mi; second_t s; ymdhms_from_julian(jd_, y, m, d, h24, mi, s); int w = day_of_week(jd_); static const char* wd[7] = { "Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday" }; std::string mon = civil_month_name(m); bool bc = (y <= 0); long long disp = bc ? (1 - (long long)y) : (long long)y; int h12 = (h24 % 12); if (h12 == 0) h12 = 12; const char* ampm = (h24 < 12 ? "am" : "pm"); std::ostringstream oss; oss << wd[w] << ", " << mon << ' ' << d << ' ' << disp << (bc ? " BC, " : " AD, ") << h12 << ":" << two(mi) << ":" << two(int(std::floor(s + 0.5))) << ' ' << ampm; return oss.str(); }
}