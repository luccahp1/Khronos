#pragma once
/* khronos/julian_calendar.hpp */
#include <khronos/def.hpp>
#include <khronos/calendar.hpp>
#include <khronos/julian_day.hpp>
#include <string>
#include <compare>
#include <ostream>
namespace khronos {
	inline bool is_julian_leapyear(year_t y) { return (y % 4) == 0; }
	inline day_t julian_days_in_month(month_t m, bool leap) {
		static const day_t dN[13] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };
		static const day_t dL[13] = { 0,31,29,31,30,31,30,31,31,30,31,30,31 };
		return leap ? dL[m] : dN[m];
	}
	jd_t julian_to_jd(year_t y, month_t m, day_t d);
	jd_t julian_to_jd(year_t y, month_t m, day_t d, hour_t h, minute_t mi, second_t s);
	void jd_to_julian(jd_t jd, year_t& y, month_t& m, day_t& d);
	void jd_to_julian(jd_t jd, year_t& y, month_t& m, day_t& d, hour_t& h, minute_t& mi, second_t& s);


	class Julian {
	public:
		Julian(); explicit Julian(now_t mode);
		Julian(year_t y, month_t m, day_t d); Julian(year_t y, month_t m, day_t d, hour_t h, minute_t mi, second_t s); explicit Julian(Jd jd);
		Julian& operator=(Jd jd);
		year_t year() const; month_t month() const; day_t day() const; hour_t hour() const; minute_t minute() const; second_t second() const;
		operator Jd() const;
		auto operator<=>(const Julian& r) const { return jd_ <=> r.jd_; } bool operator==(const Julian& r) const { return jd_ == r.jd_; }
		Julian operator+(days d)const { return Julian(Jd(jd_) + d); } Julian operator-(days d)const { return Julian(Jd(jd_) - d); }
		Julian operator+(weeks w)const { return Julian(Jd(jd_) + w); } Julian operator-(weeks w)const { return Julian(Jd(jd_) - w); }
		Julian operator+(hours h)const { return Julian(Jd(jd_) + h); } Julian operator-(hours h)const { return Julian(Jd(jd_) - h); }
		Julian operator+(minutes m)const { return Julian(Jd(jd_) + m); } Julian operator-(minutes m)const { return Julian(Jd(jd_) - m); }
		Julian operator+(seconds s)const { return Julian(Jd(jd_) + s); } Julian operator-(seconds s)const { return Julian(Jd(jd_) - s); }
                Julian operator+(months mm) const; Julian operator-(months mm) const { return *this + months(-mm.v); }
		Julian operator+(years yy) const; Julian operator-(years yy) const { return *this + years(-yy.v); }
		long double operator-(const Julian& rhs) const { return long double(jd_ - rhs.jd_); }
		std::string to_string() const;
	private: jd_t jd_{ 0 };
	};
	inline std::ostream& operator<<(std::ostream& os, const Julian& j) { return os << j.to_string(); }
	inline year_t year(const Julian& g) { return g.year(); } inline month_t month(const Julian& g) { return g.month(); }
	inline day_t day(const Julian& g) { return g.day(); } inline hour_t hour(const Julian& g) { return g.hour(); }
	inline minute_t minute(const Julian& g) { return g.minute(); } inline second_t second(const Julian& g) { return g.second(); }
}