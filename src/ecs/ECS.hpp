
#pragma once
#include "ComponentManager.hpp"
#include "ECS_types.hpp"
#include "EntityManager.hpp"
#include "SystemManager.hpp"

class ECS {
public:

	ECS() {};

	ECS(std::fstream& stream) : entityManager(stream) {}

	void deserialiseComponents(std::fstream& stream) {
		componentManager.deserialise(stream);
		for (Entity entity = 1; entity <= MAX_ENTITIES; entity++) {
			for (int comp = 1; comp < ComponentId::count; comp++) {
				if (componentManager.has(entity, ComponentId::from_int(comp))) {
					entityManager.signatures[entity].set(comp, true);
				}
			}
			systemManager.signatureChange(entity, entityManager.signatures[entity]);
		}
	}

	Entity createEntity() {
		return entityManager.createEntity();
	}

	int entityCount() {
		return entityManager.entityCount();
	}

	void destroyEntity(Entity entity) {
		entityManager.destroyEntity(entity);
		componentManager.destroyEntity(entity);
		systemManager.destroyEntity(entity);
	}

	void serialise(std::fstream& stream) {
		entityManager.serialise(stream);
		componentManager.serialise(stream);
	}

	template <typename T>
	ComponentId::value rosterComponent(ComponentId::value type = ComponentId::NONE) {
		return componentManager.roster<T>(type);
	}

	template <typename T>
	void addComponent(T component, Entity entity) {
		if (!entity) return;
		componentManager.add<T>(component, entity);
		auto signature = entityManager.signatures[entity];
		signature.set(componentManager.roster<T>(), true);
		entityManager.signatures[entity] = signature;
		systemManager.signatureChange(entity, signature);
	}

	template <typename T>
	void removeComponent(Entity entity) {
		componentManager.remove<T>(entity);
		auto signature = entityManager.signatures[entity];
		signature.set(size_t(componentManager.roster<T>()), false);
		entityManager.signatures[entity] = signature;
		systemManager.signatureChange(entity, signature);
	}

	template <typename T>
	bool hasComponent(Entity entity) {
		if (!entity) return false;
		return componentManager.has<T>(entity);
	}

	template <typename T>
	T& getComponent(Entity entity) {
		return componentManager.get<T>(entity);
	}

	template <typename T>
	T* rosterSystem(SystemId::value id, std::vector<ComponentId::value>&& ids) {
		T* system = systemManager.roster<T>(id, makeSiganture(std::move(ids)));
		system->ecs = this;
		return system;
	}

	static Signature makeSiganture(std::vector<ComponentId::value>&& ids) {
		Signature signature;
		for (ComponentId::value id : ids) signature.set(size_t(id), true);
		return signature;
	}

	void update() {}


private:
	EntityManager entityManager;
	ComponentManager componentManager;
	SystemManager systemManager;
};