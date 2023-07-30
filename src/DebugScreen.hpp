
#pragma once
#include "TextureManager.hpp"
#include "TextManager.hpp"
#include "Window.hpp"
#include "World.hpp"

class DebugScreen {
public:
	bool handleEvent(InputEvent event) {
		if (event.id == InputEventId::DEBUG) {
			active = !active;
			return true;
		}
		return false;
	}

	void update(World* w, int fps, int ups) {
		world = w;
		framesPerSecond = fps;
		updatesPerSecond = ups;
	}

	void draw() {
		if (!active) return;
		pair windowSize = Window::instance->size;
		TextureManager::drawRect({20, 20}, {300, windowSize.y / 2 - 30}, {0, 0, 0, 100}, false, true);

		std::string text;
		text += "FPS: " + std::to_string(framesPerSecond) + '\n';
		text += "UPS: " + std::to_string(updatesPerSecond) + '\n';

		if (world) {
			text += "Ticks: " + std::to_string(world->ticks) + '\n';
			text += "Time: " + world->time.string() + '\n';
			text += "Entity Count: " + std::to_string(world->ecs.entityCount()) + '\n';
			int particleCount = world->particleSystem.activeCount;
			text += "Particles: " + std::to_string(particleCount) + '\n';

			if (world->player) {
				pair playerPosition = vec::round(world->ecs.getComponent<PositionComponent>(world->player).position);
				text += "Player Position: " + std::to_string(playerPosition.x) + ", " + std::to_string(playerPosition.y) + '\n';

				pair playerChunk = vec::round(world->ecs.getComponent<PositionComponent>(world->player).chunk);
				text += "Player Chunk: " + std::to_string(playerChunk.x) + ", " + std::to_string(playerChunk.y) + '\n';

				CreatureState::value playerState = world->ecs.getComponent<CreatureStateComponent>(world->player).state;
				text += "Player State: " + CreatureState::to_string(playerState) + '\n';

				Biome::value biome = world->playerRealm->environment->getBiome(playerPosition);
				text += "Biome: " + Biome::to_string(biome) + '\n';

				int temperature = world->playerRealm->environment->temparatureMap->get(playerPosition);
				text += "Temperateure: " + std::to_string(temperature) + '\n';

				int elevation = world->playerRealm->environment->elevationMap->get(playerPosition);
				text += "Elevation: " + std::to_string(elevation) + '\n';

				int precipitation = world->playerRealm->environment->precipitationMap->get(playerPosition);
				text += "Precipitation: " + std::to_string(precipitation) + '\n';

				int vegetation = world->playerRealm->environment->vegetationMap->get(playerPosition);
				text += "Vegetation: " + std::to_string(vegetation) + '\n';

				int variation = world->playerRealm->environment->variationMap->get(playerPosition);
				text += "Variation: " + std::to_string(variation) + '\n';

				TileId::value tileId = world->playerRealm->map->getTileId(playerPosition);
				text += "Tile: " + TileId::to_string(tileId) + '\n';
			}
		}

		TextManager::drawText(text, pair(30, 30));
	}

private:
	int framesPerSecond;
	int updatesPerSecond;
	World* world;
	bool active = true;
};