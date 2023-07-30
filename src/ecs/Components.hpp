
#pragma once
#include "utils.hpp"
#include "ECS_types.hpp"
#include "Sprite.hpp"
#include "Item.hpp"
#include "ItemTemplates.hpp"
#include "Loot.hpp"
#include "ParticleSystem.hpp"
#include "SoundManager.hpp"
#include "RealmManager.hpp"

struct PositionComponent {
	vec position;
	RealmId realmId;
	pair chunk;
};

struct SpriteComponent {
	SpriteStack spriteStack;
	float scale = 1;
	float z = 0;
	float angle = 0;
	std::array<std::pair<bool, uint>, SpriteEffectId::count> effects = {};
};

ENUM(CreatureState,
IDLE,
WALKING,
RUNNING,
SITTING,
EATING,
JUMPING)

struct CreatureStateComponent {
	CreatureState::value state;
	Direction::value facing;
	uint lastChange = 0;
};

struct MovementComponent {
	float speed;
};

struct ControllerComponent {};

struct DirectionComponent {
	Direction::value direction;
};

struct ColliderComponent {
	Collider collider;
};

struct ItemComponent {
	ItemId::value itemId;
	uint count = 1;
};

struct AnimalAiComponent {
	uint nextChange;
};

struct MonsterAiComponent {};

struct InventoryComponent {
	Inventory inventory;
};

struct HealthComponent {
	uchar health;
	uchar maxHealth;
	uint lastDamage = 0;
};

struct PlayerComponent {
	Inventory hotbar;
	Inventory equipment;
	uchar activeSlot = 0;
	uint lastAction = 0;
};

struct ResourceComponent {
	ItemKind::value toolId;
	SoundId::value soundId;
	int level;
};

struct LootComponent {
	LootTable lootTable;
};

struct ItemKindComponent {
	std::bitset<ItemKind::count> itemKinds;
	std::array<uchar, ItemProperty::count> itemProperties;
};

struct DamageComponent {
	uchar damage;
};

struct ForceComponent {
	vec force;
};

struct GridComponent {
	pair anker;
	RealmId realmId;
	pair size = {1, 1};
	bool solid = true;
	bool opaque = true;
};

ENUM(StationId,
ANVIL,
FURNACE,
TOOL_STATION,
WORK_STATION,
SAWMILL,
CHEST,
CAMP_FIRE)

struct StationComponent {
	StationId::value stationId;
};

struct Textblock {
	uchar size;
	std::array<char, 20> chars;
	Textblock(std::string text = "") : size(text.size()), chars() {
		assert(size <= 20);
		for (int i = 0; i < size; i++) chars[i] = text[i]; 
	}

	std::string get() {
		std::string text;
		for (int i = 0; i < size; i++) text += chars[i];
		return text;
	}
};

struct NameComponent {
	Textblock name;
};

struct GatherComponent {};

struct DeathComponent {};

struct ParticleComponent {
	ParticleStyle style;
	uint lastEmit = 0;
	uint cooldown = 300;
	bool active = true;
};

struct LightComponent {
	bool active = true;
	float intensity = 3;
	SDL_Color tint = {255, 255, 255, 255};
	float flickerSpeed = 1;
	float flickerAmplitude = 0;
};

struct SensorComponent {
	int radius;
	bool engaged = false;
	vec position = {0, 0};
	uint lastSeen = 0;
};

struct ProjectileComponent {
	vec speed;
	bool grounded = false;
};

struct LauncherComponent {
	float maxForce = 0.75f;
	float minForce = 0.3f;
	float chargeTime = 2000;
	float charge = 0;
};

struct ChunkComponent {};

ENUM(AiState,
WANDER,
CHASE,
MELEE,
RANGED,
FLEE,
EAT);

struct AiComponent {
	AiState::value state = AiState::NONE;
	uint lastChange = 0;
	bool change = false;
	std::array<ushort, AiState::count> scores = {};
};

struct AiMoveComponent {
	bool move = false;
	std::array<float, Direction::count> moveScores = {};
};

struct AiWanderComponent {
	vec position;
	vec direction;
};

struct AiFleeComponent {
	vec avoid;
	vec target;
	uint lastChange;
};

struct AiChaseComponent {
	vec target;
};

struct PortalComponent {
	RealmId realmId;
	pair position;
};