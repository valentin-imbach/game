
#include "Environment.hpp"
#include "Window.hpp"
#include "Sprite.hpp"

Environment::Environment(uint seed, WorldParameters params) : seed(seed) {
	uint s = seed;	

	temparatureMap = std::make_unique<PerlinNoise>(s++, 200, pair::diff(params.temperature), pair::avg(params.temperature), 3);
	precipitationMap = std::make_unique<PerlinNoise>(s++, 100, pair::diff(params.percipitation), pair::avg(params.percipitation), 3);
	elevationMap = std::make_unique<PerlinNoise>(s++, 200, pair::diff(params.elevation), pair::avg(params.elevation), 3);

	vegetationMap = std::make_unique<BoundDistribution>(std::make_unique<PerlinNoise>(s++, 100, 200, 50, 3), 0, 100);
	variationMap = std::make_unique<BoundDistribution>(std::make_unique<PerlinNoise>(s++, 10, 200, 50, 5), 0, 100);
}

Biome::value Environment::getBiome(pair position) {
	// if (realmType == RealmType::CAVE) return Biome::MOUNTAIN;
	int temperature = temparatureMap->get(position);
	int precipitation = precipitationMap->get(position);
	int elevation = elevationMap->get(position);
	// int vegetation = vegetationMap->get(position);

	if (elevation <= 0) return Biome::OCEAN;
	if (elevation >= 1000) return Biome::MOUNTAIN;

	if (temperature < -5) return Biome::TUNDRA;
	if (temperature < 0) return Biome::TAIGA;

	if (temperature < 10) {
		if (precipitation < 40) return Biome::DESERT;
		if (precipitation < 120) return Biome::GRASSLAND;
		if (precipitation < 200) return Biome::FOREST;
	}

	if (temperature < 20) {
		if (precipitation < 60) return Biome::DESERT;
		if (precipitation < 140) return Biome::SAVANNA;
		if (precipitation < 200) return Biome::FOREST;
		return Biome::SWAMP;
	}

	if (precipitation < 70) return Biome::DESERT;
	if (precipitation < 150) return Biome::SAVANNA;
	if (precipitation < 250) return Biome::FOREST;
	return Biome::RAINFOREST;
}

void Environment::draw(uint ticks) {
	uint s = ticks;
	if (weatherId == WeatherId::RAIN) {
		for (int i = 0; i < intensity; i++) {
			uint ss = noise::UInt(i);

			int sx = noise::Int(ss++, 2);
			int sy = noise::Int(ss++, 2);
			Sprite rainSprite(SpriteSheet::RAIN, {0, sy});	

			int startx = noise::Int(ss++, 2 * Window::instance->size.x);
			int offset = noise::Int(ss++, Window::instance->size.y * 2);
			float speed = noise::Float(ss++, 0.8f, 1.2f);
			int t = int(speed * ticks + offset) % (Window::instance->size.y * 2);

			int dx = startx - t/4;
			int dy = t - 20;

			rainSprite.draw(pair(dx, dy), 4);
		}
	}

}