
#include "EntityManager.hpp"
#include "utils/logger.hpp"

EntityManager::EntityManager() {
    for (Entity entity = 1; entity <= MAX_ENTITIES; entity += 1) {
        availableEntities.push(entity);
    }
}

Entity EntityManager::createEntity() {
    if (availableEntities.empty()) {
        WARNING("Entity limit reached");
        return 0;
    }
    Entity entity = availableEntities.front();
    availableEntities.pop();
    return entity;
};

void EntityManager::destroyEntity(Entity entity) {
    if (!entity) return;
    signatures[entity].reset();
    availableEntities.push(entity);
}

Entity EntityManager::entityCount() {
    return MAX_ENTITIES - availableEntities.size();
}