#pragma once
/* khronos/timeofday.hpp */
#include <khronos/def.hpp>
#include <cmath>
namespace khronos {
	constexpr double SECONDS_PER_DAY = 24.0 * 60.0 * 60.0;
	constexpr int operator"" _am(unsigned long long h) { return int(h % 12); }
	constexpr int operator"" _pm(unsigned long long h) { return int((h % 12) + 12); }
	inline tod_t tod(hour_t h, minute_t m, second_t s) {
		double secs = (double(h) * 60.0 + double(m)) * 60.0 + double(s);
		return tod_t(secs / SECONDS_PER_DAY);
	}
	inline void tod_to_hms(tod_t t, hour_t& h, minute_t& m, second_t& s) {
		double secs = t * SECONDS_PER_DAY;
		if (secs < 0) secs = 0;
		secs = std::fmod(secs, SECONDS_PER_DAY);
		h = hour_t(secs / 3600.0);
		secs -= h * 3600.0;
		m = minute_t(secs / 60.0);
		secs -= m * 60.0;
		s = second_t(secs);
	}
}