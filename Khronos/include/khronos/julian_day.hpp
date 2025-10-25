#pragma once
/* khronos/julian_day.hpp */

#include <khronos/def.hpp>
#include <khronos/timeofday.hpp>
#include <khronos/gregorian_calendar.hpp>

#include <compare>
#include <ostream>
#include <string>

namespace khronos {

tod_t hms_to_days(hour_t hour, minute_t minute, second_t second) noexcept;
long double jd_day_fraction(jd_t jd) noexcept;
int jd_to_hms(long double fraction, hour_t& hour, minute_t& minute, second_t& second) noexcept;

class Jd {
public:
    Jd();
    explicit Jd(now_t mode);
    explicit Jd(jd_t jd);

    constexpr jd_t jd() const noexcept { return jd_; }
    constexpr operator jd_t() const noexcept { return jd_; }

    auto operator<=>(const Jd&) const = default;
    bool operator==(const Jd&) const = default;

    Jd operator+(days d) const noexcept { return Jd(jd_ + d.v); }
    Jd operator-(days d) const noexcept { return Jd(jd_ - d.v); }
    Jd operator+(weeks w) const noexcept { return Jd(jd_ + w.v * 7.0L); }
    Jd operator-(weeks w) const noexcept { return Jd(jd_ - w.v * 7.0L); }
    Jd operator+(hours h) const noexcept { return Jd(jd_ + h.v / 24.0L); }
    Jd operator-(hours h) const noexcept { return Jd(jd_ - h.v / 24.0L); }
    Jd operator+(minutes m) const noexcept { return Jd(jd_ + m.v / (24.0L * 60.0L)); }
    Jd operator-(minutes m) const noexcept { return Jd(jd_ - m.v / (24.0L * 60.0L)); }
    Jd operator+(seconds s) const noexcept { return Jd(jd_ + s.v / 86400.0L); }
    Jd operator-(seconds s) const noexcept { return Jd(jd_ - s.v / 86400.0L); }
    Jd operator+(years y) const noexcept { return Jd(jd_ + y.v * static_cast<long double>(EARTH_ORBITAL_PERIOD_DAYS)); }
    Jd operator-(years y) const noexcept { return Jd(jd_ - y.v * static_cast<long double>(EARTH_ORBITAL_PERIOD_DAYS)); }

    Jd& operator+=(days d) noexcept { jd_ += d.v; return *this; }
    Jd& operator-=(days d) noexcept { jd_ -= d.v; return *this; }
    Jd& operator+=(weeks w) noexcept { jd_ += w.v * 7.0L; return *this; }
    Jd& operator-=(weeks w) noexcept { jd_ -= w.v * 7.0L; return *this; }
    Jd& operator+=(hours h) noexcept { jd_ += h.v / 24.0L; return *this; }
    Jd& operator-=(hours h) noexcept { jd_ -= h.v / 24.0L; return *this; }
    Jd& operator+=(minutes m) noexcept { jd_ += m.v / (24.0L * 60.0L); return *this; }
    Jd& operator-=(minutes m) noexcept { jd_ -= m.v / (24.0L * 60.0L); return *this; }
    Jd& operator+=(seconds s) noexcept { jd_ += s.v / 86400.0L; return *this; }
    Jd& operator-=(seconds s) noexcept { jd_ -= s.v / 86400.0L; return *this; }
    Jd& operator+=(years y) noexcept { jd_ += y.v * static_cast<long double>(EARTH_ORBITAL_PERIOD_DAYS); return *this; }
    Jd& operator-=(years y) noexcept { jd_ -= y.v * static_cast<long double>(EARTH_ORBITAL_PERIOD_DAYS); return *this; }

    jd_t operator-(const Jd& rhs) const noexcept { return jd_ - rhs.jd_; }

    std::string to_string() const;

private:
    jd_t jd_{0};
};

Jd operator+(days d, const Jd& jd) noexcept;
Jd operator+(weeks w, const Jd& jd) noexcept;
Jd operator+(hours h, const Jd& jd) noexcept;
Jd operator+(minutes m, const Jd& jd) noexcept;
Jd operator+(seconds s, const Jd& jd) noexcept;
Jd operator+(years y, const Jd& jd) noexcept;

std::ostream& operator<<(std::ostream& os, const Jd& jd);

}  // namespace khronos

