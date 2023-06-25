
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
#include "Camera.hpp"
#include "Sprite.hpp"

class World {
public:
	World(std::string name, uint seed);
	World(std::fstream& stream);

	void linkGrid(Entity entity) { realm->linkGrid(entity); }
	void unlinkGrid(Entity entity) { realm->unlinkGrid(entity); }

	void linkChunk(Entity entity) { realm->linkChunk(entity); }
	void unlinkChunk(Entity entity) { realm->unlinkChunk(entity); }
	
	void update(uint dt);
	void updateCamera(Entity target);
	bool handleEvent(InputEvent event, uint dt);
	void draw();
	void drawTiles();
	
	void serialise(std::fstream& stream);
	void deserialise(std::fstream& stream);

	std::bitset<size_t(InputStateId::MAX)> inputState;
	bool state = false;

	Entity player = 0;
	Camera camera;

	ECS ecs;
	ParticleSystem particleSystem;

	std::unique_ptr<Realm> realm;

	Time time;
	bool colliderDraw = true;

private:
	uint seed;
	void rosterComponents();
	void rosterSystems();

	void init();

	std::unique_ptr<GuiElement> makeInventory();
	std::unique_ptr<GuiElement> makeMenu();

	std::vector<DrawCall> drawQueue;

	EntityDrawSystem* entityDrawSystem;
	CreatureMovementSystem* creatureMovementSystem;
	ControllerSystem* controllerSystem;
	CreatureAnimationSystem* creatureAnimationSystem;
	CollisionSystem* collisionSystem;
	ItemPickupSystem* itemPickupSystem;
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
	ChunkSystem* chunkSystem;
	LightSystem* lightSystem;
	SensorSystem* sensorSystem;
	ProjectileSystem* projectileSystem;

	GuiManager guiManager;
	std::string name;
	uint ticks;

	pair playerChunk;

	friend class Game;
	friend class Console;
	friend class DebugScreen;
	friend class EntityFactory;
	friend class Realm;
};