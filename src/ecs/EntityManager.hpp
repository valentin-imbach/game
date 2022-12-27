
#pragma once
#include "ECS_types.hpp"
#include "utils.hpp"

class EntityManager {
public:
	EntityManager() {
		availableEntities.reserve(MAX_ENTITIES);
		for (int entity = MAX_ENTITIES; entity >= 1; entity--) {
			availableEntities.push_back(entity);
		}
	}

	EntityManager(std::fstream& stream) {
		size_t count;
		stream.read((char*)&count, sizeof(count));
		availableEntities.reserve(count);
		for (int i = 0; i < count; i++) {
			Entity entity;
			stream.read((char*)&entity, sizeof(entity));
			availableEntities.push_back(entity);
		}
		LOG("Entity manager deserialised");
	}

	Entity createEntity() {
		if (availableEntities.empty()) {
			WARNING("Entity limit reached");
			return 0;
		}
		Entity entity = availableEntities.back();
		availableEntities.pop_back();
		return entity;
	}

	void destroyEntity(Entity entity) {
		if (!entity) return;
		LOG("destroyed", entity);
		signatures[entity].reset();
		availableEntities.push_back(entity);
	}

	void serialise(std::fstream& stream) {
		size_t count = availableEntities.size();
		stream.write((char*)&count, sizeof(count));
		for (Entity entity: availableEntities) {
			stream.write((char*)&entity, sizeof(entity));
		}
		LOG("Entity Manager serialised");
	}

	int entityCount() { return MAX_ENTITIES - availableEntities.size(); }

	std::array<Signature, MAX_ENTITIES + 1> signatures;

private:
	std::vector<Entity> availableEntities;
};