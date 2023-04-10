
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

		std::string ticksText = "Ticks: " + std::to_string(world->ticks);
		TextManager::drawText(ticksText, {150, 30});

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

		Biome::value biome = world->realm->map->getBiome(playerPosition);
		std::string biomeText = "Biome: " + Biome::to_string(biome);
		TextManager::drawText(biomeText, {30, 120});

		int temperature = world->realm->map->temparatureMap->get(playerPosition);
		std::string temperatureText = "Temperateure: " + std::to_string(temperature);
		TextManager::drawText(temperatureText, {30, 140});

		int elevation = world->realm->map->elevationMap->get(playerPosition);
		std::string elevationText = "Elevation: " + std::to_string(elevation);
		TextManager::drawText(elevationText, {30, 160});

		int precipitation = world->realm->map->precipitationMap->get(playerPosition);
		std::string precipitationText = "Precipitation: " + std::to_string(precipitation);
		TextManager::drawText(precipitationText, {30, 180});

		int vegetation = world->realm->map->vegetationMap->get(playerPosition);
		std::string vegetationText = "Vegetation: " + std::to_string(vegetation);
		TextManager::drawText(vegetationText, {30, 200});

		int variation = world->realm->map->variationMap->get(playerPosition);
		std::string variationText = "Variation: " + std::to_string(variation);
		TextManager::drawText(variationText, {30, 230});

		TileId::value tileId = world->realm->map->getTileId(playerPosition);
		std::string tileText = "Tile: " + TileId::to_string(tileId);
		TextManager::drawText(tileText, {30, 250});

		int particleCount = world->particleSystem.activeCount;
		std::string particleText = "Particles: " + std::to_string(particleCount);
		TextManager::drawText(particleText, {30, 280});

		pair playerChunk = round(world->ecs.getComponent<PositionComponent>(world->player).chunk);
		std::string playerChunkText = "Player Chunk: " + std::to_string(playerChunk.x) + ", " + std::to_string(playerChunk.y);
		TextManager::drawText(playerChunkText, {30, 310});

		std::string timeText = world->time.string();
		TextManager::drawText(timeText, {30, 340});
	}

private:
	int framesPerSecond;
	World* world;
	bool active = true;
};