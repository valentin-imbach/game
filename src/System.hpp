
#pragma once
#include "ComponentManager.hpp"
#include "ECS_types.hpp"
#include "utils/utils.hpp"

class System {
public:
	ComponentManager* componentManager = nullptr;
	std::set<Entity> entities;
};