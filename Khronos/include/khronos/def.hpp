#pragma once
/* khronos/def.hpp */

namespace khronos {

using second_t = double;
using minute_t = int;
using hour_t = int;
using tod_t = double;
using day_t = int;
using month_t = int;
using year_t = long long;
using jd_t = long double;

struct days {
    long double v;
    explicit constexpr days(long double n) : v(n) {}
};

struct weeks {
    long double v;
    explicit constexpr weeks(long double n) : v(n) {}
};

struct hours {
    long double v;
    explicit constexpr hours(long double n) : v(n) {}
};

struct minutes {
    long double v;
    explicit constexpr minutes(long double n) : v(n) {}
};

struct seconds {
    long double v;
    explicit constexpr seconds(long double n) : v(n) {}
};

struct months {
    long long v;
    explicit constexpr months(long long n) : v(n) {}
};

struct years {
    long double v;
    explicit constexpr years(long double n) : v(n) {}
};

inline constexpr days days(long double n) {
    return days{n};
}

inline constexpr days days_(long double n) {
    return days{n};
}

inline constexpr weeks weeks(long double n) {
    return weeks{n};
}

inline constexpr weeks weeks_(long double n) {
    return weeks{n};
}

inline constexpr hours hours(long double n) {
    return hours{n};
}

inline constexpr hours hours_(long double n) {
    return hours{n};
}

inline constexpr minutes minutes(long double n) {
    return minutes{n};
}

inline constexpr minutes minutes_(long double n) {
    return minutes{n};
}

inline constexpr seconds seconds(long double n) {
    return seconds{n};
}

inline constexpr seconds seconds_(long double n) {
    return seconds{n};
}

inline constexpr months months(long long n) {
    return months{n};
}

inline constexpr months months_(long long n) {
    return months{n};
}

inline constexpr years years(long double n) {
    return years{n};
}

inline constexpr years years_(long double n) {
    return years{n};
}

enum now_t { NOTIMEOFDAY = 0, WTIMEOFDAY };

constexpr double EARTH_ORBITAL_PERIOD_DAYS = 365.256363004;

}  // namespace khronos

