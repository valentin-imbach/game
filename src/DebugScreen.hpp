
#pragma once
#include "TextureManager.hpp"
#include "TextManager.hpp"
#include "Window.hpp"
#include "World.hpp"

class DebugScreen {
public:
	bool handleEvent(SDL_Event event) {
		if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_F1) {
			active = !active;
			return true;
		}
		return false;
	}

	void update(World* w, int fps) {
		world = w;
		framesPerSecond = fps;
	}

	void draw() {
		if (!active) return;
		pair windowSize = Window::instance->size;
		TextureManager::drawRect({20, 20}, {300, windowSize.y / 2 - 30}, {0, 0, 0, 100}, false, true);

		std::string fpsText = "FPS: " + std::to_string(framesPerSecond);
		TextManager::drawText(fpsText, {30, 30});

		if (!world) return;
		std::string entityCountText = "Entity Count: " + std::to_string(world->ecs.entityCount());
		TextManager::drawText(entityCountText, {30, 50});

		if (!world->player) return;
		pair playerPosition = round(world->ecs.getComponent<PositionComponent>(world->player).position);
		std::string playerPositionText = "Player Position: " + std::to_string(playerPosition.x) + ", " + std::to_string(playerPosition.y);
		TextManager::drawText(playerPositionText, {30, 70});

		CreatureState::value playerState = world->ecs.getComponent<CreatureStateComponent>(world->player).state;
		std::string playerStateText = "Player State: " + CreatureState::to_string(playerState);
		TextManager::drawText(playerStateText, {30, 90});
	}

private:
	int framesPerSecond;
	World* world;
	bool active = true;
};