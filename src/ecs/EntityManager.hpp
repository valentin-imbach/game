
#pragma once
#include "ECS_types.hpp"
#include "utils.hpp"

class EntityManager {
public:
	EntityManager() {}

	EntityManager(std::fstream& stream) {
		deserialise_object(stream, entityCount);
		signatures.clear();
		for (int i = 0; i < entityCount; i++) {
			Entity entity;
			deserialise_object(stream, entity);
			signatures[entity] = {};
		}
		LOG("Entity manager deserialised");
	}

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
	}

	void serialise(std::fstream& stream) {
		serialise_object(stream, entityCount);
		for (auto& iter : signatures) {
			Entity entity = iter.first;
			serialise_object(stream, entity);
		}
		LOG("Entity Manager serialised");
	}

	void deserialise(std::fstream& stream) {
		deserialise_object(stream, entityCount);
		signatures.clear();
		for (int i = 0; i < entityCount; i++) {
			Entity entity;
			deserialise_object(stream, entity);
			signatures[entity] = {};
		}
		LOG("Entity manager deserialised");
	}

	std::unordered_map<Entity, Signature> signatures;
	uint entityCount = 0;
};