
#pragma once
#include <cmath>
#include "vec.hpp"

namespace lerp {
	[[nodiscard]] static float linear(float t) {
		return t;
	}

	[[nodiscard]] static float linear(float t, float a, float b) {
		return a + linear(t) * (b-a);
	}

	[[nodiscard]] static float smooth(float t) {
		return 6 * std::pow(t, 5) - 15 * std::pow(t, 4) + 10 * std::pow(t, 3);
	}

	[[nodiscard]] static float smooth(float t, float a, float b) {
		return a + smooth(t) * (b-a);
	}

	[[nodiscard]] static vec smooth(float t, vec a, vec b) {
		return vec(smooth(t, a.x, b.x),  smooth(t, a.y, b.y));
	}

	[[nodiscard]] static float flicker(float t) {
		return std::sin(t)/2 + std::sin(M_PI * t)/4 + std::sin(M_PI * M_PI * t)/4;
	}

	[[nodiscard]] static vec shake(float t) {
		return vec(flicker(t), flicker(t + 10));
	}
};