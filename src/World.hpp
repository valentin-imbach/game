
#pragma once
#include "utils/utils.hpp"
#include "ECS.hpp"
#include "Systems/systems.hpp"
#include "Map.hpp"

class World {
public:
	World(std::string name);
	void update(uint dt);

private:
	void rosterComponents();
	void rosterSystems();

	ECS ecs;
	SpriteSystem* spriteSystem;
	CreatureMovementSystem* creatureMovementSystem;
	ControllerSystem* controllerSystem;
	CameraSystem* cameraSystem;

	std::string name;

	Entity player;
	Entity camera;

	Map map;
	void renderMap();
};