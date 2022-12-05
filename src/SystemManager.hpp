
#pragma once 
#include <unordered_map>
#include "SystemId.hpp"
#include "EntityManager.hpp"
#include "System.hpp"
#include "utils/logger.hpp"

class SystemManager {
public:
    template <typename T>
    T* roster(SystemId id, Signature signature) {
        signatures[size_t(id)] = signature;
        std::unique_ptr<T> system = std::make_unique<T>();
        T* ret = system.get();
        systems[size_t(id)] = std::move(system);
        return ret;
    }

    void destroyEntity(Entity entity) {
        for (int i = 1; i < int(SystemId::MAX); i++) {
            systems[i] -> entities.erase(entity);
        }
    }

    void signatureChange(Entity entity, Signature signature) {
		for (int i = 1; i < int(SystemId::MAX); i++) {
			if ((signature & signatures[i]) == signatures[i]) {
                systems[i] -> entities.insert(entity);
			} else {
				systems[i] -> entities.erase(entity);
			}
		}
	}

private:
    std::array<Signature, size_t(SystemId::MAX)> signatures = {};
	std::array<std::unique_ptr<System>, size_t(SystemId::MAX)> systems = {};
};