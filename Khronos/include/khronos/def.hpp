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


	enum now_t { NOTIMEOFDAY = 0, WTIMEOFDAY };
	constexpr double EARTH_ORBITAL_PERIOD_DAYS = 365.256363004;
}