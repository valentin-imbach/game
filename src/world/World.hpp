
#pragma once
#include "GuiManager.hpp"
#include "HealthSystem.hpp"
#include "LootSystem.hpp"
#include "utils.hpp"
#include "ECS.hpp"
#include "systems.hpp"
#include "Map.hpp"
#include "Events.hpp"

typedef std::vector<std::pair<float, DrawCall>> DrawQueue;

class World {
public:
	World(std::string name);
	void update(uint dt);
	void handleEvents();

	std::vector<InputEvent> inputEvents;
	std::bitset<size_t(InputStateId::MAX)> inputState;
private:
	void rosterComponents();
	void rosterSystems();

	ECS ecs;
	EntityDrawSystem* entityDrawSystem;
	CreatureMovementSystem* creatureMovementSystem;
	ControllerSystem* controllerSystem;
	CameraSystem* cameraSystem;
	CreatureAnimationSystem* creatureAnimationSystem;
	CollisionSystem* collisionSystem;
	ItemPickupSystem* itemPickupSystem;
	TileDrawSystem* tileDrawSystem;
	AnimalAiSystem* animalAiSystem;
	ForageSystem* forageSystem;
	HealthSystem* healthSystem;
	LootSystem* lootSystem;

	GuiManager guiManager;

	std::string name;

	Entity player;
	Entity camera;

	Map map;
	GridMap gridMap;

	friend class Game;
};