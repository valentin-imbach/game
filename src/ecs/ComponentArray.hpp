
#pragma once
#include "ECS_types.hpp"
#include "utils.hpp"

class IComponentArray {
public:
	virtual ~IComponentArray() = default;
	virtual void destroyEntity(Entity entity) = 0;
};

template <typename T>
class ComponentArray : public IComponentArray {
public:
	bool hasComponent(Entity entity) {
		return (entityToIndex.find(entity) != entityToIndex.end());
	}

	void addComponent(Entity entity, T component) {
		if (hasComponent(entity)) {
			WARNING("Component added to same entity more than once");
			return;
		}

		entityToIndex[entity] = size;
		indexToEntity[size] = entity;
		components[size] = component;
		size += 1;
	}

	void removeComponent(Entity entity) {
		if (!hasComponent(entity)) {
			WARNING("Trying to removing non-existent component");
			return;
		}

		size_t index = entityToIndex[entity];
		components[index] = components[size - 1];

		Entity lastEntity = indexToEntity[size - 1];
		entityToIndex[lastEntity] = index;
		indexToEntity[index] = lastEntity;

		entityToIndex.erase(entity);
		indexToEntity.erase(size - 1);
		size -= 1;
	}

	T& getComponent(Entity entity) {
		if (!hasComponent(entity)) {
			ERROR("Trying to access non-existent component");
			exit(EXIT_FAILURE);
		}
		return components[entityToIndex[entity]];
	}

	void destroyEntity(Entity entity) override {
		if (hasComponent(entity)) removeComponent(entity);
	}

private:
	std::array<T, MAX_ENTITIES> components;
	std::unordered_map<Entity, size_t> entityToIndex;
	std::unordered_map<size_t, Entity> indexToEntity;
	size_t size;
};