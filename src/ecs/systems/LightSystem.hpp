
#pragma once
#include "Components.hpp"
#include "ECS.hpp"
#include "System.hpp"
#include "Window.hpp"
#include "Camera.hpp"
#include "Time.hpp"

class LightSystem : public System {
public:
	void update(Camera camera, Time time, uint ticks, EntitySet& set) {
		uchar ambient = lerp::smooth(std::abs(time.mins() - 720.0f) / 720, 0, 200);

		SDL_Texture* texture = TextureManager::createTexture(Window::instance->size, {0, 0, 0, ambient});
		SDL_BlendMode blendMode = SDL_ComposeCustomBlendMode(SDL_BLENDFACTOR_SRC_ALPHA, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD, SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE_MINUS_SRC_COLOR, SDL_BLENDOPERATION_ADD);
		SDL_SetTextureBlendMode(TextureManager::lightTexture, blendMode);

		for (Entity entity : set) {
			if (entities.find(entity) == entities.end()) continue;
			
			LightComponent& lightComponent = ecs->getComponent<LightComponent>(entity);
			if (!lightComponent.active) continue;
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);

			vec screenPosition = camera.screenPosition(positionComponent.position);
			float radius = lightComponent.intensity + lightComponent.flickerAmplitude * lerp::flicker(lightComponent.flickerSpeed * ticks / 1000);
			int size  = 2 * BIT * camera.zoom * radius;

			TextureStyle style;
			style.tint = lightComponent.tint;
			TextureManager::drawTexture(TextureManager::lightTexture, texture, {0,0}, {255, 255}, screenPosition, vec(size, size), style);
		}

		TextureStyle style;
		style.centered = false;
		TextureManager::drawTexture(texture, nullptr, {0,0}, Window::instance->size, {0,0}, Window::instance->size, style);
		SDL_DestroyTexture(texture);
	}
};