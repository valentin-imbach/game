
#pragma once
#include "utils.hpp"
#include "Sprite.hpp"
#include "Camera.hpp"

ENUM(ParticleId,
	DIRT,
	SMOKE,
	DEATH,
	FIRE
);

struct ParticleEmitter {
	uint lastEmit = 0;
	uint cooldown = 300;
};

struct ParticleStyle {
	pair source = {0, 0};
	vec positionOffset = {0, 0};
	vec positionVariance = {0, 0};
	vec velocity = {0, 0};
	vec velocityVariance = {0, 0};
	vec acceleration = {0, 0};
	float alphaStart = 1;
	float alphaEnd = 1;
	float scale = 1;
	float scaleVariance = 0;
	uint lifeSpan = 1000;

	static void setTemplates();
	static std::array<ParticleStyle, ParticleId::count> templates;
};

struct Particle {
	Sprite sprite;
	float scale;
	vec position;
	vec velocity;
	vec acceleration;
	RealmId realmId;
	uint lifeSpan;
	uint age;
	bool active = false;
	float alphaStart;
	float alphaEnd;

	void update(uint dt);
	void draw(Camera camera);
};

class ParticleSystem {
public:
	ParticleSystem(int number);

	void update(uint dt);
	void draw(Camera camera);
	void emit(ParticleStyle& style, vec position, RealmId realmId);

	int activeCount;

private:
	int number;
	int index;
	uint seed;
	std::vector<Particle> pool;
};