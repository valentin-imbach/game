
#pragma once
#include "utils.hpp"
#include "systems_fwd.hpp"
#include "ECS.hpp"

#include "RealmManager.hpp"
#include "GuiManager.hpp"
#include "ParticleSystem.hpp"

#include "Camera.hpp"
#include "Minimap.hpp"
#include "Time.hpp"

class World {
public:
	World(std::string name, uint seed, bool debug = false);
	World(std::fstream& stream);

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
	Entity inspect = 0;
	Entity hover = 0;
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
	CreatureActionSystem* creatureActionSystem;
	EffectSystem* effectSystem;
	ExplosiveSystem* explosiveSystem;
	ItemModSystem* itemModSystem;

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