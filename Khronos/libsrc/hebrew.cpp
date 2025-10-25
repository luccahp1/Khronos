/* khronos/hebrew.cpp */
#include <khronos/hebrew_calendar.hpp>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cmath>
namespace khronos {
	static void ymdhms_from_hebrew(jd_t jd, year_t& y, month_t& m, day_t& d, hour_t& h, minute_t& mi, second_t& s) { jd_to_hebrew(jd, y, m, d, h, mi, s); }
	Hebrew::Hebrew() { time_t t = time(nullptr); tm tmLocal{}; localtime_s(&tmLocal, &t); jd_ = gregorian_to_jd(year_t(tmLocal.tm_year + 1900), month_t(tmLocal.tm_mon + 1), day_t(tmLocal.tm_mday), hour_t(tmLocal.tm_hour), minute_t(tmLocal.tm_min), second_t(tmLocal.tm_sec)); }
	Hebrew::Hebrew(now_t mode) {
		time_t t = time(nullptr); tm tmLocal{}; localtime_s(&tmLocal, &t); if (mode == NOTIMEOFDAY) jd_ = gregorian_to_jd(year_t(tmLocal.tm_year + 1900), month_t(tmLocal.tm_mon + 1), day_t(tmLocal.tm_mday)); else jd_ = gregorian_to_jd(year_t(tmLocal.tm_year + 190