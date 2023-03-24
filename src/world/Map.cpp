
#include "Map.hpp"

#include "utils.hpp"

Map::Map(uint seed)
	: mapSeed(seed) {
	generate();
	//analyse(100000);
}

int Map::getTemperature(pair position) {
	return 10 + perlin(mapSeed + 234667, vec(position) / 200) * 120;
}

int Map::getPrecipitation(pair position) {
	return std::max(0, 130 + int(perlin(mapSeed + 372342, vec(position) / 100) * 800));
}

int Map::getElevation(pair position) {
	return 500 + perlin(mapSeed + 267443, vec(position) / 50) * 3000;
}

int Map::getVegetation(pair position) {
	return std::min(std::max(0, 50 + int(perlin(mapSeed + 934328, vec(position) / 30) * 200)), 100);
}

int Map::getVariation(pair position) {
	return std::min(std::max(0, 50 + int(perlin(mapSeed + 825934, vec(position) / 10) * 200)), 100);
}

Biome::value Map::getBiome(pair position) {
	int temperature = getTemperature(position);
	int precipitation = getPrecipitation(position);
	int elevation = getElevation(position);
	int vegetation = getVegetation(position);

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

void Map::generate() {
	for (int x = 0; x < MAP_WIDTH; x++) {
		for (int y = 0; y < MAP_HEIGHT; y++) {
			pair position(x, y);
			Biome::value biome = getBiome(position);
			int variation = getVariation(position);
			BiomeGroundTemplate* ground = BiomeTemplate::templates[biome]->getGround(variation);
			tiles[x][y] = std::make_unique<Tile>(ground->tileId);
		}
	}

	for (int x = 0; x < MAP_WIDTH; x++) {
		for (int y = 0; y < MAP_HEIGHT; y++) {
			pair position(x, y);
			updateStyle(position);
		}
	}
}

TileId::value Map::getTileId(pair position) {
	if (position.x < 0 || position.y < 0) return TileId::NONE;
	if (position.x >= MAP_WIDTH || position.y >= MAP_HEIGHT) return TileId::NONE;
	return tiles[position.x][position.y]->tileId;
}

void Map::updateStyle(pair position, bool propagate) {
	std::vector<std::pair<TileId::value, Sprite>>& sprites = tiles[position.x][position.y]->sprites;
	sprites.clear();

	TileId::value tileId = getTileId(position);
	pair baseVariant = rand_choice<pair>(seed++, {{4, 1}, {3, 1}, {2, 1}, {1, 1}, {1, 2}, {1, 3}, {1, 4}});
	Sprite baseSprite = Sprite(Tile::spriteSheets[tileId], baseVariant);
	sprites.emplace_back(tileId, baseSprite);

	for (int d = 0; d < 8; d += 2) {
		TileId::value id = getTileId(position + taxiSteps[d + 1]);
		if (id == TileId::NONE || id >= tileId) continue;
		TileId::value left = getTileId(position + taxiSteps[(d + 2) % 8 + 1]);
		TileId::value right = getTileId(position + taxiSteps[(d + 6) % 8 + 1]);
		TileId::value opposite = getTileId(position + taxiSteps[(d + 4) % 8 + 1]);

		// Straights
		if (left != id && right != id) {
			std::vector<pair> variants[4] = {{{0, 2}, {0, 3}}, {{2, 5}, {3, 5}}, {{5, 2}, {5, 3}}, {{2, 0}, {3, 0}}};
			Sprite sprite = Sprite(Tile::spriteSheets[id], rand_choice<pair>(seed++, variants[d / 2]));
			sprites.emplace_back(id, sprite);
		}

		// Us
		if (left == id && right == id && opposite != id) {
			pair variants[4] = {{3, 4}, {4, 2}, {2, 4}, {4, 3}};
			Sprite sprite = Sprite(Tile::spriteSheets[id], variants[d / 2]);
			sprites.emplace_back(id, sprite);
		}
	}

	for (int d = 1; d < 8; d += 2) {
		TileId::value id = getTileId(position + taxiSteps[d + 1]);
		if (id == TileId::NONE) continue;
		TileId::value left = getTileId(position + taxiSteps[(d + 1) % 8 + 1]);
		TileId::value left2 = getTileId(position + taxiSteps[(d + 3) % 8 + 1]);
		TileId::value right = getTileId(position + taxiSteps[(d + 7) % 8 + 1]);
		TileId::value right2 = getTileId(position + taxiSteps[(d + 5) % 8 + 1]);

		// Curves
		TileId::value curve = TileId::MAX;
		if (left < tileId && left == right && left2 != left && right2 != right) {
			pair variants[4] = {{3, 2}, {2, 2}, {2, 3}, {3, 3}};
			Sprite sprite = Sprite(Tile::spriteSheets[left], variants[d / 2]);
			sprites.emplace_back(left, sprite);
			curve = left;
		}

		// Corners
		if (id < tileId && left != id && right != id && id < curve) {
			pair variants[4] = {{0, 5}, {5, 5}, {5, 0}, {0, 0}};
			Sprite sprite = Sprite(Tile::spriteSheets[id], variants[d / 2]);
			sprites.emplace_back(id, sprite);
		}
	}

	// Os
	TileId::value id1 = getTileId(position + taxiSteps[1]);
	TileId::value id2 = getTileId(position + taxiSteps[3]);
	TileId::value id3 = getTileId(position + taxiSteps[5]);
	TileId::value id4 = getTileId(position + taxiSteps[7]);
	if (id1 < tileId && id1 != TileId::NONE && id1 == id2 && id1 == id3 && id1 == id4) {
		Sprite sprite = Sprite(Tile::spriteSheets[id1], {4, 4});
		sprites.emplace_back(id1, sprite);
	}

	std::sort(sprites.begin(), sprites.end(), [](const auto left, const auto right) { return left.first > right.first; });

	if (propagate) {
		for (pair step : taxiSteps) updateStyle(position + step);
	}
}

void Map::analyse(int samples) {
	uint count[Biome::count] = {};
	for (int i = 0; i < samples; i++) {
		int x = rand_int(seed++, -10000, 10000);
		int y = rand_int(seed++, -10000, 10000);
		pair position(x, y);
		count[getBiome(position)] += 1;
	}
	for (int b = 1; b < Biome::count; b++) {
		int percent = round((100.0f * count[b]) / samples);
		LOG(Biome::strings[b], std::to_string(percent) + "%");
	}
}