
#pragma once
#include "ECS.hpp"
#include "Events.hpp"
#include "GuiManager.hpp"
#include "Map.hpp"
#include "PlayerSystem.hpp"
#include "systems.hpp"
#include "utils.hpp"
#include "ParticleSystem.hpp"
#include "Realm.hpp"

using DrawQueue = std::vector<std::pair<float, DrawCall>>;

class World {
public:
	World(std::string name);
	World(std::fstream& stream);
	
	void update(uint dt);
	bool handleEvent(InputEvent event);
	void draw();
	
	void serialise(std::fstream& stream);
	void deserialise(std::fstream& stream);

	std::bitset<size_t(InputStateId::MAX)> inputState;

	Entity player = 0;
	Entity camera = 0;

	ECS ecs;
	ParticleSystem particleSystem = ParticleSystem(1000);

	std::unique_ptr<Realm> realm;
	void link(Entity entity);

private:
	void rosterComponents();
	void rosterSystems();

	void init();

	std::unique_ptr<GuiElement> makeInventory();
	std::unique_ptr<GuiElement> makeMenu();

	std::unordered_map<pair, std::set<Entity>> chunks;

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
	ParticleEmitSystem* particleEmitSystem;
	CreatureParticleSystem* creatureParticleSystem;
	HandRenderSystem* handRenderSystem;
	GridDeathSystem* gridDeathSystem;
	ChunkSystem* chunkSystem;

	GuiManager guiManager;
	std::string name;
	uint ticks;

	friend class Game;
	friend class Console;
	friend class DebugScreen;
	friend class EntityFactory;
};