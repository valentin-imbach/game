
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
#include "RealmManager.hpp"
#include "Minimap.hpp"
#include "Time.hpp"

class World {
public:
	World(std::string name, uint seed, bool debug = false);
	World(std::fstream& stream);

	// void linkGrid(Entity entity, GridComponent& gridComponent) {
	// 	realmManager.getRealm(gridComponent.realmId)->linkGrid(entity, gridComponent);
	// }

	// void unlinkGrid(Entity entity, GridComponent& gridComponent) {
	// 	realmManager.getRealm(gridComponent.realmId)->unlinkGrid(entity, gridComponent);
	// }

	// void linkChunk(Entity entity, PositionComponent& positionComponent) {
	// 	realmManager.getRealm(positionComponent.realmId)->linkChunk(entity, positionComponent);
	// }

	// void unlinkChunk(Entity entity, PositionComponent& positionComponent) {
	// 	realmManager.getRealm(positionComponent.realmId)->unlinkChunk(entity, positionComponent);
	// }

	void update(uint dt);
	void updateCamera(Entity target);
	bool handleEvent(InputEvent event, uint dt);
	void draw();
	void drawTiles();
	
	void serialise(std::fstream& stream);
	void deserialise(std::fstream& stream);

	std::bitset<InputStateId::count> inputState;
	bool state = false;

	Entity player = 0;
	Camera camera;

	ECS ecs;
	ParticleSystem particleSystem;

	RealmManager realmManager;
	Realm* playerRealm;

	Realm* spawnRealm;
	pair spawn;

	Time time;
	bool colliderDraw = true;

private:
	uint seed;
	void rosterComponents();
	void rosterSystems();

	void init();

	EntitySet updateSet;

	std::unique_ptr<GuiElement> makeInventory();
	std::unique_ptr<GuiElement> makeMenu();
	std::unique_ptr<GuiElement> makeDeathScreen();

	bool playerAlive = true;
	void respawn();

	std::vector<DrawCall> drawQueue;

	EntityDrawSystem* entityDrawSystem;
	CreatureMovementSystem* creatureMovementSystem;
	ControllerSystem* controllerSystem;
	CreatureAnimationSystem* creatureAnimationSystem;
	CollisionSystem* collisionSystem;
	ItemPickupSystem* itemPickupSystem;
	ForageSystem* forageSystem;
	HealthSystem* healthSystem;
	LootSystem* lootSystem;
	PlayerSystem* playerSystem;
	ColliderDrawSystem* colliderDrawSystem;
	GridSystem* gridSystem;
	InteractionSystem* interactionSystem;
	DeathSystem* deathSystem;
	InventoryDeathSystem* inventoryDeathSystem;
	ParticleEmitSystem* particleEmitSystem;
	CreatureParticleSystem* creatureParticleSystem;
	HandRenderSystem* handRenderSystem;
	ChunkSystem* chunkSystem;
	LightSystem* lightSystem;
	SensorSystem* sensorSystem;
	ProjectileSystem* projectileSystem;
	AiSystem* aiSystem;
	AiWanderSystem* aiWanderSystem;
	AiMoveSystem* aiMoveSystem;
	AiFleeSystem* aiFleeSystem;
	AiChaseSystem* aiChaseSystem;
	AiMeleeSystem* aiMeleeSystem;
	PositionSystem* positionSystem;
	MaturitySystem* maturitySystem;
	HitboxDrawSystem* hitboxDrawSystem;
	DamageSystem* damageSystem;
	HitboxSystem* hitboxSystem;

	GuiManager guiManager;
	Minimap minimap;
	std::string name;

	uint ticks;
	int tickSpeed = 1;

	pair playerChunk;

	friend class Game;
	friend class Console;
	friend class DebugScreen;
	friend class EntityFactory;
	friend class Realm;
};