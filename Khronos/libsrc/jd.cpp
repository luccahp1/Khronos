/* khronos/jd.cpp */
#include <khronos/julian_day.hpp>
#include <ctime>
#include <sstream>
#include <cmath>
namespace khronos {
	Jd::Jd() { time_t t = time(nullptr); tm tmLocal{}; localtime_s(&tmLocal, &t); jd_ = gregorian_to_jd(year_t(tmLocal.tm_year + 1900), month_t(tmLocal.tm_mon + 1), day_t(tmLocal.tm_mday), hour_t(tmLocal.tm_hour), minute_t(tmLocal.tm_min), second_t(tmLocal.tm_sec)); }
	Jd::Jd(now_t mode) { time_t t = time(nullptr); tm tmLocal{}; localtime_s(&tmLocal, &t); if (mode == NOTIMEOFDAY) jd_ = gregorian_to_jd(year_t(tmLocal.tm_year + 1900), month_t(tmLocal.tm_mon + 1), day_t(tmLocal.tm_mday)); else jd_ = gregorian_to_jd(year_t(tmLocal.tm_year + 1900), month_t(tmLocal.tm_mon + 1), day_t(tmLocal.tm_mday), hour_t(tmLocal.tm_hour), minute_t(tmLocal.tm_min), second_t(tmLocal.tm_sec)); }
	Jd::Jd(jd_t v) :jd_(v) {}
	std::string Jd::to_string() const { std::ostringstream oss; long double iv; long double frac = std::modfl((long double)jd_, &iv); if (std::fabsl(frac) < 1e-12L) oss << "JD " << (long long)iv; else oss << "JD " << (double)jd_; return oss.str(); }
}