
#include "Chunk.hpp"
#include "ChunkManager.hpp"
#include "Environment.hpp"
#include "Generation.hpp"
#include "EntityFactory.hpp"
#include "ResourceTemplates.hpp"

#define CLIFF_GAP 200

Chunk::Chunk(pair position) : position(position) {
	seed = hash(69, position);
}

void Chunk::serialise(std::fstream& stream) {

}

void Chunk::deserialise(std::fstream& stream) {

}

bool Chunk::inside(pair pos) {
	return (pos / CHUNK_SIZE) == position;
}

void Chunk::advance(ChunkManager* manager, Environment* environment) {
	switch (stage) {
		case ChunkStage::NONE:
			setNode(manager, environment);
			break;
		case ChunkStage::NODE:
			setBiome(manager, environment);
			break;
		case ChunkStage::BIOME:
			setRiver(manager, environment);
			break;
		case ChunkStage::RIVER:
			setGround(manager, environment);
			break;
		case ChunkStage::GROUND:
			setTiles(manager);
			break;
		case ChunkStage::TILES:
			setObjects(manager, environment);
			break;
		case ChunkStage::OBJECTS:
			break;
		case ChunkStage::LOADED:
		case ChunkStage::MAX:
			return;
	}
	stage = ChunkStage::from_int(stage + 1);
}

void Chunk::setNode(ChunkManager* manager, Environment* environment) {
	uint s = hash(seed, 123);

	nodeOffset.x = noise::Int(s++, 2, CHUNK_SIZE - 2);
	nodeOffset.y = noise::Int(s++, 2, CHUNK_SIZE - 2);
	node = position * CHUNK_SIZE + nodeOffset;

	elevation = environment->elevationMap->get(node);

	ocean = (elevation < 0);
}

void Chunk::setBiome(ChunkManager* manager, Environment* environment) {
	
	coastal = false;
	if (!ocean) {
		for (int i = 1; i < Direction::count; i += 2) {
			pair chunk = position + Direction::taxi[i];
			auto it = manager->chunks.find(chunk);
			if (it->second.ocean) coastal = true;
		}
	}

	biome = environment->getBiome(node, ocean, coastal);

	// if (coastal) {
	// 	biome = Biome::BEACH;
	// }

	float low = elevation;
	float heigh = elevation;
	for (int i = 1; i < Direction::count; i += 2) {
		pair chunk = position + Direction::taxi[i];
		auto it = manager->chunks.find(chunk);
		float h = it->second.elevation;
		if (h < low) {
			low = h;
			down = Direction::from_int(i);
		}

		if (h > heigh) {
			heigh = h;
			up = Direction::from_int(i);
		}
	}

	pair chunkUp = position + pair(0, -1);
	auto it = manager->chunks.find(chunkUp);
	float gap = it->second.elevation - elevation;
	cliff = true; //(!ocean && gap > CLIFF_GAP);

}

void Chunk::setRiver(ChunkManager* manager, Environment* environment) {
	int riverFreq = 5;
	int riverLength = 5;
	if (hash(position) % riverFreq) return;
	if (elevation < 1000) return;

	riverSource = true;
	pair chunk = position;
	for (int i = 0; i < riverLength; i++) {
		auto it = manager->chunks.find(chunk);
		it->second.river += 1;
		
		Direction::value d = it->second.down;
		if (!d || it->second.elevation < 0) break;
		chunk += Direction::taxi[d];
	}
}

void Chunk::refreshMap(Environment* environment) {
	SDL_Surface* mapSurface = SDL_CreateSurface(CHUNK_SIZE, CHUNK_SIZE, SDL_PIXELFORMAT_RGBA8888);
	Uint32 *mapPixels = (Uint32 *)mapSurface->pixels;

	SDL_Surface* tempSurface = SDL_CreateSurface(CHUNK_SIZE, CHUNK_SIZE, SDL_PIXELFORMAT_RGBA8888);
	Uint32 *tempPixels = (Uint32 *)tempSurface->pixels;

	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			GroundId::value groundId = tiles[x][y].groundId;
			mapPixels[y * CHUNK_SIZE + x] = GroundTemplate::templates[groundId].colour;

			pair offset(x, y);
			pair pos = CHUNK_SIZE * position + offset;
			float temp = environment->temparatureMap->get(pos);

			ushort red = (temp + 30) * 3;
			ushort blue = 255 - red;
			uint colour = 0x000000ff | (red << 24) | (blue << 8);
			tempPixels[y * CHUNK_SIZE + x] = colour;
		}
	}
	
	mapTexture = SDL_CreateTextureFromSurface(Window::instance->renderer, mapSurface);
	SDL_DestroySurface(mapSurface);

	tempTexture = SDL_CreateTextureFromSurface(Window::instance->renderer, tempSurface);
	SDL_DestroySurface(tempSurface);
}

