#pragma once
/* khronos/islamic_calendar.hpp */
#include <khronos/def.hpp>
#include <khronos/julian_day.hpp>
#include <string>
#include <compare>
#include <ostream>
namespace khronos {
	enum IslamicMonth { Muharram = 1, Safar, RabiI, RabiII, JumadaI, JumadatTania, Rajab, Shaban, Ramadan, Shawwal, DhulQada, DhulHijja };
        inline bool is_islamic_leapyear(year_t y) {
                long long c = (y % 30 + 30) % 30;
                static const bool leap[30] = { false,true,false,true,false,false,true,false,true,false,false,true,false,true,false,false,true,false,true,false,false,true,false,true,false,false,true,false,true,false };
                return leap[c];
        }
        std::string islamic_month_name(month_t m);
        day_t islamic_days_in_month(year_t y, month_t m);
        jd_t islamic_to_jd(year_t y, month_t m, day_t d);
        jd_t islamic_to_jd(year_t y, month_t m, day_t d, hour_t h, minute_t mi, second_t s);
        void jd_to_islamic(jd_t jd, year_t& y, month_t& m, day_t& d);
        void jd_to_islamic(jd_t jd, year_t& y, month_t& m, day_t& d, hour_t& h, minute_t& mi, second_t& s);
        class Islamic {
        public:
                Islamic(); explicit Islamic(now_t mode);
                Islamic(year_t y, month_t m, day_t d); Islamic(year_t y, month_t m, day_t d, hour_t h, minute_t mi, second_t s); explicit Islamic(Jd jd);
                year_t year() const; month_t month() const; day_t day() const; hour_t hour() const; minute_t minute() const; second_t second() const;
                operator Jd() const; auto operator<=>(const Islamic& r) const { return jd_ <=> r.jd_; } bool operator==(const Islamic& r) const { return jd_ == r.jd_; }
                Islamic operator+(days d)const { return Islamic(Jd(jd_) + d); } Islamic operator-(days d)const { return Islamic(Jd(jd_) - d); }
                Islamic operator+(weeks w)const { return Islamic(Jd(jd_) + w); } Islamic operator-(weeks w)const { return Islamic(Jd(jd_) - w); }
                Islamic operator+(hours h)const { return Islamic(Jd(jd_) + h); } Islamic operator-(hours h)const { return Islamic(Jd(jd_) - h); }
                Islamic operator+(minutes m)const { return Islamic(Jd(jd_) + m); } Islamic operator-(minutes m)const { return Islamic(Jd(jd_) - m); }
                Islamic operator+(seconds s)const { return Islamic(Jd(jd_) + s); } Islamic operator-(seconds s)const { return Islamic(Jd(jd_) - s); }
                Islamic operator+(months m) const; Islamic operator-(months m) const { return *this + months(-m.v); }
                Islamic operator+(years y) const; Islamic operator-(years y) const { return *this + years(-y.v); }
                long double operator-(const Islamic& rhs) const { return long double(jd_ - rhs.jd_); }
                std::string to_string() const;
        private: jd_t jd_{ 0 };
        };
	inline std::ostream& operator<<(std::ostream& os, const Islamic& g) { return os << g.to_string(); }
	inline year_t year(const Islamic& g) { return g.year(); } inline month_t month(const Islamic& g) { return g.month(); }
	inline day_t day(const Islamic& g) { return g.day(); } inline hour_t hour(const Islamic& g) { return g.hour(); }
	inline minute_t minute(const Islamic& g) { return g.minute(); } inline second_t second(const Islamic& g) { return g.second(); }
}