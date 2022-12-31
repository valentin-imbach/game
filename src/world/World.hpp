
#pragma once
#include "GuiManager.hpp"
#include "PlayerSystem.hpp"
#include "utils.hpp"
#include "ECS.hpp"
#include "systems.hpp"
#include "Map.hpp"
#include "Events.hpp"

using DrawQueue = std::vector<std::pair<float, DrawCall>>;

class World {
public:
	World(std::string name);
	World(std::fstream& stream);
	void update(uint dt);
	void handleEvents();

	void serialise(std::fstream& stream);
	void deserialise(std::fstream& stream);

	std::vector<InputEvent> inputEvents;
	std::bitset<size_t(InputStateId::MAX)> inputState;

	Entity player = 0;
	Entity camera = 0;
private:
	void rosterComponents();
	void rosterSystems();

	void generate();

	std::unique_ptr<GuiElement> makeInventory();

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
	DamageSystem* damageSystem;
	PlayerSystem* playerSystem;
	ColliderDrawSystem* colldierDrawSystem;
	GridSystem* gridSystem;
	InteractionSystem* interactionSystem;
	MonsterAiSystem* monsterAiSystem;

	GuiManager guiManager;

	std::string name;

	Map map;
	GridMap gridMap;

	friend class Game;
	friend class Console;
	friend class DebugScreen;
};