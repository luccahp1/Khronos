#pragma once
/* khronos/utility.hpp */
#include <cmath>
namespace khronos {
	namespace utility {
		inline double mod(double a, double b) {
			double r = std::fmod(a, b);
			if (r < 0) r += std::fabs(b);
			return r;
		}
		inline int jwday(double j) { return int(std::floor(j + 1.5)) % 7; }
	}
} // namespace khronos::utility