
#pragma once
#include "ECS_types.hpp"
#include "utils/utils.hpp"

class IComponentArray {
public:
	virtual ~IComponentArray() = default;
	virtual void destroyEntity(Entity entity) = 0;
};

template <typename T>
class ComponentArray : public IComponentArray {
public:
	void addComponent(Entity entity, T component) {
		if (entityToIndex.find(entity) != entityToIndex.end()) {
			WARNING("Component added to same entity more than once");
			return;
		}

		entityToIndex[entity] = size;
		indexToEntity[size] = entity;
		components[size] = component;
		size += 1;
	}

	void removeComponent(Entity entity) {
		if (entityToIndex.find(entity) == entityToIndex.end()) {
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
		if (entityToIndex.find(entity) == entityToIndex.end()) {
			ERROR("Trying to access non-existent component");
			return components[0];
		}
		return components[entityToIndex[entity]];
	}

	void destroyEntity(Entity entity) override {
		if (entityToIndex.find(entity) != entityToIndex.end())
			removeComponent(entity);
	}

private:
	std::array<T, MAX_ENTITIES> components;
	std::unordered_map<Entity, size_t> entityToIndex;
	std::unordered_map<size_t, Entity> indexToEntity;
	size_t size;
};