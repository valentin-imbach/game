
#pragma once
#include "ECS_types.hpp"
#include "utils.hpp"

class ECS;

class System {
public:
	ECS* ecs;
	EntitySet entities;
};