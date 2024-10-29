
#pragma once
#include "ECS_types.hpp"
#include "utils.hpp"

class IComponentArray {
public:
	virtual ~IComponentArray() = default;
	virtual void destroyEntity(Entity entity) = 0;
	// virtual void serialise(std::fstream& stream) = 0;
	// virtual void deserialise(std::fstream& stream) = 0;

	bool has(Entity entity) {
		return (entityToIndex.find(entity)!= entityToIndex.end());
	}

protected:
	std::unordered_map<Entity, uint> entityToIndex;
	std::unordered_map<uint, Entity> indexToEntity;
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

		uint size = components.size();
		entityToIndex[entity] = size;
		indexToEntity[size] = entity;
		components.push_back(component);

		if (start) start(entity, component);
	}

	void removeComponent(Entity entity) {
		if (!hasComponent(entity)) {
			WARNING("Trying to remove non-existent component");
			return;
		}

		uint index = entityToIndex[entity];
		if (end) end(entity, components[index]);

		uint size = components.size();
		components[index] = components[size - 1];

		Entity lastEntity = indexToEntity[size - 1];
		entityToIndex[lastEntity] = index;
		indexToEntity[index] = lastEntity;

		entityToIndex.erase(entity);
		indexToEntity.erase(size - 1);
		components.pop_back();
	}

	T& getComponent(Entity entity) {
		return components[entityToIndex[entity]];
	}

	void destroyEntity(Entity entity) override {
		if (hasComponent(entity)) removeComponent(entity);
	}

	// void serialise(std::fstream& stream) override {
	// 	uint size = components.size();
	// 	serialise_object(stream, size);
	// 	for (int index = 0; index < size; index++) {
	// 		Entity entity = indexToEntity[index];
	// 		serialise_object(stream, entity);
	// 		serialise_object(stream, components[index]);
	// 	}
	// }

	// void deserialise(std::fstream& stream) override {
	// 	entityToIndex.clear();
	// 	indexToEntity.clear();
	// 	components.clear();
	// 	uint size = components.size();
	// 	deserialise_object(stream, size);
	// 	for (int index = 0; index < size; index++) {
	// 		Entity entity;
	// 		deserialise_object(stream, entity);
	// 		indexToEntity[index] = entity;
	// 		entityToIndex[entity] = index;
	// 		components.emplace_back();
	// 		deserialise_object(stream, components[index]);
	// 		if (start) start(entity, components[index]);
	// 	}
	// }

	void setCallbacks(std::function<void(Entity, T&)> start, std::function<void(Entity, T&)> end) {
		this->start = start;
		this->end = end;
	}

private:
	std::vector<T> components;
	std::function<void(Entity, T&)> start;
	std::function<void(Entity, T&)> end;
};