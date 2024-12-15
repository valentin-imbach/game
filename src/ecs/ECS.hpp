
#pragma once
#include "ECS_types.hpp"
#include "ComponentArray.hpp"
#include "System.hpp"

class ECS {
public:
	Entity createEntity() {
		static uint s = 1;
		Entity entity = noise::UInt(s++); //TODO collision? 0?
		signatures[entity];
		entityCount += 1;
		return entity;
	}

	void destroyEntity(Entity entity) {
		if (!entity) return;
		// LOG("destroyed", entity);
		signatures.erase(entity);
		for (int i = 1; i < ComponentId::count; i++) componentArrays[i]->remove(entity);
		for (int i = 1; i < SystemId::count; i++) {
			if (!systems[i]) continue;
			systems[i]->entities.erase(entity);
		}
		entityCount -= 1;
		// activeEntityCount -= 1;
	}

	// void addEntity(Entity entity) {
	// }

	// void removeEntity(Entity entity) {
	// 	if (!entity) return;
	// 	activeEntityCount -= 1;
	// 	LOG("removed", entity);
	// 	entitySignatures.erase(entity);
	// }

	template <typename T>
	ComponentId::value rosterComponent(ComponentId::value id = ComponentId::NONE) {
		static ComponentId::value componentId = id;
		if (id) componentArrays[id] = std::make_unique<ComponentArray<T>>();
		return componentId;
	}

	template <typename T>
	void addComponent(T component, Entity entity) {
		if (!entity) return;
		ComponentId::value id = rosterComponent<T>();
		if (!id) ERROR("Trying to add unrostered component");
		static_cast<ComponentArray<T>*>(componentArrays[id].get())->add(entity, component);
		Signature& signature = signatures[entity];
		signature.set(id, true);
		signatureChange(entity, signature);
	}

	template <typename T>
	void removeComponent(Entity entity) {
		if (!entity) return;
		ComponentId::value id = rosterComponent<T>();
		if (!id) ERROR("Trying to remove unrostered component");
		static_cast<ComponentArray<T>*>(componentArrays[id].get())->remove(entity);
		Signature& signature = signatures[entity];
		signature.set(id, false);
		signatureChange(entity, signature);
	}

	template <typename T>
	bool hasComponent(Entity entity) {
		ComponentId::value id = rosterComponent<T>();
		return componentArrays[id]->has(entity);
	}

	template <typename T>
	T& getComponent(Entity entity) {
		ComponentId::value id = rosterComponent<T>();
		if (!id) ERROR("Trying to get unrostered component");
		return static_cast<ComponentArray<T>*>(componentArrays[id].get())->get(entity);
	}

	template <typename T>
	T* rosterSystem(SystemId::value id) {
		std::unique_ptr<T> system = std::make_unique<T>();
		system->roster();
		system->ecs = this;
		T* res = system.get();
		systems[id] = std::move(system);
		return res;
	}

	void signatureChange(Entity entity, Signature& signature) {
		for (int i = 1; i < SystemId::count; i++) {
			if (!systems[i]) continue;
			if ((signature & systems[i]->signature) == systems[i]->signature) {
				systems[i]->entities.insert(entity);
			} else {
				systems[i]->entities.erase(entity);
			}
		}
	}

private:
	std::unordered_map<Entity, Signature> signatures;
	uint entityCount = 0;
	uint activeEntityCount = 0;

	// std::array<Signature, SystemId::count> systemSignatures;
	std::array<std::unique_ptr<System>, SystemId::count> systems;

	std::array<std::unique_ptr<IComponentArray>, ComponentId::count> componentArrays;

	// template <typename T>
	// ComponentArray<T>* getComponentArray() {
	// 	return static_cast<ComponentArray<T>*>(componentArrays[roster<T>()].get());
	// }
friend class DebugScreen;
};