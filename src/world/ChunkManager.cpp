#include "ChunkManager.hpp"
#include "World.hpp"
#include "Components.hpp"

#define TILE_FRAME_TIME 2000

ChunkManager::ChunkManager(uint seed) : seed(seed), chunks() {
	stageBuffer = {};
	stageBuffer[ChunkStage::BIOME] = 1;
	stageBuffer[ChunkStage::RIVER] = 5;
	stageBuffer[ChunkStage::GROUND] = 5;
	stageBuffer[ChunkStage::TILES] = 1;
	stageBuffer[ChunkStage::LOADED] = 1;
}

void ChunkManager::generateChunk(pair position, ChunkStage::value target, Environment* environment) {
	
	if (fixed) return;

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
	pair start = getChunk(camera.position);

	for (int x = start.x - range.x; x <= start.x + range.x; x++) {
		for (int y = start.y - range.y; y <= start.y + range.y; y++) {
			pair chunk(x, y);

			auto it = chunks.find(chunk);
			if (it == chunks.end()) continue;

			it->second.drawTiles(camera, ticks);
		}
	}
}

pair ChunkManager::getChunk(vec position) {
	return vec::floor(position / CHUNK_SIZE);
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

WallId::value ChunkManager::getWall(pair position) {
	pair chunk = getChunk(position);
	if (!checkStage(chunk, ChunkStage::GROUND)) return WallId::NONE;
	pair offset = getOffset(position);
	return chunks.find(chunk)->second.tiles[offset.x][offset.y].wallId;
}

void ChunkManager::setGround(pair position, GroundId::value groundId) {
	pair chunk = getChunk(position);
	if (!checkStage(chunk, ChunkStage::GROUND)) return;
	pair offset = getOffset(position);
	chunks.find(chunk)->second.tiles[offset.x][offset.y].groundId = groundId;
	updateStyle2(position, true);
}

void ChunkManager::setWall(pair position, WallId::value wallId) {
	pair chunk = getChunk(position);
	if (!checkStage(chunk, ChunkStage::GROUND)) return;
	pair offset = getOffset(position);
	chunks.find(chunk)->second.tiles[offset.x][offset.y].wallId = wallId;
	updateStyle(position, true);
}

bool ChunkManager::solid(pair position) {
	pair chunk = getChunk(position);
	if (!checkStage(chunk, ChunkStage::GROUND)) return true;
	pair offset = getOffset(position);
	return chunks.find(chunk)->second.solid[offset.x][offset.y];
}

bool ChunkManager::opaque(pair position) {
	pair chunk = getChunk(position);
	if (!checkStage(chunk, ChunkStage::GROUND)) return true;
	pair offset = getOffset(position);
	return chunks.find(chunk)->second.opaque[offset.x][offset.y];
}

Entity ChunkManager::gridEntity(pair position) {
	pair chunk = getChunk(position);
	if (!checkStage(chunk, ChunkStage::GROUND)) return 0;
	pair offset = getOffset(position);
	return chunks.find(chunk)->second.entityGrid[offset.x][offset.y];
}

void ChunkManager::link(pair position, Entity entity, bool solid, bool opaque) {
	pair chunk = getChunk(position);
	// assert(checkStage(chunk, ChunkStage::OBJECTS));
	pair offset = getOffset(position);
	Chunk& c = chunks.find(chunk)->second;
	if (c.entityGrid[offset.x][offset.y]) {
		WARNING("Linking more than one entity to grid at", position);
		return;
	}
	c.entityGrid[offset.x][offset.y] = entity;
	c.solid[offset.x][offset.y] = solid;
	c.opaque[offset.x][offset.y] = opaque;
}

void ChunkManager::unlink(pair position, Entity entity) {
	pair chunk = getChunk(position);
	// assert(checkStage(chunk, ChunkStage::OBJECTS));
	pair offset = getOffset(position);
	Chunk& c = chunks.find(chunk)->second;
	// assert(c.entityGrid[offset.x][offset.y] == entity);
	c.entityGrid[offset.x][offset.y] = 0;
	c.solid[offset.x][offset.y] = false;
	c.opaque[offset.x][offset.y] = false;
}

void ChunkManager::updateStyle2(pair position, bool propagate) {
	std::vector<std::pair<Sprite, int>> sprites;

	GroundId::value ids[8] = {
		getGround(position + pair(1, 0)),
		getGround(position),
		getGround(position + pair(0, 1)),
		getGround(position + pair(1, 1)),
		getGround(position + pair(1, 0)),
		getGround(position),
		getGround(position + pair(0, 1)),
		getGround(position + pair(1, 1))
	};

	if (ids[0] && ids[0] == ids[1] && ids[0] == ids[2] && ids[0] == ids[3]) {
		const GroundTemplate& temp = GroundTemplate::templates[ids[0]];
		Sprite baseSprite = Sprite(temp.spriteSheet2, pair(2,1), pair(1,1), temp.frames, TILE_FRAME_TIME, 0, pair(6,0));
		sprites.emplace_back(baseSprite, ids[0]);
	}

	pair ones[4] = { pair(1,5), pair(4,5), pair(4,0), pair(1,0) };
	pair twos[4] = { pair(2,5), pair(4,1), pair(2,0), pair(0,2) };
	pair threes[4] = { pair(3,2), pair(3,3), pair(1,1), pair(1,4) };

	for (int i = 0; i < 4; i++) {
		if (!ids[0]) continue;
		const GroundTemplate& temp = GroundTemplate::templates[ids[i]];

		if (ids[i + 1] == ids[i] && ids[i + 2] == ids[i] &&  ids[i + 3] != ids[i]) {
			Sprite sprite = Sprite(temp.spriteSheet2, threes[i], pair(1,1), temp.frames, TILE_FRAME_TIME, 0, pair(6,0));
			sprites.emplace_back(sprite, ids[i]);
		}

		if (ids[i + 1] == ids[i] && ids[i + 2] != ids[i] &&  ids[i + 3] != ids[i]) {
			Sprite sprite = Sprite(temp.spriteSheet2, twos[i], pair(1,1), temp.frames, TILE_FRAME_TIME, 0, pair(6,0));
			sprites.emplace_back(sprite, ids[i]);
		}

		if (ids[i + 1] != ids[i] && ids[i + 2] != ids[i] && ids[i + 3] != ids[i]) {
			Sprite sprite = Sprite(temp.spriteSheet2, ones[i], pair(1,1), temp.frames, TILE_FRAME_TIME, 0, pair(6,0));
			sprites.emplace_back(sprite, ids[i]);
		}
	}

	pair diag[2] = { pair(4,3), pair(4,2) };

	for (int i = 0; i < 2; i++) {
		if (!ids[0]) continue;
		const GroundTemplate& temp = GroundTemplate::templates[ids[i]];

		if (ids[i + 1] != ids[i] && ids[i + 2] == ids[i] && ids[i + 3] != ids[i]) {
			Sprite sprite = Sprite(temp.spriteSheet2, diag[i], pair(1,1), temp.frames, TILE_FRAME_TIME, 0, pair(6,0));
			sprites.emplace_back(sprite, ids[i]);
		}
	}

	auto lambda = [](const auto left, const auto right) {
		return left.second > right.second;
	};

	std::sort(sprites.begin(), sprites.end(), lambda);

	assert(sprites.size() <= SPRITE_LAYER_COUNT);

	pair chunk = getChunk(position);
	pair offset = getOffset(position);
	Tile& tile = chunks.find(chunk)->second.tiles[offset.x][offset.y];

	tile.sprites.clear();
	for (int i = 0; i < sprites.size(); i++) {
		tile.sprites.setSprite(i, sprites[i].first);
	}

	if (propagate) {
		for (int dir = 1; dir < Direction::count; dir++) {
			updateStyle2(position + Direction::taxi[dir]);
		}
	}

}

void ChunkManager::updateStyle(pair position, bool propagate) {
	// for (int i = 0; i < Direction::count; i++) {
	// 	pair pos = position + Direction::taxi[i];
	// 	pair chunk = getChunk(pos);
	// 	if (!checkStage(chunk, ChunkStage::GROUND)) return;
	// }

	// if (!getGround(position)) return;

	uint s = hash(42, seed, position);

	std::vector<std::pair<Sprite, int>> sprites;

	GroundId::value groundId = getGround(position);
	GroundId::value top = getGround(position + pair(0, -1));
	GroundId::value bottom = getGround(position + pair(0, 1));
	GroundId::value bb = getGround(position + pair(0, 2));
	GroundId::value left = getGround(position + pair(-1, 0));
	GroundId::value right = getGround(position + pair(1, 0));
	GroundId::value bl = getGround(position + pair(-1, 1));
	GroundId::value br = getGround(position + pair(1, 1));
	GroundId::value bbl = getGround(position + pair(-1, 2));
	GroundId::value bbr = getGround(position + pair(1, 2));
	GroundId::value tl = getGround(position + pair(-1, -1));
	GroundId::value tr = getGround(position + pair(1, -1));

	WallId::value wallId = getWall(position);
	WallId::value lw = getWall(position + pair(-1, 0));
	WallId::value rw = getWall(position + pair(1, 0));
	WallId::value tw = getWall(position + pair(0, -1));
	WallId::value ttw = getWall(position + pair(0, -2));
	WallId::value bw = getWall(position + pair(0, 1));
	WallId::value tlw = getWall(position + pair(-1, -1));
	WallId::value trw = getWall(position + pair(1, -1));
	WallId::value blw = getWall(position + pair(-1, 1));
	WallId::value brw = getWall(position + pair(1, 1));

	//* Walls

	if (wallId && (bottom || bb)) {
		pair source(11, 0);
	
		if (lw == wallId && rw == wallId) {
			source = (tw == wallId && bottom) ? pair(1, 1) : pair(1, 0);
		} else if (lw == wallId) {
			if (bw == wallId) {
				source = (tw == wallId && bottom) ? pair(4, 1) : pair(4, 0);
			} else {
				source = (tw == wallId) ? pair(4, 2) : pair(7, 0);
			}
		} else if (rw == wallId) {
			if (bw == wallId) {
				source = (tw == wallId && bottom) ? pair(3, 1) : pair(3, 0);
			} else {
				source = (tw == wallId) ? pair(3, 2) : pair(6, 0);
			}
		} else {
			source = (tw == wallId && bottom) ? pair(9, 1) : pair(9, 0);
		}
		sprites.emplace_back(Sprite(WallTemplate::templates[wallId].spriteSheet, source), 0);
	}

	if (wallId && !bottom && !bb) {
		if (left || bl || bbl) sprites.emplace_back(Sprite(WallTemplate::templates[wallId].spriteSheet, pair(2, 0)), -1);
		if (right || br || bbr) sprites.emplace_back(Sprite(WallTemplate::templates[wallId].spriteSheet, pair(0, 0)), -1);
		if (top) sprites.emplace_back(Sprite(WallTemplate::templates[wallId].spriteSheet, pair(1, 3)), 0);
		if (!left && !bl && !bbl && !right && !br && !bbr && !top) {
			if (tr) sprites.emplace_back(Sprite(WallTemplate::templates[wallId].spriteSheet, pair(0, 3)), 0);
			if (tl) sprites.emplace_back(Sprite(WallTemplate::templates[wallId].spriteSheet, pair(2, 3)), 0);
		}
	}

	if (!wallId && groundId && tw) {
		pair source(11, 0);
		if (trw == tw && tlw == tw) {
			source = pair(1, 2);
		} else if (trw == tw) {
			source = pair(3, 3);
		} else if (tlw == tw) {
			source = pair(4, 3);
		} else {
			source = pair(9, 2);
		}
		sprites.emplace_back(Sprite(WallTemplate::templates[tw].spriteSheet, source), 0);
	}

	//* Wall Overlap

	// if (top && !tw && wallId) {
	// 	std::vector<pair> variants[4] = {{{0, 2}, {0, 3}}, {{2, 5}, {3, 5}}, {{5, 2}, {5, 3}}, {{2, 0}, {3, 0}}};
	// 	Sprite sprite = Sprite(GroundTemplate::templates[top].spriteSheet, noise::choice<pair>(s++, variants[1]), pair(1,1), GroundTemplate::templates[top].frames, TILE_FRAME_TIME, 0, pair(6,0));
	// 	sprites.emplace_back(sprite, top - 10);
	// }

	//* Ground

	if (groundId) {
		pair baseVariant = noise::choice<pair>(s++, {{4, 1}, {3, 1}, {2, 1}, {1, 1}, {1, 2}, {1, 3}, {1, 4}});
		Sprite baseSprite = Sprite(GroundTemplate::templates[groundId].spriteSheet, baseVariant, pair(1,1), GroundTemplate::templates[groundId].frames, TILE_FRAME_TIME, 0, pair(6,0));
		sprites.emplace_back(baseSprite, groundId);
	} else {
		groundId = GroundId::MAX;
	}

	if (!wallId) {

		for (int dir = 1; dir < Direction::count; dir += 2) {
			GroundId::value id = getGround(position + Direction::taxi[dir]);
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
			if (!id) continue;

			GroundId::value left = getGround(position + Direction::taxi[Direction::rotate(dir, 1)]);
			GroundId::value left2 = getGround(position + Direction::taxi[Direction::rotate(dir, 3)]);
			GroundId::value right = getGround(position + Direction::taxi[Direction::rotate(dir, -1)]);
			GroundId::value right2 = getGround(position + Direction::taxi[Direction::rotate(dir, -3)]);

			//* Curves
			GroundId::value curve = GroundId::MAX;
			if (left < groundId && left == right && left2 != left && right2 != right) {
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
	}

	auto lambda = [](const auto left, const auto right) {
		return left.second > right.second;
	};

	std::sort(sprites.begin(), sprites.end(), lambda);

	assert(sprites.size() <= SPRITE_LAYER_COUNT);

	pair chunk = getChunk(position);
	pair offset = getOffset(position);
	Tile& tile = chunks.find(chunk)->second.tiles[offset.x][offset.y];

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

void ChunkManager::serialise2(std::filesystem::path path) {
	std::filesystem::create_directory(path / "chunks");
	for (auto& c : chunks) c.second.serialise2(path / "chunks");
}

void ChunkManager::attach(Entity entity, pair chunk) {
	if(!entity) return;
	auto it = chunks.find(chunk);
	if (it == chunks.end()) {
		WARNING("Trying to attach entity to unloaded chunk");
		return;
	}

	if (entityIndex.find(entity) != entityIndex.end()) {
		WARNING("Trying to attach entity to multiple chunks");
		return;
	}

	(*it).second.entities.insert(entity);
	entityIndex[entity] = chunk;
}

void ChunkManager::detach(Entity entity) {
	assert(entity);

	auto it = entityIndex.find(entity);
	if (it == entityIndex.end()) {
		WARNING("Trying to detach loose entity");
		return;
	}
	pair chunk = (*it).second;
	auto it2 = chunks.find(chunk);
	if (it2 == chunks.end() || (*it2).second.stage != ChunkStage::LOADED) {
		WARNING("Trying to detach entity from unloaded chunk");
		return;
	}

	entityIndex.erase(entity);
	(*it2).second.entities.erase(entity);
}

void ChunkManager::reballance(World* world) {
	for (auto& c : chunks) {
		Chunk& chunk = c.second;
		if (chunk.stage != ChunkStage::LOADED) continue;
		EntitySet set = chunk.entities;
		for (const Entity& entity: set) {
			if (!world->ecs.hasComponent<PositionComponent>(entity)) continue;
			PositionComponent& positionComponent = world->ecs.getComponent<PositionComponent>(entity);			
			vec middle = vec(CHUNK_SIZE * c.first) + vec(CHUNK_SIZE - 1, CHUNK_SIZE - 1) / 2;
			vec pos = positionComponent.position;
			vec offset = pos - middle;
			int buffer = CHUNK_REACH + CHUNK_SIZE / 2;
			if (std::abs(offset.x) >= buffer || std::abs(offset.y) >= buffer) {
				pair newChunk = getChunk(pos);
				auto it = chunks.find(newChunk);
				if (it == chunks.end() || it->second.stage != ChunkStage::LOADED) continue;
				detach(entity);
				attach(entity, newChunk);
			}			
		}
	}
}