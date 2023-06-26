
#include "ParticleSystem.hpp"
#include "TextureManager.hpp"
#include "ECS.hpp"
#include "Window.hpp"
#include "random.hpp"

ParticleStyle ParticleSystem::DIRT;
ParticleStyle ParticleSystem::SMOKE;

void Particle::update(uint dt) {
	age += dt;
	if (age > lifeSpan) active = false;

	velocity += dt * (acceleration / 1000);
	position += dt * (velocity / 1000);
}

void Particle::draw(Camera camera) {
	if (realmId != camera.realmId) return;
	float t = float(age) / lifeSpan;
	float alpha = (1-t) * alphaStart + t * alphaEnd;
	TextureStyle style;
	style.alpha = alpha;
	sprite.draw(camera.screenPosition(position), camera.zoom * scale, style);
}

ParticleSystem::ParticleSystem(int number) : number(number), index(number - 1), seed(123) {
	pool.resize(number);

	DIRT.sprite = Sprite(SpriteSheet::PARTICLES, {0, 0});
	DIRT.velocity = {0, -1};
	DIRT.scale = 0.5f;
	DIRT.positionVariance = {0.2, 0};
	DIRT.velocityVariance = {1 , 0.2};
	DIRT.acceleration.y = 3;
	DIRT.lifeSpan = 500;
	DIRT.alphaEnd = 0;

	SMOKE.sprite = Sprite(SpriteSheet::PARTICLES, {1, 0});
	SMOKE.scale = 0.5f;
	SMOKE.velocity = {0, -1};
	SMOKE.positionVariance = {0.2, 0.1};
	SMOKE.velocityVariance = {0 , 0.1};
	SMOKE.lifeSpan = 700;
	SMOKE.positionOffset = {0, -0.5f};
	SMOKE.alphaEnd = 0;
	SMOKE.scaleVariance = 0.2;
}

void ParticleSystem::update(uint dt) {
	activeCount = 0;
	for (Particle& particle : pool) {
		if (particle.active) {
			activeCount += 1;
			particle.update(dt);
		}
	}
}

void ParticleSystem::draw(Camera camera) {
	for (Particle& particle : pool) {
		if (particle.active) particle.draw(camera);
	}
}

void ParticleSystem::emit(ParticleStyle& style, vec position, RealmId realmId) {
	Particle& particle = pool[index];
	index = (index == 0) ? (number - 1) : index - 1;

	particle.sprite = style.sprite;
	particle.scale = style.scale + noise::Float(seed++, -style.scaleVariance, style.scaleVariance);

	particle.realmId = realmId;

	particle.position = position + style.positionOffset;
	particle.position.x += noise::Float(seed++, -style.positionVariance.x, style.positionVariance.x);
	particle.position.y += noise::Float(seed++, -style.positionVariance.y, style.positionVariance.y);

	particle.velocity = style.velocity;
	particle.velocity.x += noise::Float(seed++, -style.velocityVariance.x, style.velocityVariance.x);
	particle.velocity.y += noise::Float(seed++, -style.velocityVariance.y, style.velocityVariance.y);

	particle.acceleration = style.acceleration;

	particle.alphaStart = style.alphaStart;
	particle.alphaEnd = style.alphaEnd;

	particle.lifeSpan = style.lifeSpan;
	particle.age = 0;

	particle.active = true;
}