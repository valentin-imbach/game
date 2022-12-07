
#pragma once
#include "utils/utils.hpp"
#include "ECS_types.hpp"
#include "Sprite.hpp"
#include "Collider.hpp"

struct PositionComponent {
	vec position;
};

struct SpriteComponent {
	SpriteStack spriteStack;
	uint8_t height;
};

struct CameraComponent {
	uint8_t zoom;
	Entity target;
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