
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
	// pair chunk;
};

struct SpriteComponent {
	SpriteStack spriteStack;
	float scale = 1;
	float z = 0;
	float angle = 0;
	std::array<std::pair<bool, uint>, SpriteEffectId::count> effects = {};
};

struct MovementComponent {
	float walkingSpeed;
	float runningSpeed;
	MovementState::value movementState = MovementState::IDLE;
	Direction::value facing = Direction::EAST;
	uint movementStart = 0;
	float speedBonus = 0;
};

struct ActionComponent {
	ActionState::value actionState = ActionState::IDLE;
	Entity item = 0;
	vec position = {0, 0};
	uint start = 0;
	uint trigger = 0;
	uint end = 0;
};

struct CreatureAnimationComponent {
	std::array<std::pair<SpriteStack, SpriteStack>, MovementState::count> sprites;
};

struct ControllerComponent {};

struct DirectionComponent {
	Direction::value direction;
};

struct ColliderComponent {
	Shape collider;
};

struct HitboxComponent {
	Shape hitbox;
};

struct ItemComponent {
	ItemId::value itemId;
	uint count = 1;
	bool show = false;
};

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

struct MeleeItemComponent {
	uchar damage;
};

struct ForceComponent {
	vec force = {};
};

struct GridComponent {
	pair anker;
	RealmId realmId;
	pair size = {1, 1};
	bool solid = true;
	bool opaque = true;
};

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

struct DeathComponent {};

struct ParticleComponent {
	std::array<ParticleEmitter, ParticleId::count> emitters;
};

struct LightComponent {
	bool active = true;
	float intensity = 3;
	Colour tint = {255, 255, 255, 255};
	float flickerSpeed = 1;
	float flickerAmplitude = 0;
};

struct TagComponent {
	std::bitset<EntityTag::count> tags;
};

struct SensorComponent {
	int radius;
	EntityTag::value tag;
	bool engaged = false;
	vec position = {0, 0};
	uint lastSeen = 0;
	Entity item = 0;
};

struct ProjectileComponent {
	vec speed;
	bool grounded = false;
};

#define WIND_UP_COUNT 3

struct WindUpComponent {
	SpriteStack sprites[WIND_UP_COUNT];
	float chargeTime = 2000;
	float charge = 0;
};

struct LauncherComponent {
	float force = 0.75f;
	ProjectileId::value projectileId = ProjectileId::ARROW;
};

// struct ChunkComponent {};

struct AiComponent {
	AiState::value state = AiState::NONE;
	bool change = false;
	uint lastChange = 0;
	std::array<ushort, AiState::count> scores = {};
};

// struct AiMoveComponent {
// 	bool move = false;
// 	std::array<float, Direction::count> moveScores = {};
// };

struct AiWanderComponent {
	vec position;
	vec direction;
};

struct AiFleeComponent {
	ushort baseScore;
	vec avoid;
};

struct AiChaseComponent {
	vec target;
};

struct AiMeleeComponent {
	int damage;
	uint cooldown;
	uint lastHit;
	Entity item;
};

struct AiPostComponent {
	vec position;
	uint range;
};

struct PortalComponent {
	RealmId realmId;
	pair position;
};

struct MaturityComponent {
	uint birth;
	uint delay;
	ushort maxMaturity = 4;
	ushort maturity = 0;
};

struct DamageComponent {
	int damage = 0;
	uint startTime = 0;
	uint duration = 0;
	vec force = {0, 0};
	Entity imune = 0;
};

struct DurabilityComponent {
	ushort durability;
	ushort maxDurability;
};

struct VanityComponent {
	SpriteStack spriteStack;
};

struct EffectInstance {
	uint level = 1;
	uint end = 0;
	uint lastEvent = 0;
};

struct EffectComponent {
	std::bitset<Effect::count> applicable = {};
	std::array<EffectInstance, Effect::count> effects = {};
};

struct ExplosiveComponent {
	uint trigger = 0;
};

ENUM(ItemModId,
SPEED
)

struct ItemModComponent {
	std::array<int8_t, ItemModId::count> mods;
};

struct HandComponent {
	Entity item = 0;
};

struct ProcessingComponent {
	float process = 0;
	ItemContainer input;
	ItemContainer output;
	std::array<ItemId::value, ItemId::count> map;
};

struct FuelComponent {
	SpriteStack coldSprites;
	SpriteStack hotSprites;

	float heat = 0;
	float targetHeat = 0;

	bool burning = false;

	uint start;
	uint end;
	float progress = 0;

	bool light = false;
	ParticleId::value particle;

	ItemContainer input;
};

struct CraftingComponent {
	float progress = 0;
	std::array<ItemContainer, 3> inputs;
	ItemContainer output;
};

struct TankComponent {
	ushort capacity;
	SpriteStack emptySprite;
	SpriteStack fullSprite;

	ushort content = 0;
	Liquid::value liquid = Liquid::NONE;
};

struct SpawnerComponent {
	EnemyId::value enemyId;
	uint lastSpawn = 0;
	ushort radius = 5;
	uint dealy = 5000;
	ParticleId::value particleId = ParticleId::SMOKE;
};

struct AiLureComponent {
	ItemId::value itemId;
	vec target;
};