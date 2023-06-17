
#pragma once
#include "utils.hpp"
#include "ECS_types.hpp"
#include "Sprite.hpp"
#include "Item.hpp"
#include "ItemTemplates.hpp"
#include "Loot.hpp"
#include "ParticleSystem.hpp"
#include "SoundManager.hpp"

struct PositionComponent {
	vec position;
	pair chunk = {0,0};
};

struct SpriteComponent {
	SpriteStack spriteStack;
	float scale = 1;
	float z = 0;
	float angle = 0;
	std::array<std::pair<bool, uint>, SpriteEffectId::count> effects = {};
};

struct CameraComponent {
	float zoom;
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
	uint lastChange;
};

struct MovementComponent {
	float speed;
};

struct ControllerComponent {

};

struct DirectionComponent {
	Direction::value direction;
};

struct ColliderComponent {
	Collider collider;
};

struct ItemComponent {
	ItemId::value itemId;
	uint count;
};

struct AnimalAiComponent {
	uint nextChange;
};

struct MonsterAiComponent {
	
};

struct InventoryComponent {
	Inventory inventory;
};

struct HealthComponent {
	uchar health;
	uchar maxHealth;
	uint lastDamage;
};

struct PlayerComponent {
	Inventory hotbar;
	Inventory equipment;
	uchar activeSlot;
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
	pair size;
	bool solid;
	bool opaque;
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