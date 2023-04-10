
#pragma once
#include "Components.hpp"
#include "ECS.hpp"
#include "System.hpp"
#include "Window.hpp"

#define MINUTE 30

struct Time {
	int day, hour, minute;
	uint rest;

	Time()
		: day(0), hour(0), minute(0), rest(0) {}

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
	void update(Entity camera, Time time, uint ticks) {

		Uint8 maxLight = 0;
		Uint8 minLight = 200;

		vec cameraPosition = ecs->getComponent<PositionComponent>(camera).position;
		float cameraZoom = ecs->getComponent<CameraComponent>(camera).zoom;

		Uint8 ambient = Lerp::smooth(abs(time.mins() - 720.0f) / 720, maxLight, minLight);

		SDL_Texture* texture = SDL_CreateTexture(Window::instance->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, Window::instance->size.x, Window::instance->size.y);
		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

		SDL_SetRenderTarget(Window::instance->renderer, texture);
		SDL_SetRenderDrawBlendMode(Window::instance->renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(Window::instance->renderer, 0, 0, 0, ambient);
		SDL_RenderClear(Window::instance->renderer);

		SDL_BlendMode blendMode = SDL_ComposeCustomBlendMode(
			SDL_BLENDFACTOR_SRC_ALPHA, // dstColorFactor
			SDL_BLENDFACTOR_ONE,			 // srcColorFactor
			SDL_BLENDOPERATION_ADD,				 // colorOperation
			SDL_BLENDFACTOR_ZERO,			 // dstAlphaFactor
			SDL_BLENDFACTOR_ONE_MINUS_SRC_COLOR, // srcAlphaFactor
			SDL_BLENDOPERATION_ADD);
		// SDL_SetRenderDrawBlendMode(Window::instance->renderer, SDL_BLENDMODE_ADD);
		// SDL_SetTextureBlendMode(texture, blendMode);
		SDL_SetTextureBlendMode(TextureManager::lightTexture, blendMode);
		for (Entity entity : entities) {
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			LightComponent& lightComponent = ecs->getComponent<LightComponent>(entity);

			float radius = lightComponent.intensity + lightComponent.flickerAmplitude * Lerp::flicker(lightComponent.flickerSpeed * ticks / 1000);

			pair position = round(BIT * cameraZoom * (positionComponent.position - cameraPosition)) + (Window::instance->size) / 2;
			int size = BIT * cameraZoom * radius;
			SDL_Rect dst = {position.x - size, position.y - size, 2 * size, 2 * size};

			SDL_SetTextureColorMod(TextureManager::lightTexture, lightComponent.tint.r, lightComponent.tint.g, lightComponent.tint.b);
			SDL_RenderCopyEx(Window::instance->renderer, TextureManager::lightTexture, NULL, &dst, 0, NULL, SDL_FLIP_NONE);

		}

		SDL_SetRenderTarget(Window::instance->renderer, NULL);
		SDL_SetRenderDrawBlendMode(Window::instance->renderer, SDL_BLENDMODE_BLEND);
		SDL_RenderCopy(Window::instance->renderer, texture, NULL, NULL);

		SDL_DestroyTexture(texture);
	}
};