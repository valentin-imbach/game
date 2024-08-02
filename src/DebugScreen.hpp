
#pragma once
#include "TextureManager.hpp"
#include "TextManager.hpp"
#include "Window.hpp"
#include "World.hpp"
#include "Environment.hpp"
#include "Components.hpp"

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
		TextureManager::drawRect(pair(20, 20), pair(300, windowSize.y / 2 - 30), Colour(0, 0, 0, 100), false, true);
		TextureManager::drawRect(pair(windowSize.x - 320, 20), pair(300, windowSize.y - 30), Colour(0, 0, 0, 100), false, true);


		//* Debug
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

		TextManager::drawText(debugText, pair(30, 30));

		//* Tile
		std::string tileText = "Tile Info\n";

		pair mPos = Window::instance->mousePosition;
		vec pos = world->camera.worldPosition(mPos);
		pair tile = vec::round(pos);

		Environment* env = world->playerRealm->environment.get();
		tileText += "Position: (" + std::to_string(tile.x) + ", " + std::to_string(tile.y) + ")\n";
		tileText += "Biome: " + Biome::to_string(env->getBiome(tile)) + '\n';
		tileText += "Temperateure: " + std::to_string(env->temparatureMap->get(tile)) + '\n';
		tileText += "Elevation: " + std::to_string(env->elevationMap->get(tile)) + '\n';
		tileText += "Precipitation: " + std::to_string(env->precipitationMap->get(tile)) + '\n';
		tileText += "Vegetation: " + std::to_string(env->vegetationMap->get(tile)) + '\n';
		tileText += "Variation: " + std::to_string(env->variationMap->get(tile)) + '\n';

		GroundId::value groundId = world->playerRealm->ground(tile);
		tileText += "Tile: " + GroundId::to_string(groundId) + '\n';

		TextManager::drawText(tileText, pair(windowSize.x - 310, 30));

		//* Entity
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
			AiComponent& aiComponent = world->ecs.getComponent<AiComponent>(entity);
			entityText += "Ai Scores:";
			for (int i = 1; i < AiState::count; i++) {
				entityText += "- " + AiState::strings[i] + ": " + std::to_string(aiComponent.scores[i]) + "\n""";
			}
			entityText += "Ai State: " + AiState::to_string(aiComponent.state) + '\n';
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