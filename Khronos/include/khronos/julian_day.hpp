#pragma once
/* khronos/julian_day.hpp */
#include <khronos/def.hpp>
#include <khronos/timeofday.hpp>
#include <khronos/gregorian_calendar.hpp>
#include <string>
#include <compare>
#include <ostream>
#include <sstream>
#include <cmath>
namespace khronos {
	struct days { long double v; explicit days(long double n) :v(n) {} };
	struct weeks { long double v; explicit weeks(long double n) :v(n) {} };
	struct hours { long double v; explicit hours(long double n) :v(n) {} };
	struct minutes { long double v; explicit minutes(long double n) :v(n) {} };
	struct seconds { long double v; explicit seconds(long double n) :v(n) {} };
	struct years { long double v; explicit years(long double n) :v(n) {} };
	inline days days_(long double n) { return days(n); } inline weeks weeks_(long double n) { return weeks(n); } inline hours hours_(long double n) { return hours(n); } inline minutes minutes_(long double n) { return minutes(n); } inline seconds seconds_(long double n) { return seconds(n); } inline years years_(long double n) { return years(n); }
	inline days days(long double n) { return days(n); } inline weeks weeks(long double n) { return weeks(n); } inline hours hours(long double n) { return hours(n); } inline minutes minutes(long double n) { return minutes(n); } inline seconds seconds(long double n) { return seconds(n); } inline years years(long double n) { return years(n); }
	class Jd {
	public:
		Jd(); explicit Jd(now_t mode); explicit Jd(jd_t jd);
		constexpr jd_t jd() const noexcept { return jd_; } constexpr operator jd_t() const noexcept { return jd_; }
		auto operator<=>(const Jd&) const = default; bool operator==(const Jd&) const = default;
		Jd operator+(days d)const { return Jd(jd_ + d.v); } Jd operator-(days d)const { return Jd(jd_ - d.v); }
		Jd operator+(weeks w)const { return Jd(jd_ + w.v * 7.0L); } Jd operator-(weeks w)const { return Jd(jd_ - w.v * 7.0L); }
		Jd operator+(hours h)const { return Jd(jd_ + h.v / 24.0L); } Jd operator-(hours h)const { return Jd(jd_ - h.v / 24.0L); }
		Jd operator+(minutes m)const { return Jd(jd_ + m.v / (24.0L * 60.0L)); } Jd operator-(minutes m)const { return Jd(jd_ - m.v / (24.0L * 60.0L)); }
		Jd operator+(seconds s)const { return Jd(jd_ + s.v / 86400.0L); } Jd operator-(seconds s)const { return Jd(jd_ - s.v / 86400.0L); }
		Jd operator+(years y)const { return Jd(jd_ + y.v * (long double)EARTH_ORBITAL_PERIOD_DAYS); } Jd operator-(years y)const { return Jd(jd_ - y.v * (long double)EARTH_ORBITAL_PERIOD_DAYS); }
		Jd& operator+=(days d) { jd_ += d.v; return *this; } Jd& operator-=(days d) { jd_ -= d.v; return *this; }
		Jd& operator+=(weeks w) { jd_ += w.v * 7.0L; return *this; } Jd& operator-=(weeks w) { jd_ -= w.v * 7.0L; return *this; }
		Jd& operator+=(hours h) { jd_ += h.v / 24.0L; return *this; } Jd& operator-=(hours h) { jd_ -= h.v / 24.0L; return *this; }
		Jd& operator+=(minutes m) { jd_ += m.v / (24.0L * 60.0L); return *this; } Jd& operator-=(minutes m) { jd_ -= m.v / (24.0L * 60.0L); return *this; }
		Jd& operator+=(seconds s) { jd_ += s.v / 86400.0L; return *this; } Jd& operator-=(seconds s) { jd_ -= s.v / 86400.0L; return *this; }
		Jd& operator+=(years y) { jd_ += y.v * (long double)EARTH_ORBITAL_PERIOD_DAYS; return *this; } Jd& operator-=(years y) { jd_ -= y.v * (long double)EARTH_ORBITAL_PERIOD_DAYS; return *this; }
		std::string to_string() const;
	private: jd_t jd_{ 0 };
	};
	inline std::ostream& operator<<(std::ostream& os, const Jd& jd) { return os << jd.to_string(); }
}