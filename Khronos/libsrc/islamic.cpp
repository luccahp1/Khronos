/* khronos/islamic.cpp */
#include <khronos/islamic_calendar.hpp>
#include <khronos/calendar.hpp>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cmath>
namespace khronos {
	static void ymdhms_from_islamic(jd_t jd, year_t& y, month_t& m, day_t& d, hour_t& h, minute_t& mi, second_t& s) { jd_to_islamic(jd, y, m, d, h, mi, s); }
	Islamic::Islamic() { time_t t = time(nullptr); tm tmLocal{}; localtime_s(&tmLocal, &t); jd_ = gregorian_to_jd(year_t(tmLocal.tm_year + 1900), month_t(tmLocal.tm_mon + 1), day_t(tmLocal.tm_mday), hour_t(tmLocal.tm_hour), minute_t(tmLocal.tm_min), second_t(tmLocal.tm_sec)); }
	Islamic::Islamic(now_t mode) { time_t t = time(nullptr); tm tmLocal{}; localtime_s(&tmLocal, &t); if (mode == NOTIMEOFDAY) jd_ = gregorian_to_jd(year_t(tmLocal.tm_year + 1900), month_t(tmLocal.tm_mon + 1), day_t(tmLocal.tm_mday)); else jd_ = gregorian_to_jd(year_t(tmLocal.tm_year + 1900), month_t(tmLocal.tm_mon + 1), day_t(tmLocal.tm_mday), hour_t(tmLocal.tm_hour), minute_t(tmLocal.tm_min), second_t(tmLocal.tm_sec)); }
	Islamic::Islamic(year_t y, month_t m, day_t d) { jd_ = islamic_to_jd(y, m, d); } Islamic::Islamic(year_t y, month_t m, day_t d, hour_t h, minute_t mi, second_t s) { jd_ = islamic_to_jd(y, m, d, h, mi, s); } Islamic::Islamic(Jd jd) :jd_((jd_t)jd.jd()) {}
	year_t Islamic::year() const { year_t y; month_t m; day_t d; hour_t h; minute_t mi; second_t s; ymdhms_from_islamic(jd_, y, m, d, h, mi, s); return y; }
	month_t Islamic::month() const { year_t y; month_t m; day_t d; hour_t h; minute_t mi; second_t s; ymdhms_from_islamic(jd_, y, m, d, h, mi, s); return m; }
	day_t Islamic::day() const { year_t y; month_t m; day_t d; hour_t h; minute_t mi; second_t s; ymdhms_from_islamic(jd_, y, m, d, h, mi, s); return d; }
	hour_t Islamic::hour() const { year_t y; month_t m; day_t d; hour_t h; minute_t mi; second_t s; ymdhms_from_islamic(jd_, y, m, d, h, mi, s); return h; }
	minute_t Islamic::minute() const { year_t y; month_t m; day_t d; hour_t h; minute_t mi; second_t s; ymdhms_from_islamic(jd_, y, m, d, h, mi, s); return mi; }
	second_t Islamic::second() const { year_t y; month_t m; day_t d; hour_t h; minute_t mi; second_t s; ymdhms_from_islamic(jd_, y, m, d, h, mi, s); return s; }
	Islamic::operator Jd() const { return Jd(jd_); }
	std::string Islamic::to_string() const { year_t y; month_t m; day_t d; hour_t h24; minute_t mi; second_t s; ymdhms_from_islamic(jd_, y, m, d, h24, mi, s); int w = day_of_week(jd_); static const char* awd[7] = { "al-ithnayn","ath-thulatha'","al-'arb`a'","al-khamis","al-jum`a","as-sabt","al-ahad" }; std::string monthNames[13] = { "","Muharram","Safar","RabiI","RabiII","JumadaI","JumadatTania","Rajab","Shaban","Ramadan","Shawwal","DhulQada","DhulHijja" }; int h12 = (h24 % 12); if (h12 == 0) h12 = 12; const char* ampm = (h24 < 12 ? "am" : "pm"); std::ostringstream oss; oss << awd[(w + 1) % 7] << ", " << monthNames[m] << ' ' << d << ' ' << y << ", " << h12 << ":" << std::setw(2) << std::setfill('0') << mi << ":" << std::setw(2) << std::setfill('0') << int(std::floor(s + 0.5)) << ' ' << ampm; return oss.str(); }
}