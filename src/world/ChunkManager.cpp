
#include "ChunkManager.hpp"
#define TILE_FRAME_TIME 2000

ChunkManager::ChunkManager(uint seed) : seed(seed), chunks() {
	stageBuffer = {};
	stageBuffer[ChunkStage::BIOME] = 1;
	stageBuffer[ChunkStage::RIVER] = 5;
	stageBuffer[ChunkStage::GROUND] = 5;
	stageBuffer[ChunkStage::TILES] = 1;
}

void ChunkManager::generateChunk(pair position, ChunkStage::value target, Environment* environment) {
	
	chunks.emplace(position, position);
	auto it = chunks.find(position);

	if (target == ChunkStage::NONE) return;
	if (it->second.stage >= target) return;

	int buffer = stageBuffer[target];
	for (int x = position.x - buffer; x <= position.x + buffer; x++) {
		for (int y = position.y - buffer; y <= position.y + buffer; y++) {
			pair pos(x, y);
			generateChunk(pos, ChunkStage::from_int(target - 1), environment);
		}
	}

	it->second.advance(this, environment);
}

void ChunkManager::draw(Camera camera, uint ticks) {
	pair range = vec::ceil(vec(Window::instance->size) / (2 * BIT * camera.zoom * CHUNK_SIZE));
	pair start = vec::round(camera.position) / CHUNK_SIZE;

	for (int x = start.x - range.x; x <= start.x + range.x; x++) {
		for (int y = start.y - range.y; y <= start.y + range.y; y++) {
			pair chunk(x, y);

			auto it = chunks.find(chunk);
			if (it == chunks.end()) continue;

			it->second.drawTiles(camera, ticks);
		}
	}
}

pair ChunkManager::getChunk(pair position) {
	return vec::floor(vec(position) / CHUNK_SIZE);
}

pair ChunkManager::getOffset(pair position) {
	return position - CHUNK_SIZE * getChunk(position);
}

bool ChunkManager::checkStage(pair chunk, ChunkStage::value stage) {
	auto it = chunks.find(chunk);
	if (it == chunks.end()) return false;
	return it->second.stage >= stage;
}

GroundId::value ChunkManager::getGround(pair position) {
	pair chunk = getChunk(position);
	if (!checkStage(chunk, ChunkStage::GROUND)) return GroundId::NONE;
	pair offset = getOffset(position);
	return chunks.find(chunk)->second.tiles[offset.x][offset.y].groundId;
}

