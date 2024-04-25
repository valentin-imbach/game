
#pragma once
#include "SDL2/SDL_pixels.h"
#include <cstdint>

struct Colour {
	Colour (uint8_t r = 255, uint8_t g = 255, uint8_t b = 255, uint8_t a = 255) : r(r), g(g), b(b), a(a) {}
	uint8_t r, g, b, a;

	Colour mult(Colour other) {
		return Colour(r * other.r / 255, g * other.g / 255, b * other.b / 255);
	}

	operator SDL_Colour() const {
		return {r,g,b,a};
	}
};

