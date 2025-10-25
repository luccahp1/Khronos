/* khronos/gregorian.cpp */
#include <khronos/gregorian_calendar.hpp>
#include <khronos/timeofday.hpp>
#include <khronos/calendar.hpp>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cmath>
namespace khronos {
	static void ymdhms_from_jd(jd_t jd, year_t& y, month_t& m, day_t& d, hour_t& h, minute_t& mi, second_t& s) { jd_to_gregorian(jd, y, m, d, h, mi, s); }
	static jd_t jd_from_ymdhms(year_t y, month_t m, day_t d, hour_t h, minute_t mi, second_t s) { return gregorian_to_jd(y, m, d, h, mi, s); }
	static day_t clamp_dom(year_t y, month_t m, day_t d) { bool leap = is_gregorian_leapyear(y); day_t dim = gregorian_days_in_month(m, leap); if (d > dim) d = dim; if (d < 1) d = 1; return d; }
	Gregorian::Gregorian() { time_t t = time(nullptr); tm tmLocal{}; localtime_s(&tmLocal, &t); jd_ = gregorian_to_jd(year_t(tmLocal.tm_year + 1900), month_t(tmLocal.tm_mon + 1), day_t(tmLocal.tm_mday), hour_t(tmLocal.tm_hour), minute_t(tmLocal.tm_min), second_t(tmLocal.tm_sec)); }
	Gregorian::Gregorian(now_t mode) { time_t t = time(nullptr); tm tmLocal{}; localtime_s(&tmLocal, &t); if (mode == NOTIMEOFDAY) jd_ = gregorian_to_jd(year_t(tmLocal.tm_year + 1900), month_t(tmLocal.tm_mon + 1), day_t(tmLocal.tm_mday)); else jd_ = gregorian_to_jd(year_t(tmLocal.tm_year + 1900), month_t(tmLocal.tm_mon + 1), day_t(tmLocal.tm_mday), hour_t(tmLocal.tm_hour), minute_t(tmLocal.tm_min), second_t(tmLocal.tm_sec)); }
	Gregorian::Gregorian(year_t y, month_t m, day_t d) { jd_ = gregorian_to_jd(y, m, d); }
	Gregorian::Gregorian(year_t y, month_t m, day_t d, hour_t h, minute_t mi, second_t s) { jd_ = gregorian_to_jd(y, m, d, h, mi, s); }
	Gregorian::Gregorian(Jd jd) :jd_((jd_t)jd.jd()) {}
	year_t Gregorian::year() const { year_t y; month_t m; day_t d; hour_t h; minute_t mi; second_t s; ymdhms_from_jd(jd_, y, m, d, h, mi, s); return y; }
	month_t Gregorian::month() const { year_t y; month_t m; day_t d; hour_t h; minute_t mi; second_t s; ymdhms_from_jd(jd_, y, m, d, h, mi, s); return m; }
	day_t Gregorian::day() const { year_t y; month_t m; day_t d; hour_t h; minute_t mi; second_t s; ymdhms_from_jd(jd_, y, m, d, h, mi, s); return d; }
	hour_t Gregorian::hour() const { year_t y; month_t m; day_t d; hour_t h; minute_t mi; second_t s; ymdhms_from_jd(jd_, y, m, d, h, mi, s); return h; }
	minute_t Gregorian::minute() const { year_t y; month_t m; day_t d; hour_t h; minute_t mi; second_t s; ymdhms_from_jd(jd_, y, m, d, h, mi, s); return mi; }
	second_t Gregorian::second() const { year_t y; month_t m; day_t d; hour_t h; minute_t mi; second_t s; ymdhms_from_jd(jd_, y, m, d, h, mi, s); return s; }
	Gregorian::operator Jd() const { return Jd(jd_); }
	Gregorian Gregorian::operator+(months mn) const { year_t y; month_t m; day_t d; hour_t h; minute_t mi; second_t s; ymdhms_from_jd(jd_, y, m, d, h, mi, s); long long total = (long long)m - 1 + mn.v; long long newY = y + total / 12; long long newM = (total % 12 + 12) % 12 + 1; day_t newD = clamp_dom((year_t)newY, (month_t)newM, d); jd_t newjd = jd_from_ymdhms((year_t)newY, (month_t)newM, newD, h, mi, s); return Gregorian(Jd(newjd)); }
	Gregorian Gregorian::operator+(years yr) const { year_t y; month_t m; day_t d; hour_t h; minute_t mi; second_t s; ymdhms_from_jd(jd_, y, m, d, h, mi, s); year_t newY = y + (year_t)yr.v; day_t newD = clamp_dom(newY, m, d); jd_t newjd = jd_from_ymdhms(newY, m, newD, h, mi, s); return Gregorian(Jd(newjd)); }
	static std::string two_digits(int v) { std::ostringstream oss; oss << std::setw(2) << std::setfill('0') << v; return oss.str(); }
	std::string Gregorian::to_string() const { year_t y; month_t m; day_t d; hour_t h24; minute_t mi; second_t s; ymdhms_from_jd(jd_, y, m, d, h24, mi, s); int w = day_of_week(jd_); static const char* wd[7] = { "Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday" }; std::string mon = gregorian_month_name(m); bool bce = (y <= 0); long long disp = bce ? (1 - (long long)y) : (long long)y; int h12 = (h24 % 12); if (h12 == 0) h12 = 12; const char* ampm = (h24 < 12 ? "am" : "pm"); std::ostringstream oss; oss << wd[w] << ", " << mon << ' ' << d << ' ' << disp << (bce ? " BCE, " : " CE, ") << h12 << ":" << two_digits(mi) << ":" << two_digits(int(std::floor(s + 0.5))) << ' ' << ampm; return oss.str(); }
}