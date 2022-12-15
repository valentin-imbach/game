
#pragma once
#include "ComponentArray.hpp"
#include "utils.hpp"

class ComponentManager {
public:
	template <typename T>
	ComponentId::value roster(ComponentId::value type = ComponentId::NONE) {
		static ComponentId::value s_type = ComponentId::NONE;
		if (type == ComponentId::NONE) {
			if (s_type == ComponentId::NONE)
				ERROR("Using non-registered component");
			return s_type;
		}
		if (s_type != ComponentId::NONE) WARNING("Component registered twice");
		componentArrays[size_t(type)] = std::make_unique<ComponentArray<T>>();
		return s_type = type;
	}

	template <typename T>
	bool has(Entity entity) {
		return getComponentArray<T>()->hasComponent(entity);
	}

	template <typename T>
	void add(T component, Entity entity) {
		getComponentArray<T>()->addComponent(entity, component);
	}

	template <typename T>
	void remove(Entity entity) {
		getComponentArray<T>()->removeComponent(entity);
	}

	template <typename T>
	T &get(Entity entity) {
		return getComponentArray<T>()->getComponent(entity);
	}

	void destroyEntity(Entity entity) {
		for (int i = 1; i < ComponentId::count; i++) {
			componentArrays[i]->destroyEntity(entity);
		}
	}

private:
	std::array<std::unique_ptr<IComponentArray>, size_t(ComponentId::MAX)>
	    componentArrays = {};

	template <typename T>
	ComponentArray<T> *getComponentArray() {
		return static_cast<ComponentArray<T> *>(
		    componentArrays[size_t(roster<T>())].get());
	}
};