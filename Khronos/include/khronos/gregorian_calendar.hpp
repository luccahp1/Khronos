#pragma once
/* khronos/gregorian_calendar.hpp */
#include <khronos/def.hpp>
#include <khronos/calendar.hpp>
#include <khronos/julian_day.hpp>
#include <string>
#include <compare>
#include <ostream>
namespace khronos {
        jd_t gregorian_to_jd(year_t y, month_t m, day_t d, hour_t h, minute_t mi, second_t s);
        jd_t gregorian_to_jd(year_t y, month_t m, day_t d);
        void jd_to_gregorian(jd_t jd, year_t& y, month_t& m, day_t& d);
        void jd_to_gregorian(jd_t jd, year_t& y, month_t& m, day_t& d, hour_t& h, minute_t& mi, second_t& s);


        class Gregorian {
	public:
		Gregorian(); explicit Gregorian(now_t mode);
		Gregorian(year_t y, month_t m, day_t d); Gregorian(year_t y, month_t m, day_t d, hour_t h, minute_t mi, second_t s);
		explicit Gregorian(Jd jd);
		Gregorian& operator=(Jd jd);
		year_t year() const; month_t month() const; day_t day() const; hour_t hour() const; minute_t minute() const; second_t second() const;
		operator Jd() const;
		auto operator<=>(const Gregorian& rhs) const { return jd_ <=> rhs.jd_; } bool operator==(const Gregorian& rhs) const { return jd_ == rhs.jd_; }
		Gregorian operator+(days d)const { return Gregorian(Jd(jd_) + d); } Gregorian operator-(days d)const { return Gregorian(Jd(jd_) - d); }
		Gregorian operator+(weeks w)const { return Gregorian(Jd(jd_) + w); } Gregorian operator-(weeks w)const { return Gregorian(Jd(jd_) - w); }
		Gregorian operator+(hours h)const { return Gregorian(Jd(jd_) + h); } Gregorian operator-(hours h)const { return Gregorian(Jd(jd_) - h); }
		Gregorian operator+(minutes m)const { return Gregorian(Jd(jd_) + m); } Gregorian operator-(minutes m)const { return Gregorian(Jd(jd_) - m); }
		Gregorian operator+(seconds s)const { return Gregorian(Jd(jd_) + s); } Gregorian operator-(seconds s)const { return Gregorian(Jd(jd_) - s); }
		Gregorian operator+(months m) const; Gregorian operator-(months m) const { return *this + months(-m.v); }
		Gregorian operator+(years y) const; Gregorian operator-(years y) const { return *this + years(-y.v); }
		long double operator-(const Gregorian& rhs) const { return long double(jd_ - rhs.jd_); }
		std::string to_string() const;
	private: jd_t jd_{ 0 };
	};
	inline year_t year(const Gregorian& g) { return g.year(); } inline month_t month(const Gregorian& g) { return g.month(); }
	inline day_t day(const Gregorian& g) { return g.day(); } inline hour_t hour(const Gregorian& g) { return g.hour(); }
	inline minute_t minute(const Gregorian& g) { return g.minute(); } inline second_t second(const Gregorian& g) { return g.second(); }
	inline std::ostream& operator<<(std::ostream& os, const Gregorian& g) { return os << g.to_string(); }
}