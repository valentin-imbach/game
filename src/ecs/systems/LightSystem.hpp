
#pragma once
#include "Components.hpp"
#include "ECS.hpp"
#include "System.hpp"
#include "Window.hpp"
#include "Camera.hpp"

#define MINUTE 1000

struct Time {
	int day, hour, minute;
	uint rest;

	Time()
		: day(0), hour(6), minute(0), rest(0) {}

	void update(uint dt) {
		rest += dt;
		minute += rest / MINUTE;
		rest %= MINUTE;
		hour += minute / 60;
		minute %= 60;
		day += hour / 24;
		hour %= 24;
	}

	std::string string() {
		std::string hourString = std::string(hour < 10, '0') + std::to_string(hour);
		std::string minuteString = std::string(minute < 10, '0') + std::to_string(minute);
		return "Day " + std::to_string(day) + ", " + hourString + ":" + minuteString;
	}

	int mins() {
		return 60 * hour + minute;
	}
};

class LightSystem : public System {
public:
	void update(Camera camera, Time time, uint ticks, EntitySet& set) {
		uchar ambient = lerp::smooth(std::abs(time.mins() - 720.0f) / 720, 0, 200);

		SDL_Texture* texture = TextureManager::createTexture(Window::instance->size, {0, 0, 0, ambient});
		SDL_BlendMode blendMode = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_SRC_ALPHA, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE_MINUS_SRC_COLOR, SDL_BLENDOPERATION_ADD);
		SDL_SetTextureBlendMode(TextureManager::lightTexture, blendMode);

		for (Entity entity : set) {
			if (entities.find(entity) == entities.end()) continue;
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			LightComponent& lightComponent = ecs->getComponent<LightComponent>(entity);

			pair screenPosition = camera.screenPosition(positionComponent.position);
			float radius = lightComponent.intensity + lightComponent.flickerAmplitude * lerp::flicker(lightComponent.flickerSpeed * ticks / 1000);
			int size  = 2* BIT * camera.zoom * radius;

			TextureStyle style;
			style.tint = lightComponent.tint;
			TextureManager::drawTexture(TextureManager::lightTexture, texture, {0,0}, {255, 255}, screenPosition, {size, size}, style);
		}

		TextureStyle style;
		style.centered = false;
		TextureManager::drawTexture(texture, nullptr, {0,0}, Window::instance->size, {0,0}, Window::instance->size, style);

		SDL_DestroyTexture(texture);
	}
};