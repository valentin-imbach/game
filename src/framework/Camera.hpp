
#pragma once
#include "utils.hpp"
#include "TextureManager.hpp"
#include "Window.hpp"

using RealmId = unsigned char;

#define SHAKE_AMP 20
#define SHAKE_DUR 1000

struct Camera {
	vec position;
	RealmId realmId;
	float zoom = 4;

	pair offset = {0, 0};
	uint shake = 0;

	void update(uint ticks) {
		if (!shake || ticks > shake + SHAKE_DUR) {
			shake = 0;
			return;
		}

		float t = float(ticks - shake)/SHAKE_DUR;
		float amp = lerp::smooth(t, SHAKE_AMP, 0);

		offset = vec::round(amp * lerp::shake(ticks));
	}

	vec screenPosition(vec pos) {
		return BIT * zoom * (pos  - position) + (Window::instance->size)/2 - offset;
	}

	vec worldPosition(vec pos) {
		return position + vec(pos - Window::instance->size / 2 + offset) / (zoom * BIT);
	}
};