void Chunk::setGround(ChunkManager* manager, Environment* environment) {
	// for (int i = 1; i < Direction::count; i++) {
	// 	pair chunk = position + Direction::taxi[i];
	// 	if (!manager->checkStage(chunk, ChunkStage::NODE)) return;
	// }
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			pair offset(x, y);
			pair pos = CHUNK_SIZE * position + offset;

			pair dpos = vec::round(environment->fuzz->get(pos));
			
			pair chunk = position;
			int dist = pair::sdist(dpos, node);

			for (int i = 1; i < Direction::count; i++) {
				pair chunk2 = position + Direction::taxi[i];
				int d = pair::sdist(dpos, manager->chunks.find(chunk2)->second.node);
				if (d < dist) {
					chunk = chunk2;
					dist = d;
				}
			}

			int variation = environment->variationMap->get(dpos);
			Biome::value b = manager->chunks.find(chunk)->second.biome;
			BiomeGroundTemplate& groundTemp = BiomeTemplate::templates[b].getGround(variation);
			tiles[x][y].groundId = groundTemp.groundId;

			auto is_on = [](pair a, pair b, pair pos, float d){
				int n = 20;
				vec v = a;
				vec step = vec(b - a) / n;
				for (int i = 0; i <= n; i++) {
					if (vec::dist(v, pos) < d) return true;
					v += step;
				}
				return false;
			};

			if (environment->rivers && river) {
				if (down) {
					pair a = node;
					pair to = position + Direction::taxi[down];
					auto it = manager->chunks.find(to);
					pair b = it->second.node;
					if (is_on(a, b, pos, river)) {
						tiles[x][y].groundId = GroundId::WATER;
					}
				}

				for (int i = 1; i < Direction::count; i++) {
					pair from = position + Direction::taxi[i];
					auto it = manager->chunks.find(from);
					if (it->second.river && it->second.down == Direction::rotate(i, 4)) {
						pair a = node;
						pair b = it->second.node;
						if (is_on(a, b, pos, it->second.river)) {
							tiles[x][y].groundId = GroundId::WATER;
						}
					}
				}
			}
		}
	}

	if (environment->cliffs && cliff) {
		for (int x = CHUNK_SIZE/2 - 4; x < CHUNK_SIZE/2 + 4; x++) {
			tiles[x][CHUNK_SIZE/2 - 1].groundId = GroundId::DIRT_WALL;
			tiles[x][CHUNK_SIZE/2].groundId = GroundId::DIRT_WALL;
		}
		tiles[CHUNK_SIZE/2 - 5][CHUNK_SIZE/2 - 1].groundId = GroundId::DIRT_WALL;
		tiles[CHUNK_SIZE/2 + 4][CHUNK_SIZE/2 - 1].groundId = GroundId::DIRT_WALL;
	}

	// tiles[nodeOffset.x][nodeOffset.y].groundId = GroundId::PLANKS;


	// for (int x = 0; x < CHUNK_SIZE; x++) {
	// 	for (int y = 0; y < CHUNK_SIZE; y++) {
	// 		pair pos(x, y);
	// 		uint s = hash(seed, pos);
	// 		pair baseVariant = noise::choice<pair>(s, {{4, 1}, {3, 1}, {2, 1}, {1, 1}, {1, 2}, {1, 3}, {1, 4}});
	// 		Sprite baseSprite = Sprite(GroundTemplate::templates[tiles[x][y].groundId].spriteSheet, baseVariant);
	// 		tiles[x][y].sprites.setSprite(0, baseSprite);
	// 	}
	// }

	refreshMap(environment);
	// LOG("Ground set at", position);
}

void Chunk::setTiles(ChunkManager* manager) {
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			pair offset(x, y);
			pair pos = CHUNK_SIZE * position + offset;
			manager->updateStyle(pos);
		}
	}
	// LOG("Tiles set at", position);
}

void Chunk::setObjects(ChunkManager* manager, Environment* environment) {
	uint s = hash(234, position);
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			pair offset(x, y);
			pair pos = CHUNK_SIZE * position + offset;

			//if (tiles[x][y].groundId == GroundId::WATER || tiles[x][y].wallId) continue;
			
			// if (!free(position)) continue;
			float variation = environment->variationMap->get(pos);
			float vegetation = environment->vegetationMap->get(pos);

			uint ss = hash(s, offset);

			if (!noise::bernoulli(ss++, vegetation / 200)) continue;

			BiomeGroundTemplate& ground = BiomeTemplate::templates[biome].getGround(variation);

			int total = 0;
			for (auto& p : ground.resources) total += p.second;

			if (total == 0) continue;
			int choice = noise::Int(ss++, total);

			for (auto& p : ground.resources) {
				choice -= p.second;
				if (choice < 0) {
					pair size = ResourceTemplate::templates[p.first].size;
					if (!manager->realm->free(pos, size) || !manager->realm->walkable(pos, size)) break;
					Entity resource = EntityFactory::createResource(p.first, manager->realm->realmId, pos);
					break;
				}
			}

		}
	}
}

void Chunk::drawTiles(Camera camera, uint ticks) {
	for (int a = 0; a < CHUNK_SIZE; a++) {
		for (int b = 0; b < CHUNK_SIZE; b++) {
			pair offset(a, b);
			vec pos = CHUNK_SIZE * position + offset;
			vec screenPosition = camera.screenPosition(pos + vec(0.5f, 0.5f));
			tiles[a][b].sprites.draw(screenPosition, camera.zoom, TextureStyle(), ticks);
		}
	}
}

void Chunk::serialise2(std::filesystem::path path) {
	std::string posStr = "(" + std::to_string(position.x) + "," + std::to_string(position.y) + ")";
	std::filesystem::create_directory(path / posStr);

	std::fstream chunkFile = std::fstream(path / posStr / "chunk.binary", std::ios::out | std::ios::binary);
	std::fstream entityFile = std::fstream(path / posStr / "entities.binary", std::ios::out | std::ios::binary);
	std::fstream tileFile = std::fstream(path / posStr / "tiles.binary", std::ios::out | std::ios::binary);

	if (!chunkFile || !entityFile || !tileFile) ERROR("Failed to create save files for chunk", position);

	serialise_object(chunkFile,	position);
	serialise_object(chunkFile,	stage);

	for (int y = 0; y < CHUNK_SIZE; y++) {
		for (int x = 0; x < CHUNK_SIZE; x++) {
			serialise_object(tileFile, tiles[x][y].groundId);
		}
	}

	chunkFile.close();
	entityFile.close();
	tileFile.close();
}