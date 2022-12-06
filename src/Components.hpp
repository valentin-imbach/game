
#pragma once
#include "utils/utils.hpp"
#include "ECS_types.hpp"
#include "Sprite.hpp"

enum class SpriteSheet;

struct PositionComponent {
	vec position;
};

struct SpriteComponent {
	Sprite sprite;
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