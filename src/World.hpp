
#pragma once
#include "utils/utils.hpp"
#include "ECS.hpp"
#include "Systems/systems.hpp"
#include "Map.hpp"
#include "Events.hpp"

typedef std::vector<std::pair<float, DrawCall>> DrawQueue;
typedef std::unordered_map<pair, Entity> GridMap;

class World {
public:
	World(std::string name);
	~World();
	void update(uint dt);
	void handleEvents();

	std::vector<InputEvent> inputEvents;
	std::bitset<size_t(InputState::MAX)> inputStates;
	static World* world;
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

	std::string name;

	Entity player;
	Entity camera;

	Map map;
	GridMap gridMap;
};