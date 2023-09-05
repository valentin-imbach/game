
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"
#include "TextureManager.hpp"

class HitboxSystem : public System {
public:
	void update(std::unordered_map<Entity,std::vector<Entity>>& hits, EntitySet& set) {
		hits.clear();

		std::vector<std::pair<Entity, vec>> ents;
		for (Entity entity : set) {
			if (entities.find(entity) == entities.end()) continue;
			Shape& hitbox = ecs -> getComponent<HitboxComponent>(entity).hitbox;
			vec position = ecs -> getComponent<PositionComponent>(entity).position;
			ents.emplace_back(entity, hitbox.topBottom(position));
		}

		auto lambda = [](auto& l, auto& r) { return l.second.x < r.second.x; };
		std::sort(ents.begin(), ents.end(), lambda);

		for (int i = 0; i < ents.size(); i++) {
			Entity entityA = ents[i].first;
			vec positionA = ecs -> getComponent<PositionComponent>(entityA).position;
			Shape hitboxA = ecs -> getComponent<HitboxComponent>(entityA).hitbox;
			for (int j = i + 1; j < ents.size(); j++) {
				Entity entityB = ents[j].first;
				vec positionB = ecs -> getComponent<PositionComponent>(entityB).position;
				Shape hitboxB = ecs -> getComponent<HitboxComponent>(entityB).hitbox;

				if (ents[j].second.x > ents[i].second.y) break;

				if (Shape::colide(hitboxA, positionA, hitboxB, positionB)) {
					hits[entityA].push_back(entityB);
					hits[entityB].push_back(entityA);
				}
			}
		}
	}
};