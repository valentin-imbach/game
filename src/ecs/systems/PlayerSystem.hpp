
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"

class PlayerSystem : public System {
public:
	Entity getPlayer() {
		if (entities.empty()) return 0;
		return *entities.begin();
	}
};