
#pragma once
#include "ECS_types.hpp"
#include "utils/utils.hpp"
#include "Components.hpp"

class ECS;

class System {
public:
	ECS* ecs;
	std::set<Entity> entities;
};