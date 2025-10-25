#pragma once
/* khronos/calendar.hpp */
#include <khronos/def.hpp>
#include <khronos/utility.hpp>
#include <string>
#include <cassert>
namespace khronos {
	enum civil_month_codes_long { January = 1, February, March, April, May, June, July, August, September, October, November, December };
	enum civil_month_codes_short { JAN = January, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC };
	enum civil_weekday_codes_long { Monday = 0, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday };
	enum civil_weekday_codes_short { MON = Monday, TUE, WED, THU, FRI, SAT, SUN };


	inline std::string civil_month_name(month_t m) {
		static const char* n[13] = { "","January","February","March","April","May","June","July","August","September","October","November","December" };
		assert(m >= 1 && m <= 12); return n[m];
	}
	inline std::string civil_short_month_name(month_t m) {
		static const char* n[13] = { "","JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC" };
		assert(m >= 1 && m <= 12); return n[m];
	}


	inline bool is_gregorian_leapyear(year_t y) {
		if ((y % 4) != 0) return false; if ((y % 100) != 0) return true; return (y % 400) == 0;
	}
	inline day_t gregorian_days_in_month(month_t m, bool leap) {
		static const day_t dN[13] = { 0,31,28,31,30,31,30,31,31,30,31,30,31 };
		static const day_t dL[13] = { 0,31,29,31,30,31,30,31,31,30,31,30,31 };
		assert(m >= 1 && m <= 12); return leap ? dL[m] : dN[m];
	}
	inline std::string gregorian_month_name(month_t m) { return civil_month_name(m); }
	inline std::string gregorian_short_month_name(month_t m) { return civil_short_month_name(m); }


	inline int day_of_week(jd_t jd) { int jw = int(utility::jwday(double(jd))); return (jw + 6) % 7; }


	constexpr year_t operator"" _CE(unsigned long long y) { return (year_t)y; }
	constexpr year_t operator"" _BCE(unsigned long long y) { return year_t(1) - (year_t)y; }
	constexpr year_t operator"" _AD(unsigned long long y) { return (year_t)y; }
	constexpr year_t operator"" _BC(unsigned long long y) { return year_t(1) - (year_t)y; }
}