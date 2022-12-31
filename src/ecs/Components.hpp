
#pragma once
#include "utils.hpp"
#include "ECS_types.hpp"
#include "Sprite.hpp"
#include "Item.hpp"
#include "ItemTemplates.hpp"
#include "Loot.hpp"

struct PositionComponent {
	vec position;
};

struct SpriteComponent {
	SpriteStack spriteStack;
	uint8_t height;
	float scale = 1;
};

struct CameraComponent {
	uint8_t zoom;
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
	bool stateChanged;
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
	uint panic;
};

struct MonsterAiComponent {
	
};

struct InventoryComponent {
	Inventory inventory;
};

struct HealthComponent {
	uint8_t health;
	uint8_t maxHealth;
	bool damaged;
};

struct PlayerComponent {
	uint8_t activeSlot;
};

struct ResourceComponent {
	ToolId::value toolId;
};

struct LootComponent {
	LootTable lootTable;
};

struct ToolComponent {
	ToolId::value toolId;
};

struct DamageComponent {
	uint8_t damage;
};

struct ForceComponent {
	vec force;
};

struct GridComponent {
	pair anker;
	pair size;
};

ENUM(InteractionId,
CHEST)

struct InteractionComponent {
	InteractionId::value interactionId;
};

struct Textblock {
	uint8_t size;
	std::array<char, 20> chars;
	Textblock(std::string text = "") : size(0), chars() {
		assert(text.size() <= 20);
		size = text.size();
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