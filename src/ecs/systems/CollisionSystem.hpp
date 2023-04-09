
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"
#include "TextureManager.hpp"

#define COLLIDER_DEBUG true

class CollisionSystem : public System {
public:
	void update(std::unordered_map<Entity,std::vector<Entity>>& collisions) {
		collisions.clear();

		std::vector<std::pair<Entity, vec>> ents;
		for (Entity entity : entities) {
			Collider& collider = ecs -> getComponent<ColliderComponent>(entity).collider;
			vec position = ecs -> getComponent<PositionComponent>(entity).position;
			ents.emplace_back(entity, collider.topBottom(position));
		}

		auto lambda = [](auto& l, auto& r) { return l.second.x < r.second.x; };
		std::sort(ents.begin(), ents.end(), lambda);

		for (int i = 0; i < ents.size(); i++) {
			Entity entityA = ents[i].first;
			vec positionA = ecs -> getComponent<PositionComponent>(entityA).position;
			Collider colliderA = ecs -> getComponent<ColliderComponent>(entityA).collider;
			for (int j = i + 1; j < ents.size(); j++) {
				Entity entityB = ents[j].first;
				vec positionB = ecs -> getComponent<PositionComponent>(entityB).position;
				Collider colliderB = ecs -> getComponent<ColliderComponent>(entityB).collider;

				if (ents[j].second.x > ents[i].second.y) break;

				if (Collider::colide(colliderA, positionA, colliderB, positionB)) {
					collisions[entityA].push_back(entityB);
					collisions[entityB].push_back(entityA);
				}
			}
		}
	}
};