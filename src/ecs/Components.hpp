
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

enum class CreatureState : uint8_t {
	IDLE,
	WALKING,
	RUNNING,
	SITTING,
	EATING,
	JUMPING
};

struct CreatureStateComponent {
	CreatureState state;
	Direction facing;
	bool stateChanged;
};

struct MovementComponent {
	float speed;
};

struct ControllerComponent {

};

struct DirectionComponent {
	Direction direction;
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
};

struct InventoryComponent {
	Inventory inventory;
};

struct HealthComponent {
	uint health;
	uint maxHealth;
};

struct PlayerComponent {
	uint activeSlot;
};

struct ResourceComponent {
	ToolId toolId;
};

struct LootComponent {
	Item loot;
};

struct ToolComponent {
	ToolId toolId;
};

struct DamageComponent {
	uint8_t damage;
};