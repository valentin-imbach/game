
#pragma once
#include "TextureManager.hpp"
#include "TextManager.hpp"
#include "Window.hpp"
#include "World.hpp"

class DebugScreen {
public:
	Entity entity = 0;
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
		TextureManager::drawRect({windowSize.x - 320, 20}, {300, windowSize.y - 30}, {0, 0, 0, 100}, false, true);

		std::string debugText = "Debug Info\n";
		debugText += "FPS: " + std::to_string(framesPerSecond) + '\n';
		debugText += "UPS: " + std::to_string(updatesPerSecond) + '\n';

		if (world) {
			debugText += "Ticks: " + std::to_string(world->ticks) + '\n';
			debugText += "Time: " + world->time.string() + '\n';
			debugText += "Entity Count: " + std::to_string(world->ecs.entityCount()) + '\n';
			int particleCount = world->particleSystem.activeCount;
			debugText += "Particles: " + std::to_string(particleCount) + '\n';
		}

		std::string tileText = "Tile Info\n";
		pair mPos = Window::instance->mousePosition;
		vec pos = world->camera.worldPosition(mPos);
		pair tile = vec::round(pos);

		Biome::value biome = world->playerRealm->environment->getBiome(tile);
		tileText += "Biome: " + Biome::to_string(biome) + '\n';

		int temperature = world->playerRealm->environment->temparatureMap->get(tile);
		tileText += "Temperateure: " + std::to_string(temperature) + '\n';

		int elevation = world->playerRealm->environment->elevationMap->get(tile);
		tileText += "Elevation: " + std::to_string(elevation) + '\n';

		int precipitation = world->playerRealm->environment->precipitationMap->get(tile);
		tileText += "Precipitation: " + std::to_string(precipitation) + '\n';

		int vegetation = world->playerRealm->environment->vegetationMap->get(tile);
		tileText += "Vegetation: " + std::to_string(vegetation) + '\n';

		int variation = world->playerRealm->environment->variationMap->get(tile);
		tileText += "Variation: " + std::to_string(variation) + '\n';

		GroundId::value groundId = world->playerRealm->map->getGroundId(tile);
		tileText += "Tile: " + GroundId::to_string(groundId) + '\n';

		TextManager::drawText(debugText, pair(30, 30));
		TextManager::drawText(tileText, pair(windowSize.x - 310, 30));

		if (!entity) return;
		std::string entityText = "Entity Info\n";

		if (world->ecs.hasComponent<PositionComponent>(entity)) {
			PositionComponent& positionComponent = world->ecs.getComponent<PositionComponent>(entity);
			entityText += "Position: " + std::to_string(positionComponent.position.x).substr(0, 5) + ", ";
			entityText += std::to_string(positionComponent.position.y).substr(0, 5) + '\n';
			entityText += "Chunk: " + std::to_string(positionComponent.chunk.x) + ", " + std::to_string(positionComponent.chunk.y) + '\n';
		}

		if (world->ecs.hasComponent<HealthComponent>(entity)) {
			HealthComponent& healthCompnenet = world->ecs.getComponent<HealthComponent>(entity);
			entityText += "Health: " + std::to_string(healthCompnenet.health) + "/" + std::to_string(healthCompnenet.maxHealth) + '\n';
		}
		
		if (world->ecs.hasComponent<MovementComponent>(entity)) {
			MovementState::value movementState = world->ecs.getComponent<MovementComponent>(entity).movementState;
			entityText += "Movement State: " + MovementState::to_string(movementState) + '\n';
		}

		if (world->ecs.hasComponent<ActionComponent>(entity)) {
			ActionState::value actionState = world->ecs.getComponent<ActionComponent>(entity).actionState;
			entityText += "Action State: " + ActionState::to_string(actionState) + '\n';
		}

		if (world->ecs.hasComponent<AiComponent>(entity)) {
			AiState::value aiState = world->ecs.getComponent<AiComponent>(entity).state;
			entityText += "Ai State: " + AiState::to_string(aiState) + '\n';
		}

		// if (world->ecs.hasComponent<AiWanderComponent>(entity)) {
		// 	vec pos = world->ecs.getComponent<AiWanderComponent>(entity).position;
		// 	entityText += "Wander Position:\n" + std::to_string(pos.x) + ",\n" + std::to_string(pos.y) + '\n';
		// }

		if (world->ecs.hasComponent<SensorComponent>(entity)) {
			vec pos = world->ecs.getComponent<SensorComponent>(entity).position;
			entityText += "Sensor Position:\n" + std::to_string(pos.x) + ",\n" + std::to_string(pos.y) + '\n';
		}

		TextManager::drawText(entityText, pair(windowSize.x - 310, windowSize.y / 2));
	}

private:
	int framesPerSecond;
	int updatesPerSecond;
	World* world;
	bool active = true;
};