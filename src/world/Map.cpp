
#include "Map.hpp"
#include "utils.hpp"

#define TILE_FRAME_TIME 2000

Map::Map(pair size, uint seed)
	: size(size), seed(seed) {
	for (int x = 0; x < size.x; x++) {
		tiles.emplace_back(size.y);
	}
}

Map::Map(std::fstream& stream) {
	deserialise_object(stream, seed);
	deserialise_object(stream, size);
	for (int x = 0; x < size.x; x++) {
		tiles.emplace_back(size.y);
		for (int y = 0; y < size.y; y++) {
			GroundId::value groundId;
			GroundId::value wallId;
			deserialise_object(stream, groundId);
			deserialise_object(stream, wallId);
			if (groundId) tiles[x][y] = std::make_unique<Tile>(groundId, wallId);
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

GroundId::value Map::getGroundId(pair position) {
	if (!inside(position)) return GroundId::NONE;
	if (!tiles[position.x][position.y]) return GroundId::NONE;
	return tiles[position.x][position.y]->groundId;
}

GroundId::value Map::getWallId(pair position) {
	if (!inside(position)) return GroundId::NONE;
	if (!tiles[position.x][position.y]) return GroundId::NONE;
	return tiles[position.x][position.y]->wallId;
}

void Map::updateStyle(pair position, bool propagate) {
	if (!getGroundId(position)) return;
	uint s = seed + hash(position);

	std::vector<std::pair<Sprite, int>> sprites;

	GroundId::value groundId = getGroundId(position);
	GroundId::value top = getGroundId(position + pair(0, -1));
	GroundId::value bottom = getGroundId(position + pair(0, 1));
	GroundId::value left = getGroundId(position + pair(-1, 0));
	GroundId::value right = getGroundId(position + pair(1, 0));
	GroundId::value bl = getGroundId(position + pair(-1, 1));
	GroundId::value br = getGroundId(position + pair(1, 1));
	GroundId::value tl = getGroundId(position + pair(-1, -1));
	GroundId::value tr = getGroundId(position + pair(1, -1));

	GroundId::value wallId = getWallId(position);
	GroundId::value lw = getWallId(position + pair(-1, 0));
	GroundId::value rw = getWallId(position + pair(1, 0));
	GroundId::value tw = getWallId(position + pair(0, -1));
	GroundId::value bw = getWallId(position + pair(0, 1));


	if (groundId == GroundId::ROCK_WALL) {
		if ((top && top != GroundId::ROCK_WALL) || (bottom && bottom != GroundId::ROCK_WALL)) {
			Sprite sprite = Sprite(GroundTemplate::templates[groundId].spriteSheet, pair(1, 4));
			sprites.emplace_back(sprite, groundId);
		} else {
			if (right == GroundId::ROCK_WALL && bottom == GroundId::ROCK_WALL && br != GroundId::ROCK_WALL && br) {
				Sprite sprite = Sprite(GroundTemplate::templates[groundId].spriteSheet, pair(1, 2));
				sprites.emplace_back(sprite, groundId);
			}
			if (left == GroundId::ROCK_WALL && bottom == GroundId::ROCK_WALL && bl != GroundId::ROCK_WALL && bl) {
				Sprite sprite = Sprite(GroundTemplate::templates[groundId].spriteSheet, pair(4, 2));
				sprites.emplace_back(sprite, groundId);
			}

			if (right == GroundId::ROCK_WALL && top == GroundId::ROCK_WALL && tr != GroundId::ROCK_WALL && tr) {
				Sprite sprite = Sprite(GroundTemplate::templates[groundId].spriteSheet, pair(3, 4));
				sprites.emplace_back(sprite, groundId);
			}
			if (left == GroundId::ROCK_WALL && top == GroundId::ROCK_WALL && tl != GroundId::ROCK_WALL && tl) {
				Sprite sprite = Sprite(GroundTemplate::templates[groundId].spriteSheet, pair(5, 4));
				sprites.emplace_back(sprite, groundId);
			}
		}

		if (bottom == GroundId::ROCK_WALL && right && right != GroundId::ROCK_WALL) {
			Sprite sprite = Sprite(GroundTemplate::templates[groundId].spriteSheet, pair(1, 2));
			sprites.emplace_back(sprite, groundId);
		}

		if (bottom == GroundId::ROCK_WALL && left && left != GroundId::ROCK_WALL) {
			Sprite sprite = Sprite(GroundTemplate::templates[groundId].spriteSheet, pair(4, 2));
			sprites.emplace_back(sprite, groundId);
		}
		
	} else {

		if (wallId == GroundId::MUD_WALL) {
			if (lw != GroundId::MUD_WALL) {
				sprites.emplace_back(Sprite(GroundTemplate::templates[wallId].spriteSheet, pair(0, 2)), 0);
			} else if (rw != GroundId::MUD_WALL) {
				sprites.emplace_back(Sprite(GroundTemplate::templates[wallId].spriteSheet, pair(4, 2)), 0);
			} else if (bw == GroundId::MUD_WALL) {
				Sprite baseSprite = Sprite(GroundTemplate::templates[wallId].spriteSheet, pair(2, 2), pair(1, 1), GroundTemplate::templates[groundId].frames, TILE_FRAME_TIME, 0, pair(6,0));
				sprites.emplace_back(baseSprite, 0);
			} else {
				Sprite baseSprite = Sprite(GroundTemplate::templates[wallId].spriteSheet, pair(2, 3), pair(1, 1), GroundTemplate::templates[groundId].frames, TILE_FRAME_TIME, 0, pair(6,0));
				sprites.emplace_back(baseSprite, 0);
			}

			if (!tw && top < GroundId::MUD_WALL) {
				Sprite sprite = Sprite(GroundTemplate::templates[top].spriteSheet, pair(1, 5), pair(1,1), GroundTemplate::templates[groundId].frames, TILE_FRAME_TIME, 0, pair(6,0));
				sprites.emplace_back(sprite, top - 100);
				if (tr < top) {
					Sprite corner = Sprite(GroundTemplate::templates[tr].spriteSheet, pair(0, 5), pair(1,1), GroundTemplate::templates[tr].frames, TILE_FRAME_TIME, 0, pair(6,0));
					sprites.emplace_back(corner, tr - 100);
				}
				if (tl < top) {
					Sprite corner = Sprite(GroundTemplate::templates[tl].spriteSheet, pair(5, 5), pair(1,1), GroundTemplate::templates[tl].frames, TILE_FRAME_TIME, 0, pair(6,0));
					sprites.emplace_back(corner, tl - 100);
				}
			}

			// if (bottom != GroundId::MUD_WALL) {
			// 	Sprite sprite = Sprite(GroundTemplate::templates[bottom].spriteSheet, pair(1, 0), pair(1,1), GroundTemplate::templates[groundId].frames, TILE_FRAME_TIME, 0, pair(6,0));
			// 	sprites.emplace_back(sprite, bottom);
			// 	if (bl < bottom) {
			// 		Sprite corner = Sprite(GroundTemplate::templates[bl].spriteSheet, pair(5, 0), pair(1,1), GroundTemplate::templates[bl].frames, TILE_FRAME_TIME, 0, pair(6,0));
			// 		sprites.emplace_back(corner, bl);
			// 	}
			// 	if (br < bottom) {
			// 		Sprite corner = Sprite(GroundTemplate::templates[br].spriteSheet, pair(0, 0), pair(1,1), GroundTemplate::templates[br].frames, TILE_FRAME_TIME, 0, pair(6,0));
			// 		sprites.emplace_back(corner, br);
			// 	}
			// }
		}

		pair baseVariant = noise::choice<pair>(s++, {{4, 1}, {3, 1}, {2, 1}, {1, 1}, {1, 2}, {1, 3}, {1, 4}});
		Sprite baseSprite = Sprite(GroundTemplate::templates[groundId].spriteSheet, baseVariant, pair(1,1), GroundTemplate::templates[groundId].frames, TILE_FRAME_TIME, 0, pair(6,0));
		sprites.emplace_back(baseSprite, groundId);

		if (left == GroundId::ROCK_WALL) {
			if (bl == GroundId::ROCK_WALL) {
				Sprite sprite = Sprite(GroundTemplate::templates[GroundId::ROCK_WALL].spriteSheet, pair(2, 2));
				sprites.emplace_back(sprite, GroundId::ROCK_WALL);
			} else if (groundId != GroundId::ROCK_WALL) {
				Sprite sprite = Sprite(GroundTemplate::templates[GroundId::ROCK_WALL].spriteSheet, pair(2, 4));
				sprites.emplace_back(sprite, GroundId::ROCK_WALL);
			}
		}

		if (bottom == GroundId::ROCK_WALL) {
			Sprite sprite = Sprite(GroundTemplate::templates[GroundId::ROCK_WALL].spriteSheet, pair(1, 3));
			sprites.emplace_back(sprite, GroundId::ROCK_WALL);
		}


		for (int dir = 1; dir < Direction::count; dir += 2) {
			GroundId::value id = getGroundId(position + Direction::taxi[dir]);
			if (id == GroundId::ROCK_WALL || id == GroundId::MUD_WALL) continue;
			if (id == GroundId::NONE || id >= groundId) continue;

			GroundId::value left = getGroundId(position + Direction::taxi[Direction::rotate(dir, 2)]);
			GroundId::value right = getGroundId(position + Direction::taxi[Direction::rotate(dir, -2)]);
			GroundId::value opposite = getGroundId(position + Direction::taxi[Direction::rotate(dir, 4)]);

			// Straights
			if (left != id && right != id) {
				std::vector<pair> variants[4] = {{{0, 2}, {0, 3}}, {{2, 5}, {3, 5}}, {{5, 2}, {5, 3}}, {{2, 0}, {3, 0}}};
				Sprite sprite = Sprite(GroundTemplate::templates[id].spriteSheet, noise::choice<pair>(s++, variants[dir / 2]), pair(1,1), GroundTemplate::templates[id].frames, TILE_FRAME_TIME, 0, pair(6,0));
				sprites.emplace_back(sprite, id);
			}

			// Us
			if (left == id && right == id && opposite != id) {
				pair variants[4] = {{3, 4}, {4, 2}, {2, 4}, {4, 3}};
				Sprite sprite = Sprite(GroundTemplate::templates[id].spriteSheet, variants[dir / 2], pair(1,1), GroundTemplate::templates[id].frames, TILE_FRAME_TIME, 0, pair(6,0));
				sprites.emplace_back(sprite, id);
			}
		}

		for (int dir = 2; dir < Direction::count; dir += 2) {
			GroundId::value id = getGroundId(position + Direction::taxi[dir]);
			if (!id || GroundTemplate::templates[id].wall) continue;

			GroundId::value left = getGroundId(position + Direction::taxi[Direction::rotate(dir, 1)]);
			GroundId::value left2 = getGroundId(position + Direction::taxi[Direction::rotate(dir, 3)]);
			GroundId::value right = getGroundId(position + Direction::taxi[Direction::rotate(dir, -1)]);
			GroundId::value right2 = getGroundId(position + Direction::taxi[Direction::rotate(dir, -3)]);

			// Curves
			GroundId::value curve = GroundId::MAX;
			if (!GroundTemplate::templates[left].wall && left < groundId && left == right && left2 != left && right2 != right) {
				pair variants[4] = {{3, 2}, {2, 2}, {2, 3}, {3, 3}};
				Sprite sprite = Sprite(GroundTemplate::templates[left].spriteSheet, variants[dir / 2 - 1], pair(1,1), GroundTemplate::templates[left].frames, TILE_FRAME_TIME, 0, pair(6,0));
				sprites.emplace_back(sprite, left);
				curve = left;
			}

			// Corners
			if (id < groundId && left != id && right != id && id < curve) {
				pair variants[4] = {{0, 5}, {5, 5}, {5, 0}, {0, 0}};
				Sprite sprite = Sprite(GroundTemplate::templates[id].spriteSheet, variants[dir / 2 - 1], pair(1,1), GroundTemplate::templates[id].frames, TILE_FRAME_TIME, 0, pair(6,0));
				sprites.emplace_back(sprite, id);
			}
		}

		// Os
		GroundId::value id1 = getGroundId(position + Direction::taxi[1]);
		GroundId::value id2 = getGroundId(position + Direction::taxi[3]);
		GroundId::value id3 = getGroundId(position + Direction::taxi[5]);
		GroundId::value id4 = getGroundId(position + Direction::taxi[7]);

		if (id1 < groundId && id1 != GroundId::NONE && id1 == id2 && id1 == id3 && id1 == id4) {
			Sprite sprite = Sprite(GroundTemplate::templates[id1].spriteSheet, {4, 4}, pair(1,1), GroundTemplate::templates[id1].frames, TILE_FRAME_TIME, 0, pair(6,0));
			sprites.emplace_back(sprite, id1);
		}
	}

	auto lambda = [](const auto left, const auto right) {
		return left.second > right.second;
	};
	std::sort(sprites.begin(), sprites.end(), lambda);

	assert(sprites.size() <= SPRITE_LAYER_COUNT);
	tiles[position.x][position.y]->sprites.clear();
	for (int i = 0; i < sprites.size(); i++) {
		tiles[position.x][position.y]->sprites.setSprite(i, sprites[i].first);
	}

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
			GroundId::value groundId = getGroundId(position);
			GroundId::value wallId = getWallId(position);
			serialise_object(stream, groundId);
			serialise_object(stream, wallId);
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
			pair pos(x,y);
			GroundId::value groundId = getGroundId(pos);
			pixels[y * size.x + x] = GroundTemplate::templates[groundId].colour;
		}
	}
	return surface;
}