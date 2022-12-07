
#pragma once
#include "ECS_types.hpp"
#include "utils/utils.hpp"
#include "Components.hpp"
#include "ComponentManager.hpp"

class System {
public:
	ComponentManager* componentManager = nullptr;
	std::set<Entity> entities;
};