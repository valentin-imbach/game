
#pragma once
#include "ComponentArray.hpp"
#include "utils.hpp"
#include <typeinfo>

class ComponentManager {
public:
	template <typename T>
	ComponentId::value roster(ComponentId::value id = ComponentId::NONE) {
		static ComponentId::value componentId = id;
		if (id) componentArrays[id] = std::make_unique<ComponentArray<T>>();
		return componentId;
	}

	template <typename T>
	bool has(Entity entity) {
		return getComponentArray<T>()->has(entity);
	}

	bool has(Entity entity, ComponentId::value comp) {
		return componentArrays[comp]->has(entity);
	}

	template <typename T>
	void add(T component, Entity entity) {
		getComponentArray<T>()->add(entity, component);
	}

	template <typename T>
	void remove(Entity entity) {
		getComponentArray<T>()->remove(entity);
	}

	template <typename T>
	T& get(Entity entity) {
		if (!getComponentArray<T>()->has(entity)) {
			ERROR("Trying to access non-existent component of type", ComponentId::strings[roster<T>()]);
		}
		return getComponentArray<T>()->get(entity);
	}

	void destroyEntity(Entity entity) {
		for (int i = 1; i < ComponentId::count; i++) componentArrays[i]->destroyEntity(entity);
	}

	// void serialise(std::fstream& stream) {
	// 	for (int i = 1; i < ComponentId::count; i++) componentArrays[i]->serialise(stream);
	// 	LOG("Component Manager serialised")
	// }

	// void deserialise(std::fstream& stream) {
	// 	for (int i = 1; i < ComponentId::count; i++) componentArrays[i]->deserialise(stream);
	// 	LOG("Component Manager deserialised")
	// }

private:
	std::array<std::unique_ptr<IComponentArray>, ComponentId::count> componentArrays = {};

	template <typename T>
	ComponentArray<T>* getComponentArray() {
		return static_cast<ComponentArray<T>*>(componentArrays[roster<T>()].get());
	}
};