
#pragma once
#include "System.hpp"
#include "ECS.hpp"
#include "Components.hpp"

class ParticleEmitSystem : public System {

	ROSTER(PARTICLE, POSITION)

	void update(ParticleSystem& particleSystem, uint ticks) {
		for (Entity entity : entities) {
			ParticleComponent& particleComponent = ecs->getComponent<ParticleComponent>(entity);
			PositionComponent& positionComponent = ecs->getComponent<PositionComponent>(entity);

			for (int i = 1; i < ParticleId::count; i++) {
				ParticleEmitter& emitter = particleComponent.emitters[i];
				if (emitter.lastEmit && emitter.lastEmit + emitter.cooldown < ticks) {
					particleSystem.emit(ParticleStyle::templates[i], positionComponent.position, positionComponent.realmId);
					emitter.lastEmit = ticks;
				}
			}
			
		}
	}
};