
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
	// std::ifstream file(Window::instance->root / "json/Particles.json");
	// if (!file) ERROR("File not found");
	// nlohmann::json data = nlohmann::json::parse(file);
	// file.close();

	json::Value data = json::parseFile(Window::instance->root / "json/Particles.json");

	ParticleStyle::templates = {};

	for (auto &[key, val] : data.get<json::Object>()) {
		ParticleId::value particleId = ParticleId::from_string(key);
		if (!particleId) {
			WARNING("Unrecognised Resource:", key);
			continue;
		}

		if (val["source"]) templates[particleId].source = parsePair(val["source"]);
		if (val["positionOffset"]) templates[particleId].positionOffset = parseVec(val["positionOffset"]);
		if (val["positionVariance"]) templates[particleId].positionVariance = parseVec(val["positionVariance"]);
		if (val["velocity"]) templates[particleId].velocity = parseVec(val["velocity"]);
		if (val["velocityVariance"]) templates[particleId].velocityVariance = parseVec(val["velocityVariance"]);
		if (val["acceleration"]) templates[particleId].acceleration = parseVec(val["acceleration"]);
		if (val["alphaStart"]) templates[particleId].alphaStart = float(val["alphaStart"]);
		if (val["alphaEnd"]) templates[particleId].alphaEnd = float(val["alphaEnd"]);
		if (val["scale"]) templates[particleId].scale = float(val["scale"]);
		if (val["scaleVariance"]) templates[particleId].scaleVariance = float(val["scaleVariance"]);
		if (val["lifeSpan"]) templates[particleId].lifeSpan = int(val["lifeSpan"]);
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