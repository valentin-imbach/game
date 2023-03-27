
#pragma once
#include "utils.hpp"
#include "Sprite.hpp"

struct ParticleStyle {
	Sprite sprite;
	vec positionOffset = {0,0};
	vec positionVariance = {0,0};
	vec velocity, velocityVariance = {0,0};
	vec acceleration = {0,0};
	float alphaStart = 1, alphaEnd = 1;
	
	float scale = 1, scaleVariance = 0;
	uint lifeSpan = 1000;
};

struct Particle {
	Sprite sprite;
	float scale;
	vec position, velocity, acceleration;
	uint lifeSpan, age;
	bool active = false;
	float alphaStart, alphaEnd;

	void update(uint dt);
	void draw(vec cameraPosition, float cameraZoom);
};

class ParticleSystem {
public:
	ParticleSystem(int number);

	void update(uint dt);
	void draw(vec cameraPosition, float cameraZoom);
	void emit(ParticleStyle& style, vec position);

	static ParticleStyle DIRT;
	static ParticleStyle SMOKE;

	int activeCount;

private:
	int number, index;
	uint seed;
	std::vector<Particle> pool;
};