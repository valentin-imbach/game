
#pragma once
#include "ComponentArray.hpp"
#include "utils.hpp"
#include <typeinfo>

class ComponentManager {
public:
	template <typename T>
	ComponentId::value roster(ComponentId::value type = ComponentId::NONE) {
		bool rostered = (rosteredSet.find(&typeid(T)) != rosteredSet.end());
		static ComponentId::value s_type = ComponentId::NONE;
		if (type == ComponentId::NONE) {
			if (!rostered) ERROR("Using non-registered component");
			return rosteredSet[&typeid(T)];
		}
		if (rostered) WARNING("Component registered twice");
		componentArrays[type] = std::make_unique<ComponentArray<T>>();
		return rosteredSet[&typeid(T)] = type;
	}

	template <typename T>
	void setCallbacks(std::function<void(Entity, T&)> start, std::function<void(Entity, T&)> end) {
		getComponentArray<T>()->setCallbacks(start, end);
	}

	template <typename T>
	bool has(Entity entity) {
		return getComponentArray<T>()->hasComponent(entity);
	}

	bool has(Entity entity, ComponentId::value comp) {
		return componentArrays[comp]->has(entity);
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
	T& get(Entity entity) {
		if (!getComponentArray<T>()->hasComponent(entity)) {
			ERROR("Trying to access non-existent component of type", ComponentId::strings[roster<T>()]);
		}
		return getComponentArray<T>()->getComponent(entity);
	}

	void destroyEntity(Entity entity) {
		for (int i = 1; i < ComponentId::count; i++) componentArrays[i]->destroyEntity(entity);
	}

	void serialise(std::fstream& stream) {
		for (int i = 1; i < ComponentId::count; i++) componentArrays[i]->serialise(stream);
		LOG("Component Manager serialised")
	}

	void deserialise(std::fstream& stream) {
		for (int i = 1; i < ComponentId::count; i++) componentArrays[i]->deserialise(stream);
		LOG("Component Manager deserialised")
	}

private:
	std::array<std::unique_ptr<IComponentArray>, ComponentId::count> componentArrays = {};
	std::unordered_map<const std::type_info*, ComponentId::value> rosteredSet;

	template <typename T>
	ComponentArray<T>* getComponentArray() {
		return static_cast<ComponentArray<T>*>(componentArrays[roster<T>()].get());
	}
};