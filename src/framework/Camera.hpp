
#pragma once
#include "utils.hpp"
#include "TextureManager.hpp"
#include "Window.hpp"

struct Camera {
	vec position;
	float zoom = 4;

	pair screenPosition(vec pos) {
		return vec::round(BIT * zoom * (pos  - position) + (Window::instance->size) / 2);
	}

	vec worldPosition(pair pos) {
		return position + vec(pos - Window::instance->size / 2) / (zoom * BIT);
	}
};