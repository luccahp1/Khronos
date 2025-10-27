#pragma once
/* khronos/vulcan_calendar.hpp */
#include <khronos/def.hpp>
#include <khronos/julian_day.hpp>
#include <string>
#include <compare>
#include <ostream>
namespace khronos {
	enum VulcanMonth { Zat = 1, Druh, KriBrax, reTKhutai, TkeKhuti, Khuti, TaKrat, KriLior, etkhior, Tlakht, TkeTas, Tasmeen };
	constexpr jd_t VULCAN_EPOCH = 0.0L;
	std::string vulcan_month_name(month_t m);
	jd_t vulcan_to_jd(year_t y, month_t m, day_t d); jd_t vulcan_to_jd(year_t y, month_t m, day_t d, hour_t h, minute_t mi, second_t s);
	void jd_to_vulcan(jd_t jd, year_t& y, month_t& m, day_t& d); void jd_to_vulcan(jd_t jd, year_t& y, month_t& m, day_t& d, hour_t& h, minute_t& mi, second_t& s);
	class Vulcan {
	public:
		Vulcan(); explicit Vulcan(now_t mode);
		Vulcan(year_t y, month_t m, day_t d); Vulcan(year_t y, month_t m, day_t d, hour_t h, minute_t mi, second_t s); explicit Vulcan(Jd jd);
		Vulcan& operator=(Jd jd) { jd_ = jd.jd(); return *this; }
		year_t year() const; month_t month() const; day_t day() const; hour_t hour() const; minute_t minute() const; second_t second() const;
		operator Jd() const; auto operator<=>(const Vulcan& r) const { return jd_ <=> r.jd_; } bool operator==(const Vulcan& r) const { return jd_ == r.jd_; }
		Vulcan operator+(days d)const { return Vulcan(Jd(jd_) + d); } Vulcan operator-(days d)const { return Vulcan(Jd(jd_) - d); }
		Vulcan operator+(weeks w)const { return Vulcan(Jd(jd_) + w); } Vulcan operator-(weeks w)const { return Vulcan(Jd(jd_) - w); }
		Vulcan operator+(hours h)const { return Vulcan(Jd(jd_) + h); } Vulcan operator-(hours h)const { return Vulcan(Jd(jd_) - h); }
		Vulcan operator+(minutes m)const { return Vulcan(Jd(jd_) + m); } Vulcan operator-(minutes m)const { return Vulcan(Jd(jd_) - m); }
		Vulcan operator+(seconds s)const { return Vulcan(Jd(jd_) + s); } Vulcan operator-(seconds s)const { return Vulcan(Jd(jd_) - s); }
		long double operator-(const Vulcan& rhs) const { return long double(jd_ - rhs.jd_); }
		std::string to_string() const;
	private: jd_t jd_{ 0 };
	};
	inline std::ostream& operator<<(std::ostream& os, const Vulcan& g) { return os << g.to_string(); }
	inline year_t year(const Vulcan& g) { return g.year(); } inline month_t month(const Vulcan& g) { return g.month(); }
	inline day_t day(const Vulcan& g) { return g.day(); } inline hour_t hour(const Vulcan& g) { return g.hour(); }
	inline minute_t minute(const Vulcan& g) { return g.minute(); } inline second_t second(const Vulcan& g) { return g.second(); }
}