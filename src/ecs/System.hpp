
#pragma once
#include "ECS_types.hpp"
#include "utils.hpp"

class ECS;

class System {
public:
	ECS* ecs;
	EntitySet entities;
	Signature signature;
	virtual ~System() = default;
	virtual void roster(){};
};

#define ROSTER(ids...)                  \
public:                                 \
	void roster() override {            \
		using namespace ComponentId;    \
		std::vector<value> vec = {ids}; \
		for (value id : vec) {          \
			signature.set(id, true);    \
		}                               \
	}