void ChunkManager::updateStyle(pair position, bool propagate) {
	// for (int i = 0; i < Direction::count; i++) {
	// 	pair pos = position + Direction::taxi[i];
	// 	pair chunk = getChunk(pos);
	// 	if (!checkStage(chunk, ChunkStage::GROUND)) return;
	// }

	if (!getGround(position)) return;
	uint s = hash(42, seed, position);

	std::vector<std::pair<Sprite, int>> sprites;

	GroundId::value groundId = getGround(position);
	GroundId::value top = getGround(position + pair(0, -1));
	GroundId::value bottom = getGround(position + pair(0, 1));
	GroundId::value left = getGround(position + pair(-1, 0));
	GroundId::value right = getGround(position + pair(1, 0));
	GroundId::value bl = getGround(position + pair(-1, 1));
	GroundId::value br = getGround(position + pair(1, 1));
	GroundId::value tl = getGround(position + pair(-1, -1));
	GroundId::value tr = getGround(position + pair(1, -1));

	// GroundId::value wallId = getWallId(position);
	// GroundId::value lw = getWallId(position + pair(-1, 0));
	// GroundId::value rw = getWallId(position + pair(1, 0));
	// GroundId::value tw = getWallId(position + pair(0, -1));
	// GroundId::value bw = getWallId(position + pair(0, 1));


	// if (groundId == GroundId::ROCK_WALL) {
	// 	// if ((top && top != GroundId::ROCK_WALL) || (bottom && bottom != GroundId::ROCK_WALL)) {
	// 	// 	Sprite sprite = Sprite(GroundTemplate::templates[groundId].spriteSheet, pair(1, 4));
	// 	// 	sprites.emplace_back(sprite, groundId);
	// 	// } else {
	// 	// 	if (right == GroundId::ROCK_WALL && bottom == GroundId::ROCK_WALL && br != GroundId::ROCK_WALL && br) {
	// 	// 		Sprite sprite = Sprite(GroundTemplate::templates[groundId].spriteSheet, pair(1, 2));
	// 	// 		sprites.emplace_back(sprite, groundId);
	// 	// 	}
	// 	// 	if (left == GroundId::ROCK_WALL && bottom == GroundId::ROCK_WALL && bl != GroundId::ROCK_WALL && bl) {
	// 	// 		Sprite sprite = Sprite(GroundTemplate::templates[groundId].spriteSheet, pair(4, 2));
	// 	// 		sprites.emplace_back(sprite, groundId);
	// 	// 	}

	// 	// 	if (right == GroundId::ROCK_WALL && top == GroundId::ROCK_WALL && tr != GroundId::ROCK_WALL && tr) {
	// 	// 		Sprite sprite = Sprite(GroundTemplate::templates[groundId].spriteSheet, pair(3, 4));
	// 	// 		sprites.emplace_back(sprite, groundId);
	// 	// 	}
	// 	// 	if (left == GroundId::ROCK_WALL && top == GroundId::ROCK_WALL && tl != GroundId::ROCK_WALL && tl) {
	// 	// 		Sprite sprite = Sprite(GroundTemplate::templates[groundId].spriteSheet, pair(5, 4));
	// 	// 		sprites.emplace_back(sprite, groundId);
	// 	// 	}
	// 	// }

	// 	// if (bottom == GroundId::ROCK_WALL && right && right != GroundId::ROCK_WALL) {
	// 	// 	Sprite sprite = Sprite(GroundTemplate::templates[groundId].spriteSheet, pair(1, 2));
	// 	// 	sprites.emplace_back(sprite, groundId);
	// 	// }

	// 	// if (bottom == GroundId::ROCK_WALL && left && left != GroundId::ROCK_WALL) {
	// 	// 	Sprite sprite = Sprite(GroundTemplate::templates[groundId].spriteSheet, pair(4, 2));
	// 	// 	sprites.emplace_back(sprite, groundId);
	// 	// }
		
	// } else {

		// if (wallId == GroundId::MUD_WALL) {
		// 	if (lw != GroundId::MUD_WALL) {
		// 		sprites.emplace_back(Sprite(GroundTemplate::templates[wallId].spriteSheet, pair(0, 2)), 0);
		// 	} else if (rw != GroundId::MUD_WALL) {
		// 		sprites.emplace_back(Sprite(GroundTemplate::templates[wallId].spriteSheet, pair(4, 2)), 0);
		// 	} else if (bw == GroundId::MUD_WALL) {
		// 		Sprite baseSprite = Sprite(GroundTemplate::templates[wallId].spriteSheet, pair(2, 2), pair(1, 1), GroundTemplate::templates[groundId].frames, TILE_FRAME_TIME, 0, pair(6,0));
		// 		sprites.emplace_back(baseSprite, 0);
		// 	} else {
		// 		Sprite baseSprite = Sprite(GroundTemplate::templates[wallId].spriteSheet, pair(2, 3), pair(1, 1), GroundTemplate::templates[groundId].frames, TILE_FRAME_TIME, 0, pair(6,0));
		// 		sprites.emplace_back(baseSprite, 0);
		// 	}

		// 	if (!tw && top < GroundId::MUD_WALL) {
		// 		Sprite sprite = Sprite(GroundTemplate::templates[top].spriteSheet, pair(1, 5), pair(1,1), GroundTemplate::templates[groundId].frames, TILE_FRAME_TIME, 0, pair(6,0));
		// 		sprites.emplace_back(sprite, top - 100);
		// 		if (tr < top) {
		// 			Sprite corner = Sprite(GroundTemplate::templates[tr].spriteSheet, pair(0, 5), pair(1,1), GroundTemplate::templates[tr].frames, TILE_FRAME_TIME, 0, pair(6,0));
		// 			sprites.emplace_back(corner, tr - 100);
		// 		}
		// 		if (tl < top) {
		// 			Sprite corner = Sprite(GroundTemplate::templates[tl].spriteSheet, pair(5, 5), pair(1,1), GroundTemplate::templates[tl].frames, TILE_FRAME_TIME, 0, pair(6,0));
		// 			sprites.emplace_back(corner, tl - 100);
		// 		}
		// 	}

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
		//}

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
		GroundId::value id = getGround(position + Direction::taxi[dir]);
		if (id == GroundId::ROCK_WALL || id == GroundId::MUD_WALL) continue;
		if (id == GroundId::NONE || id >= groundId) continue;

		GroundId::value left = getGround(position + Direction::taxi[Direction::rotate(dir, 2)]);
		GroundId::value right = getGround(position + Direction::taxi[Direction::rotate(dir, -2)]);
		GroundId::value opposite = getGround(position + Direction::taxi[Direction::rotate(dir, 4)]);

		//* Straights
		if (left != id && right != id) {
			std::vector<pair> variants[4] = {{{0, 2}, {0, 3}}, {{2, 5}, {3, 5}}, {{5, 2}, {5, 3}}, {{2, 0}, {3, 0}}};
			Sprite sprite = Sprite(GroundTemplate::templates[id].spriteSheet, noise::choice<pair>(s++, variants[dir / 2]), pair(1,1), GroundTemplate::templates[id].frames, TILE_FRAME_TIME, 0, pair(6,0));
			sprites.emplace_back(sprite, id);
		}

		//* Us
		if (left == id && right == id && opposite != id) {
			pair variants[4] = {{3, 4}, {4, 2}, {2, 4}, {4, 3}};
			Sprite sprite = Sprite(GroundTemplate::templates[id].spriteSheet, variants[dir / 2], pair(1,1), GroundTemplate::templates[id].frames, TILE_FRAME_TIME, 0, pair(6,0));
			sprites.emplace_back(sprite, id);
		}
	}

	for (int dir = 2; dir < Direction::count; dir += 2) {
		GroundId::value id = getGround(position + Direction::taxi[dir]);
		if (!id || GroundTemplate::templates[id].wall) continue;

		GroundId::value left = getGround(position + Direction::taxi[Direction::rotate(dir, 1)]);
		GroundId::value left2 = getGround(position + Direction::taxi[Direction::rotate(dir, 3)]);
		GroundId::value right = getGround(position + Direction::taxi[Direction::rotate(dir, -1)]);
		GroundId::value right2 = getGround(position + Direction::taxi[Direction::rotate(dir, -3)]);

		//* Curves
		GroundId::value curve = GroundId::MAX;
		if (!GroundTemplate::templates[left].wall && left < groundId && left == right && left2 != left && right2 != right) {
			pair variants[4] = {{3, 2}, {2, 2}, {2, 3}, {3, 3}};
			Sprite sprite = Sprite(GroundTemplate::templates[left].spriteSheet, variants[dir / 2 - 1], pair(1,1), GroundTemplate::templates[left].frames, TILE_FRAME_TIME, 0, pair(6,0));
			sprites.emplace_back(sprite, left);
			curve = left;
		}

		//* Corners
		if (id < groundId && left != id && right != id && id < curve) {
			pair variants[4] = {{0, 5}, {5, 5}, {5, 0}, {0, 0}};
			Sprite sprite = Sprite(GroundTemplate::templates[id].spriteSheet, variants[dir / 2 - 1], pair(1,1), GroundTemplate::templates[id].frames, TILE_FRAME_TIME, 0, pair(6,0));
			sprites.emplace_back(sprite, id);
		}
	}

	//* Os
	GroundId::value id1 = getGround(position + Direction::taxi[1]);
	GroundId::value id2 = getGround(position + Direction::taxi[3]);
	GroundId::value id3 = getGround(position + Direction::taxi[5]);
	GroundId::value id4 = getGround(position + Direction::taxi[7]);

	if (id1 < groundId && id1 != GroundId::NONE && id1 == id2 && id1 == id3 && id1 == id4) {
		Sprite sprite = Sprite(GroundTemplate::templates[id1].spriteSheet, {4, 4}, pair(1,1), GroundTemplate::templates[id1].frames, TILE_FRAME_TIME, 0, pair(6,0));
		sprites.emplace_back(sprite, id1);
	}

	auto lambda = [](const auto left, const auto right) {
		return left.second > right.second;
	};

	std::sort(sprites.begin(), sprites.end(), lambda);

	assert(sprites.size() <= SPRITE_LAYER_COUNT);

	pair chunk = getChunk(position);
	pair offset = getOffset(position);
	Tile& tile = chunks.find(getChunk(position))->second.tiles[offset.x][offset.y];

	tile.sprites.clear();
	for (int i = 0; i < sprites.size(); i++) {
		tile.sprites.setSprite(i, sprites[i].first);
	}

	if (propagate) {
		for (int dir = 1; dir < Direction::count; dir++) {
			updateStyle(position + Direction::taxi[dir]);
		}
	}

}


std::pair<SDL_Surface*, pair> ChunkManager::minimap() {

	if (chunks.empty()) return {nullptr, {}};
	pair topLeft = chunks.begin()->second.position;
	pair bottomRight = topLeft;

	for (auto [p, c] : chunks) {
		topLeft.x = std::min(topLeft.x, p.x);
		topLeft.y = std::min(topLeft.y, p.y);

		bottomRight.x = std::max(bottomRight.x, p.x);
		bottomRight.y = std::max(bottomRight.y, p.y);
	}

	pair size = CHUNK_SIZE * (bottomRight - topLeft + pair(1, 1));
	pair offset = CHUNK_SIZE * topLeft;

	SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, size.x, size.y, 32, SDL_PIXELFORMAT_RGBA8888);
	Uint32 *pixels = (Uint32 *)surface->pixels;

	for (int x = 0; x < size.x; x++) {
		for (int y = 0; y < size.y; y++) {
			pair pos = pair(x, y) + offset;
			GroundId::value groundId = getGround(pos);
			pixels[y * size.x + x] = GroundTemplate::templates[groundId].colour;
		}
	}
	LOG(size);
	return {surface, offset};
}