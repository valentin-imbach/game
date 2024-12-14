
#pragma once
#include "ECS_types.hpp"
#include "utils.hpp"

class IComponentArray {
public:
	virtual ~IComponentArray() = default;
	virtual void remove(Entity entity) = 0;
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
	void add(Entity entity, T component) {
		if (has(entity)) ERROR("Component added to same entity more than once");
		uint size = components.size();
		entityToIndex[entity] = size;
		indexToEntity[size] = entity;
		components.push_back(component);
	}

	void remove(Entity entity) {
		if (!has(entity)) return;

		uint index = entityToIndex[entity];

		uint size = components.size();
		components[index] = components[size - 1];

		Entity lastEntity = indexToEntity[size - 1];
		entityToIndex[lastEntity] = index;
		indexToEntity[index] = lastEntity;

		entityToIndex.erase(entity);
		indexToEntity.erase(size - 1);
		components.pop_back();
	}

	T& get(Entity entity) {
		return components[entityToIndex[entity]];
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


private:
	std::vector<T> components;
};