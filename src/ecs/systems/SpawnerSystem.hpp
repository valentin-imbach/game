
#pragma once
#include "System.hpp"
#include "Components.hpp"
#include "ECS.hpp"
#include "EntityFactory.hpp"

class SpawnerSystem : public System {

	ROSTER(SPAWNER, POSITION)

	void update(uint ticks, EntitySet& set, ParticleSystem& particleSystem) {
		for (Entity entity : set) {
			if (entities.find(entity) == entities.end()) continue;
			SpawnerComponent& spawnerComponent = ecs->getComponent<SpawnerComponent>(entity);
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);
			uint past = ticks - spawnerComponent.lastSpawn;
			if (past > spawnerComponent.dealy) {
				float ang = noise::Float(ticks, 2*M_PI);
				float rad =  noise::Float(ticks + 1, spawnerComponent.radius);
				vec pos = positionComponent.position + vec::polar(ang, rad);
				EntityFactory::createEnemy(spawnerComponent.enemyId, positionComponent.realmId, pos);
				spawnerComponent.lastSpawn = ticks;
				for (int i = 0; i < 5; i++) {
					particleSystem.emit(ParticleStyle::templates[spawnerComponent.particleId], pos, positionComponent.realmId);
					particleSystem.emit(ParticleStyle::templates[ParticleId::FIRE], positionComponent.position - pair(0,1), positionComponent.realmId);
				}
			}
		}
	}

	// void changeSprite(Entity entity, ushort maturity) {
	// 	if (!ecs->hasComponent<SpriteComponent>(entity)) return;
	// 	SpriteComponent& spriteComponent = ecs->getComponent<SpriteComponent>(entity);
	// 	for (auto& layer : spriteComponent.spriteStack.stack) {
	// 		layer.sprite.source.x += layer.sprite.size.x * layer.sprite.frameCount;
	// 	}
		
	// }
};