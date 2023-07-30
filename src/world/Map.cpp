
#include "Map.hpp"
#include "utils.hpp"

Map::Map(pair size, uint seed)
	: size(size), seed(seed) {
	for (int x = 0; x < size.x; x++) tiles.emplace_back(size.y);
	
}

Map::Map(std::fstream& stream) {
	deserialise_object(stream, seed);
	deserialise_object(stream, size);
	for (int x = 0; x < size.x; x++) {
		tiles.emplace_back(size.y);
		for (int y = 0; y < size.y; y++) {
			TileId::value tileId;
			deserialise_object(stream, tileId);
			tiles[x][y] = std::make_unique<Tile>(tileId);
		}
	}

	for (int x = 0; x < size.x; x++) {
		for (int y = 0; y < size.y; y++) {
			pair position(x, y);
			updateStyle(position);
		}
	}
}

void Map::generate(Environment* environment) {
	for (int x = 0; x < size.x; x++) {
		for (int y = 0; y < size.y; y++) {
			pair position(x, y);
			Biome::value biome = environment->getBiome(position);
			int variation = environment->variationMap->get(position);
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

void Map::generateInterior(Environment* environment) {
	for (int x = 0; x < size.x; x++) {
		for (int y = 0; y < size.y; y++) {
			tiles[x][y] = std::make_unique<Tile>(TileId::PLANKS);
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
	uint s = seed + hash(position);
	std::vector<std::pair<TileId::value, Sprite>>& sprites = tiles[position.x][position.y]->sprites;
	sprites.clear();

	TileId::value tileId = getTileId(position);
	pair baseVariant = noise::choice<pair>(s++, {{4, 1}, {3, 1}, {2, 1}, {1, 1}, {1, 2}, {1, 3}, {1, 4}});
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
			Sprite sprite = Sprite(Tile::spriteSheets[id], noise::choice<pair>(s++, variants[d / 2]));
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

// void Map::analyse(int samples) {
// 	uint s = seed;
// 	uint count[Biome::count] = {};
// 	for (uint i = 0; i < samples; i++) {
// 		int x = noise::Int(s++, -10000, 10000);
// 		int y = noise::Int(s++, -10000, 10000);
// 		pair position(x, y);
// 		count[getBiome(position)] += 1;
// 	}
// 	for (uint b = 1; b < Biome::count; b++) {
// 		int percent = std::round((100.0f * count[b]) / samples);
// 		LOG(Biome::strings[b], std::to_string(percent) + "%");
// 	}
// }

void Map::serialize(std::fstream& stream) {
	serialise_object(stream, seed);
	serialise_object(stream, size);
	for (int x = 0; x < size.x; x++) {
		for (int y = 0; y < size.y; y++) {
			serialise_object(stream, tiles[x][y]->tileId);
		}
	}
}

bool Map::inside(pair pos) {
	if (pos.x < 0 || pos.x >= size.x) return false;
	if (pos.y < 0 || pos.y >= size.y) return false;
	return true;

}

SDL_Surface* Map::makeMiniMap() {
	SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, size.x, size.y, 32, SDL_PIXELFORMAT_RGBA8888);
	for (int x = 0; x < size.x; x++) {
		for (int y = 0; y < size.y; y++) {
			Uint32 *pixels = (Uint32 *)surface->pixels;
			pixels[y * size.x + x] = Tile::tileColours[tiles[x][y]->tileId];
		}
	}
	return surface;
}