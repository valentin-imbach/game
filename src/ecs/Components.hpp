
#pragma once
#include "utils.hpp"
#include "ECS_types.hpp"
#include "Sprite.hpp"
#include "Item.hpp"

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
	std::vector<Entity> collisions;
};

struct ItemComponent {
	Item item;
};

struct AnimalAiComponent {
	uint nextChange;
	uint panic;
};

struct InventoryComponent {
	Inventory inventory;
};

struct HealthComponent {
	uint health;
	uint maxHealth;
	bool damaged;
};

struct PlayerComponent {
	uint activeSlot;
};

struct ResourceComponent {
	ToolId::value toolId;
};

struct LootComponent {
	Item loot;
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