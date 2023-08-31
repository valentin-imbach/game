
#pragma once
#include "ECS_types.hpp"
#include "System.hpp"
#include "utils.hpp"

class SystemManager {
public:
	template <typename T>
	T* roster(SystemId::value id, Signature signature) {
		signatures[id] = signature;
		std::unique_ptr<T> system = std::make_unique<T>();
		T* ret = system.get();
		systems[id] = std::move(system);
		return ret;
	}

	void destroyEntity(Entity entity) {
		for (int i = 1; i < SystemId::count; i++) {
			systems[i]->entities.erase(entity);
		}
	}

	void signatureChange(Entity entity, Signature signature) {
		for (int i = 1; i < SystemId::count; i++) {
			if ((signature & signatures[i]) == signatures[i]) {
				systems[i]->entities.insert(entity);
			} else {
				systems[i]->entities.erase(entity);
			}
		}
	}

private:
	std::array<Signature, SystemId::count> signatures = {};
	std::array<std::unique_ptr<System>, SystemId::count> systems = {};
};