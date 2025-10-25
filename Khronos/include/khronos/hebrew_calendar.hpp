#pragma once
/* khronos/hebrew_calendar.hpp */
#include <khronos/def.hpp>
#include <khronos/julian_day.hpp>
#include <string>
#include <compare>
#include <ostream>
namespace khronos {
	enum HebrewMonth { Nisan = 1, Iyyar, Sivan, Tammuz, Av, Elul, Tishri, Heshvan, Kislev, Teveth, Shevat, Adar, Veadar };
	std::string hebrew_month_name(month_t m);
	jd_t hebrew_to_jd(year_t y, month_t m, day_t d);
	jd_t hebrew_to_jd(year_t y, month_t m, day_t d, hour_t h, minute_t mi, second_t s);
	void jd_to_hebrew(jd_t jd, year_t& y, month_t& m, day_t& d);
	void jd_to_hebrew(jd_t jd, year_t& y, month_t& m, day_t& d, hour_t& h, minute_t& mi, second_t& s);
	class Hebrew {
	public:
		Hebrew(); explicit Hebrew(now_t mode);
		Hebrew(year_t y, month_t m, day_t d); Hebrew(year_t y, month_t m, day_t d, hour_t h, minute_t mi, second_t s); explicit Hebrew(Jd jd);
		year_t year() const; month_t month() const; day_t day() const; hour_t hour() const; minute_t minute() const; second_t second() const;
		operator Jd() const; auto operator<=>(const Hebrew& r) const { return jd_ <=> r.jd_; } bool operator==(const Hebrew& r) const { return jd_ == r.jd_; }
		Hebrew operator+(days d)const { return Hebrew(Jd(jd_) + d); } Hebrew operator-(days d)const { return Hebrew(Jd(jd_) - d); }
		Hebrew operator+(weeks w)const { return Hebrew(Jd(jd_) + w); } Hebrew operator-(weeks w)const { return Hebrew(Jd(jd_) - w); }
		Hebrew operator+(hours h)const { return Hebrew(Jd(jd_) + h); } Hebrew operator-(hours h)const { return Hebrew(Jd(jd_) - h); }
		Hebrew operator+(minutes m)const { return Hebrew(Jd(jd_) + m); } Hebrew operator-(minutes m)const { return Hebrew(Jd(jd_) - m); }
		Hebrew operator+(seconds s)const { return Hebrew(Jd(jd_) + s); } Hebrew operator-(seconds s)const { return Hebrew(Jd(jd_) - s); }
		long double operator-(const Hebrew& rhs) const { return long double(jd_ - rhs.jd_); }
		std::string to_string() const;
	private: jd_t jd_{ 0 };
	};
	inline std::ostream& operator<<(std::ostream& os, const Hebrew& g) { return os << g.to_string(); }
	inline year_t year(const Hebrew& g) { return g.year(); } inline month_t month(const Hebrew& g) { return g.month(); }
	inline day_t day(const Hebrew& g) { return g.day(); } inline hour_t hour(const Hebrew& g) { return g.hour(); }
	inline minute_t minute(const Hebrew& g) { return g.minute(); } inline second_t second(const Hebrew& g) { return g.second(); }
}