
#include "ParticleSystem.hpp"
#include "TextureManager.hpp"
#include "ECS.hpp"
#include "Window.hpp"
#include "random.hpp"
#include "json.hpp"

std::array<ParticleStyle, ParticleId::count> ParticleStyle::templates = {};

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

void ParticleStyle::setTemplates() {
	std::ifstream file(Window::instance->root / "json/Particles.json");
	if (!file) ERROR("File not found");
	nlohmann::json data = nlohmann::json::parse(file);
	file.close();

	ParticleStyle::templates = {};

	for (auto &[key, value] : data.items()) {
		ParticleId::value particleId = ParticleId::from_string(key);
		if (!particleId) {
			WARNING("Unrecognised Resource:", key);
			continue;
		}

		if (value.contains("source")) templates[particleId].source = pair(value["source"][0], value["source"][1]);
		if (value.contains("positionOffset")) templates[particleId].positionOffset = pair(value["positionOffset"][0], value["positionOffset"][1]);
		if (value.contains("positionVariance")) templates[particleId].positionVariance = pair(value["positionVariance"][0], value["positionVariance"][1]);
		if (value.contains("velocity")) templates[particleId].velocity = pair(value["velocity"][0], value["velocity"][1]);
		if (value.contains("velocityVariance")) templates[particleId].velocityVariance = pair(value["velocityVariance"][0], value["velocityVariance"][1]);
		if (value.contains("acceleration")) templates[particleId].acceleration = pair(value["acceleration"][0], value["acceleration"][1]);
		if (value.contains("alphaStart")) templates[particleId].alphaStart = value["alphaStart"];
		if (value.contains("alphaEnd")) templates[particleId].alphaEnd = value["alphaEnd"];
		if (value.contains("scale")) templates[particleId].scale = value["scale"];
		if (value.contains("scaleVariance")) templates[particleId].scaleVariance = value["scaleVariance"];
		if (value.contains("lifeSpan")) templates[particleId].lifeSpan = value["lifeSpan"];
	}
}

ParticleSystem::ParticleSystem(int number) : number(number), index(number - 1), seed(123) {
	pool.resize(number);
	ParticleStyle::setTemplates();
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

	particle.sprite = Sprite(SpriteSheet::PARTICLES, style.source);
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