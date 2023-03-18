
#pragma once
#include "ECS.hpp"
#include "Events.hpp"
#include "GuiManager.hpp"
#include "Map.hpp"
#include "PlayerSystem.hpp"
#include "systems.hpp"
#include "utils.hpp"

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

	void init();
	void generate();

	std::unique_ptr<GuiElement> makeInventory();
	std::unique_ptr<GuiElement> makeMenu();

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
	ColliderDrawSystem* colliderDrawSystem;
	GridSystem* gridSystem;
	InteractionSystem* interactionSystem;
	MonsterAiSystem* monsterAiSystem;
	GatherSystem* gatherSystem;
	DeathSystem* deathSystem;
	InventoryDeathSystem* inventoryDeathSystem;

	GuiManager guiManager;

	std::string name;

	std::unique_ptr<Map> map;
	GridMap gridMap;
	std::unordered_set<pair> solidMap;

	friend class Game;
	friend class Console;
	friend class DebugScreen;
};