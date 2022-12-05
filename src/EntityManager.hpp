
#pragma once
#include <cstdint>
#include <queue>
#include <bitset>
#include <array>
#include "ComponentId.hpp"

typedef std::uint32_t Entity;
constexpr Entity MAX_ENTITIES = 100;

using Signature = std::bitset<size_t(ComponentId::MAX)>;

class EntityManager {
public:
    EntityManager();
    Entity createEntity();
    void destroyEntity(Entity entity);
    Entity entityCount();

    std::array<Signature, MAX_ENTITIES + 1> signatures;

private:
    std::queue<Entity> availableEntities;    
};