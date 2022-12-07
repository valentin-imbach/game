
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
	void update(uint dt);
	std::vector<InputEvent> inputEvents;
	std::bitset<size_t(InputState::MAX)> inputStates;

private:
	void rosterComponents();
	void rosterSystems();

	ECS ecs;
	SpriteSystem* spriteSystem;
	CreatureMovementSystem* creatureMovementSystem;
	ControllerSystem* controllerSystem;
	CameraSystem* cameraSystem;
	CreatureAnimationSystem* creatureAnimationSystem;
	CollisionSystem* collisionSystem;
	ItemSystem* itemSystem;

	std::string name;

	Entity player;
	Entity camera;

	Map map;
	GridMap gridMap;

	void drawMap();
	void drawEntities(DrawQueue& drawQueue);
	void handleEvents();
};