
#pragma once
#include "ECS_types.hpp"
#include "utils.hpp"

class EntityManager {
public:
	EntityManager() {};

	// EntityManager(std::fstream& stream) {
	// 	deserialise_object(stream, entityCount);
	// 	signatures.clear();
	// 	for (int i = 0; i < entityCount; i++) {
	// 		Entity entity;
	// 		deserialise_object(stream, entity);
	// 		signatures[entity] = {};
	// 	}
	// 	LOG("Entity manager deserialised");
	// }

	Entity createEntity() {
		static uint s = 1;
		Entity entity = noise::UInt(s++); //TODO collision? 0?
		signatures[entity] = {};
		entityCount += 1;
		return entity;
	}

	void destroyEntity(Entity entity) {
		if (!entity) return;
		LOG("destroyed", entity);
		signatures.erase(entity);
		entityCount -= 1;
		activeEntityCount -= 1;
	}

	void addEntity(Entity entity) {
		//TODO
	}

	void removeEntity(Entity entity) {
		if (!entity) return;
		activeEntityCount -= 1;
		LOG("removed", entity);
		signatures.erase(entity);
	}

	// void serialise(std::fstream& stream) {
	// 	serialise_object(stream, entityCount);
	// 	for (auto& iter : signatures) {
	// 		Entity entity = iter.first;
	// 		serialise_object(stream, entity);
	// 	}
	// 	LOG("Entity Manager serialised");
	// }

	// void deserialise(std::fstream& stream) {
	// 	deserialise_object(stream, entityCount);
	// 	signatures.clear();
	// 	for (int i = 0; i < entityCount; i++) {
	// 		Entity entity;
	// 		deserialise_object(stream, entity);
	// 		signatures[entity] = {};
	// 	}
	// 	LOG("Entity manager deserialised");
	// }

private:
	std::unordered_map<Entity, Signature> signatures;
	uint entityCount = 0;
	uint activeEntityCount = 0;

friend class ECS;
};