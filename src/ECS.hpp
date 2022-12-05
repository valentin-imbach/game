
#pragma once
#include "EntityManager.hpp"
#include "ComponentManager.hpp"
#include "SystemManager.hpp"

class ECS {
public:
    Entity createEntity() {
		return entityManager.createEntity();
	}

    void destroyEntity(Entity entity) {
		entityManager.destroyEntity(entity);
		componentManager.destroyEntity(entity);
		systemManager.destroyEntity(entity);
	}

    template<typename T>
	ComponentId rosterComponent(ComponentId type = ComponentId::NONE) {
		return componentManager.roster<T>(type);
	}

    template<typename T>
	void addComponent(T component, Entity entity) {
		componentManager.add<T>(component, entity);
		auto signature = entityManager.signatures[entity];
		signature.set(size_t(componentManager.roster<T>()), true);
		entityManager.signatures[entity] = signature;
		systemManager.signatureChange(entity, signature);
	}

	template<typename T>
	void removeComponent(Entity entity) {
		componentManager.remove<T>(entity);
		auto signature = entityManager.signatures[entity];
		signature.set(size_t(componentManager.roster<T>()), false);
		entityManager.signatures[entity] = signature;
		systemManager.signatureChange(entity, signature);
	}

	template<typename T>
	T& getComponent(Entity entity) {
		return componentManager.get<T>(entity);
	}

    template<typename T>
	T* rosterSystem(SystemId id, Signature signature) {
		return systemManager.roster<T>(id, signature);
	}

    static Signature makeSiganture(std::vector<ComponentId>&& ids) {
        Signature signature;
        for (ComponentId id : ids) {
            signature.set(size_t(id), true);
        }
        return signature;
    }

//private:
	EntityManager entityManager;
    ComponentManager componentManager;
	SystemManager systemManager;
};