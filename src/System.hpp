
#pragma once
#include "utils/utils.hpp"
#include "ECS_types.hpp"
#include "ComponentManager.hpp"

class System {
public:
    ComponentManager* componentManager = nullptr;
    std::set<Entity> entities;
};