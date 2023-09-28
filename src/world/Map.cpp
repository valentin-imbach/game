
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
			if (tileId) tiles[x][y] = std::make_unique<Tile>(tileId);
		}
	}
	updateStyle();
}

void Map::updateStyle() {
	for (int x = 0; x < size.x; x++) {
		for (int y = 0; y < size.y; y++) {
			updateStyle(pair(x, y));
		}
	}
}

TileId::value Map::getTileId(pair position) {
	if (!inside(position)) return TileId::NONE;
	if (!tiles[position.x][position.y]) return TileId::NONE;
	return tiles[position.x][position.y]->tileId;
}

void Map::updateStyle(pair position, bool propagate) {
	if (!getTileId(position)) return;
	uint s = seed + hash(position);
	std::vector<std::pair<TileId::value, Sprite>>& sprites = tiles[position.x][position.y]->sprites;
	sprites.clear();



	TileId::value tileId = getTileId(position);
	TileId::value top = getTileId(position + pair(0, -1));
	TileId::value bottom = getTileId(position + pair(0, 1));
	TileId::value left = getTileId(position + pair(-1, 0));
	TileId::value right = getTileId(position + pair(1, 0));
	TileId::value bl = getTileId(position + pair(-1, 1));
	TileId::value br = getTileId(position + pair(1, 1));
	TileId::value tl = getTileId(position + pair(-1, -1));
	TileId::value tr = getTileId(position + pair(1, -1));

	if (tileId == TileId::ROCK_WALL) {
		if ((top && top != TileId::ROCK_WALL) || (bottom && bottom != TileId::ROCK_WALL)) {
			Sprite sprite = Sprite(Tile::spriteSheets[tileId], pair(1, 4));
			sprites.emplace_back(tileId, sprite);
		} else {
			if (right == TileId::ROCK_WALL && bottom == TileId::ROCK_WALL && br != TileId::ROCK_WALL && br) {
				Sprite sprite = Sprite(Tile::spriteSheets[tileId], pair(1, 2));
				sprites.emplace_back(tileId, sprite);
			}
			if (left == TileId::ROCK_WALL && bottom == TileId::ROCK_WALL && bl != TileId::ROCK_WALL && bl) {
				Sprite sprite = Sprite(Tile::spriteSheets[tileId], pair(4, 2));
				sprites.emplace_back(tileId, sprite);
			}

			if (right == TileId::ROCK_WALL && top == TileId::ROCK_WALL && tr != TileId::ROCK_WALL && tr) {
				Sprite sprite = Sprite(Tile::spriteSheets[tileId], pair(3, 4));
				sprites.emplace_back(tileId, sprite);
			}
			if (left == TileId::ROCK_WALL && top == TileId::ROCK_WALL && tl != TileId::ROCK_WALL && tl) {
				Sprite sprite = Sprite(Tile::spriteSheets[tileId], pair(5, 4));
				sprites.emplace_back(tileId, sprite);
			}
		}

		if (bottom == TileId::ROCK_WALL && right && right != TileId::ROCK_WALL) {
			Sprite sprite = Sprite(Tile::spriteSheets[tileId], pair(1, 2));
			sprites.emplace_back(tileId, sprite);
		}

		if (bottom == TileId::ROCK_WALL && left && left != TileId::ROCK_WALL) {
			Sprite sprite = Sprite(Tile::spriteSheets[tileId], pair(4, 2));
			sprites.emplace_back(tileId, sprite);
		}
		
		return;
	}

	pair baseVariant = noise::choice<pair>(s++, {{4, 1}, {3, 1}, {2, 1}, {1, 1}, {1, 2}, {1, 3}, {1, 4}});
	Sprite baseSprite = Sprite(Tile::spriteSheets[tileId], baseVariant);
	sprites.emplace_back(tileId, baseSprite);

	if (left == TileId::ROCK_WALL) {
		if (bl == TileId::ROCK_WALL) {
			Sprite sprite = Sprite(Tile::spriteSheets[TileId::ROCK_WALL], pair(2, 2));
			sprites.emplace_back(TileId::ROCK_WALL, sprite);
		} else if (tileId != TileId::ROCK_WALL) {
			Sprite sprite = Sprite(Tile::spriteSheets[TileId::ROCK_WALL], pair(2, 4));
			sprites.emplace_back(TileId::ROCK_WALL, sprite);
		}
	}

	if (bottom == TileId::ROCK_WALL) {
		Sprite sprite = Sprite(Tile::spriteSheets[TileId::ROCK_WALL], pair(1, 3));
		sprites.emplace_back(TileId::ROCK_WALL, sprite);
	}





	for (int dir = 1; dir < Direction::count; dir += 2) {
		TileId::value id = getTileId(position + Direction::taxi[dir]);
		if (id == TileId::ROCK_WALL) continue;
		if (id == TileId::NONE || id >= tileId) continue;

		uint frameCount = (id == TileId::WATER) ? 4 : 1;
		uint frameDuration = 2000;
		pair animationOffset(6, 0);

		TileId::value left = getTileId(position + Direction::taxi[Direction::rotate(dir, 2)]);
		TileId::value right = getTileId(position + Direction::taxi[Direction::rotate(dir, -2)]);
		TileId::value opposite = getTileId(position + Direction::taxi[Direction::rotate(dir, 4)]);

		// Straights
		if (left != id && right != id) {
			std::vector<pair> variants[4] = {{{0, 2}, {0, 3}}, {{2, 5}, {3, 5}}, {{5, 2}, {5, 3}}, {{2, 0}, {3, 0}}};
			Sprite sprite = Sprite(Tile::spriteSheets[id], noise::choice<pair>(s++, variants[dir / 2]), pair(1,1), frameCount, frameDuration, 0, animationOffset);
			sprites.emplace_back(id, sprite);
		}

		// Us
		if (left == id && right == id && opposite != id) {
			pair variants[4] = {{3, 4}, {4, 2}, {2, 4}, {4, 3}};
			Sprite sprite = Sprite(Tile::spriteSheets[id], variants[dir / 2], pair(1,1), frameCount, frameDuration, 0, animationOffset);
			sprites.emplace_back(id, sprite);
		}
	}

	for (int dir = 2; dir < Direction::count; dir += 2) {
		TileId::value id = getTileId(position + Direction::taxi[dir]);
		if (!id || TileId::wall(id)) continue;

		TileId::value left = getTileId(position + Direction::taxi[Direction::rotate(dir, 1)]);
		TileId::value left2 = getTileId(position + Direction::taxi[Direction::rotate(dir, 3)]);
		TileId::value right = getTileId(position + Direction::taxi[Direction::rotate(dir, -1)]);
		TileId::value right2 = getTileId(position + Direction::taxi[Direction::rotate(dir, -3)]);

		uint frameCount = (left == TileId::WATER) ? 4 : 1;
		uint frameDuration = 2000;
		pair animationOffset(6, 0);

		// Curves
		TileId::value curve = TileId::MAX;
		if (!TileId::wall(left) && left < tileId && left == right && left2 != left && right2 != right) {
			pair variants[4] = {{3, 2}, {2, 2}, {2, 3}, {3, 3}};
			Sprite sprite = Sprite(Tile::spriteSheets[left], variants[dir / 2 - 1], pair(1,1), frameCount, frameDuration, 0, animationOffset);
			sprites.emplace_back(left, sprite);
			curve = left;
		}

		frameCount = (id == TileId::WATER) ? 4 : 1;

		// Corners
		if (id < tileId && left != id && right != id && id < curve) {
			pair variants[4] = {{0, 5}, {5, 5}, {5, 0}, {0, 0}};
			Sprite sprite = Sprite(Tile::spriteSheets[id], variants[dir / 2 - 1], pair(1,1), frameCount, frameDuration, 0, animationOffset);
			sprites.emplace_back(id, sprite);
		}
	}

	// Os
	TileId::value id1 = getTileId(position + Direction::taxi[1]);
	TileId::value id2 = getTileId(position + Direction::taxi[3]);
	TileId::value id3 = getTileId(position + Direction::taxi[5]);
	TileId::value id4 = getTileId(position + Direction::taxi[7]);

	uint frameCount = (id1 == TileId::WATER) ? 4 : 1;
	uint frameDuration = 2000;
	pair animationOffset(6, 0);

	if (id1 < tileId && id1 != TileId::NONE && id1 == id2 && id1 == id3 && id1 == id4) {
		Sprite sprite = Sprite(Tile::spriteSheets[id1], {4, 4}, pair(1,1), frameCount, frameDuration, 0, animationOffset);
		sprites.emplace_back(id1, sprite);
	}

	std::sort(sprites.begin(), sprites.end(), [](const auto left, const auto right) { return left.first >= right.first; });

	if (propagate) {
		for (int dir = 1; dir < Direction::count; dir++) {
			updateStyle(position + Direction::taxi[dir]);
		}
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
			pair position(x, y);
			TileId::value tileId = getTileId(position);
			serialise_object(stream, tileId);
		}
	}
}

bool Map::inside(pair pos) {
	return (0 <= pos.x && pos.x < size.x && 0 <= pos.y && pos.y < size.y);
}

SDL_Surface* Map::makeMiniMap() {
	SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, size.x, size.y, 32, SDL_PIXELFORMAT_RGBA8888);
	for (int x = 0; x < size.x; x++) {
		for (int y = 0; y < size.y; y++) {
			Uint32 *pixels = (Uint32 *)surface->pixels;
			pixels[y * size.x + x] = Tile::tileColours[getTileId({x,y})];
		}
	}
	return surface;
}