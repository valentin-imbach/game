
#include "Map.hpp"

#include "utils.hpp"

Map::Map(pair size, uint seed)
	: size(size), mapSeed(seed) {
	for (int y = 0; y < size.y; y++) tiles.emplace_back(size.x);
	temparatureMap = std::make_unique<PerlinNoise>(seed + 87364, 200, 120, 10, 3);
	precipitationMap = std::make_unique<PerlinNoise>(seed + 372342, 100, 800, 130, 3);
	elevationMap = std::make_unique<PerlinNoise>(seed + 267443, 50, 3000, 500, 3);
	vegetationMap = std::make_unique<BoundDistribution>(std::make_unique<PerlinNoise>(seed + 934328, 100, 200, 50, 3), 0, 100);
	variationMap = std::make_unique<BoundDistribution>(std::make_unique<PerlinNoise>(seed + 825934, 10, 200, 50, 5), 0, 100);
	generate();
	// analyse(100000);
}

Biome::value Map::getBiome(pair position) {
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

void Map::generate() {
	for (int x = 0; x < size.x; x++) {
		for (int y = 0; y < size.y; y++) {
			pair position(x, y);
			Biome::value biome = getBiome(position);
			int variation = variationMap->get(position);
			BiomeGroundTemplate* ground = BiomeTemplate::templates[biome]->getGround(variation);
			tiles[x][y] = std::make_unique<Tile>(ground->tileId);
		}
	}

	for (int x = 0; x < size.x; x++) {
		for (int y = 0; y < size.y; y++) {
			pair position(x, y);
			updateStyle(position);
		}
	}
}

TileId::value Map::getTileId(pair position) {
	if (position.x < 0 || position.y < 0) return TileId::NONE;
	if (position.x >= size.x || position.y >= size.y) return TileId::NONE;
	return tiles[position.x][position.y]->tileId;
}

void Map::updateStyle(pair position, bool propagate) {
	std::vector<std::pair<TileId::value, Sprite>>& sprites = tiles[position.x][position.y]->sprites;
	sprites.clear();

	TileId::value tileId = getTileId(position);
	pair baseVariant = noise::choice<pair>(seed++, {{4, 1}, {3, 1}, {2, 1}, {1, 1}, {1, 2}, {1, 3}, {1, 4}});
	Sprite baseSprite = Sprite(Tile::spriteSheets[tileId], baseVariant);
	sprites.emplace_back(tileId, baseSprite);

	for (int d = 0; d < 8; d += 2) {
		TileId::value id = getTileId(position + Direction::taxi[d + 1]);
		if (id == TileId::NONE || id >= tileId) continue;
		TileId::value left = getTileId(position + Direction::taxi[(d + 2) % 8 + 1]);
		TileId::value right = getTileId(position + Direction::taxi[(d + 6) % 8 + 1]);
		TileId::value opposite = getTileId(position + Direction::taxi[(d + 4) % 8 + 1]);

		// Straights
		if (left != id && right != id) {
			std::vector<pair> variants[4] = {{{0, 2}, {0, 3}}, {{2, 5}, {3, 5}}, {{5, 2}, {5, 3}}, {{2, 0}, {3, 0}}};
			Sprite sprite = Sprite(Tile::spriteSheets[id], noise::choice<pair>(seed++, variants[d / 2]));
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
		TileId::value id = getTileId(position + Direction::taxi[d + 1]);
		if (id == TileId::NONE) continue;
		TileId::value left = getTileId(position + Direction::taxi[(d + 1) % 8 + 1]);
		TileId::value left2 = getTileId(position + Direction::taxi[(d + 3) % 8 + 1]);
		TileId::value right = getTileId(position + Direction::taxi[(d + 7) % 8 + 1]);
		TileId::value right2 = getTileId(position + Direction::taxi[(d + 5) % 8 + 1]);

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
	TileId::value id1 = getTileId(position + Direction::taxi[1]);
	TileId::value id2 = getTileId(position + Direction::taxi[3]);
	TileId::value id3 = getTileId(position + Direction::taxi[5]);
	TileId::value id4 = getTileId(position + Direction::taxi[7]);
	if (id1 < tileId && id1 != TileId::NONE && id1 == id2 && id1 == id3 && id1 == id4) {
		Sprite sprite = Sprite(Tile::spriteSheets[id1], {4, 4});
		sprites.emplace_back(id1, sprite);
	}

	std::sort(sprites.begin(), sprites.end(), [](const auto left, const auto right) { return left.first > right.first; });

	if (propagate) {
		for (pair step : Direction::taxi) updateStyle(position + step);
	}
}

void Map::analyse(int samples) {
	uint count[Biome::count] = {};
	for (uint i = 0; i < samples; i++) {
		int x = noise::Int(seed++, -10000, 10000);
		int y = noise::Int(seed++, -10000, 10000);
		pair position(x, y);
		count[getBiome(position)] += 1;
	}
	for (uint b = 1; b < Biome::count; b++) {
		int percent = std::round((100.0f * count[b]) / samples);
		LOG(Biome::strings[b], std::to_string(percent) + "%");
	}
}