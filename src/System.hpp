
#pragma once
#include "ComponentManager.hpp"
#include "ECS_types.hpp"
#include "Sprite.hpp"
#include "utils/utils.hpp"

class DrawCall;

class System {
public:
	ComponentManager* componentManager = nullptr;
	std::set<Entity> entities;
};