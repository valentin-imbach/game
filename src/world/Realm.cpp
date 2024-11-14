
#include "Realm.hpp"
#include "Generation.hpp"

#include "EntityFactory.hpp"
#include "Dungeon2.hpp"

Realm::Realm(RealmId realmId, uint seed) : realmId(realmId), seed(seed), chunkManager(seed + 1) {
	chunkManager.realm = this;
}

Realm::Realm(std::fstream& stream) : chunkManager(0) {
	deserialise_object(stream, realmId);
	deserialise_object(stream, seed);
	chunkManager = ChunkManager(seed);
	environment = std::make_unique<Environment>(seed + 2, WorldParameters());
}

// void Realm::generate(RealmType::value realmType, WorldParameters params) {
// 	if (realmType == RealmType::WORLD) generate(params);
// 	// if (realmType == RealmType::HOUSE) generateHouse(pair(10, 7));
// 	// if (realmType == RealmType::CAVE) generateCave(3, 500);

// 	// if (realmType == RealmType::DUNGEON) generateDungeon();
// 	if (realmType == RealmType::TEST) generateFlat();
// }

void Realm::generate(WorldParameters params) {
	environment = std::make_unique<Environment>(noise::UInt(seed + 2), params);
	environment->rivers = false;
	environment->cliffs = false;

	// for (int x = 0; x < size.x; x++) {
	// 	for (int y = 0; y < size.y; y++) {
	// 		pair position(x, y);
	// 		Biome::value biome = environment->getBiome(position);
	// 		int variation = environment->variationMap->get(position);
	// 		BiomeGroundTemplate& ground = BiomeTemplate::templates[biome].getGround(variation);
	// 		// map->tiles[x][y] = std::make_unique<Tile>(ground.groundId);
	// 	}
	// }

	// uint s = seed;
	// for (int x = 0; x < map->size.x; x++) {
	// 	for (int y = 0; y < map->size.y; y++) {
	// 		pair position(x, y);
	// 		if (!free(position)) continue;
	// 		Biome::value biome = environment->getBiome(position);
	// 		int variation = environment->variationMap->get(position);
	// 		int vegetation = environment->vegetationMap->get(position);
	// 		int choice = noise::Int(s++, 0, 50 + vegetation);
	// 		BiomeGroundTemplate& ground = BiomeTemplate::templates[biome].getGround(variation);
	// 		for (auto& p : ground.resources) {
	// 			choice -= p.second;
	// 			if (choice < 0) {
	// 				Entity resource = EntityFactory::createResource(p.first, realmId, position);
	// 				break;
	// 			}
	// 		}
	// 	}
	// }

	spawn = pair(0, 0);
}

// void Realm::generateDungeon() {

// 	std::unique_ptr<dungeon::Node> dungeon = std::make_unique<dungeon::EntryNode>(seed);
// 	dungeon->build();

// 	int u = 0;
// 	int d = 0;
// 	int l = 0;
// 	int r = 0;

// 	for (pair p : dungeon->layout.grid) {
// 		r = std::max(r, p.x);
// 		l = std::min(l, p.x);
// 		u = std::min(u, p.y);
// 		d = std::max(d, p.y);
// 	}

// 	int padding = 2;
// 	pair size(r - l + 1 + 2 * padding, d - u + 1 + 2 * padding);
// 	pair offset(l - padding, u - padding);

// 	// map = std::make_unique<Map>(size, noise::UInt(seed + 1));
// 	// environment = std::make_unique<Environment>(noise::UInt(seed + 2), RealmType::CAVE);

// 	// for (int x = 0; x < map->size.x; x++) {
// 	// 	for (int y = 0; y < map->size.y; y++) {
// 	// 		pair pos(x,y);
// 	// 		if (dungeon->layout.grid.find(pos + offset) != dungeon->layout.grid.end()) {
// 	// 			map->tiles[x][y] = std::make_unique<Tile>(GroundId::ROCK);
// 	// 		}
// 	// 	}
// 	// }

// 	spawn = -offset;
// }

void Realm::generateHouse() {

	WorldParameters params;
	params.elevation = pair(500, 500);
	params.temperature = pair(20, 20);
	params.percipitation = pair(100, 100);

	pair vegetation = pair(0, 0);
	pair variation = pair(0, 0);

	environment = std::make_unique<Environment>(noise::UInt(seed + 2), params);

	chunkManager.fixed = true;


	pair pos(0, 0);
	chunkManager.chunks.emplace(pos, pos);
	Chunk& chunk = (*chunkManager.chunks.find(pos)).second;

	pair size(10, 6);
	pair offset(3, 5);

	for (int x = 0; x < size.x; x++) {
		for (int y = 0; y < size.y; y++) {
			chunk.tiles[offset.x + x][offset.y + y] = Tile(GroundId::PLANKS);
		}
		chunk.tiles[offset.x + x][offset.y - 1] = Tile(GroundId::PLANKS, WallId::PLASTER);
		chunk.tiles[offset.x + x][offset.y - 2] = Tile(GroundId::NONE, WallId::PLASTER);
	}

	for (int y = -2; y < size.y; y++) {
		chunk.tiles[offset.x - 1][offset.y + y] = Tile(GroundId::NONE, WallId::PLASTER);
		chunk.tiles[offset.x + size.x][offset.y + y] = Tile(GroundId::NONE, WallId::PLASTER);
	}

	chunk.stage = ChunkStage::GROUND;

	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			pair position(x, y);
			chunkManager.updateStyle(position);
		}
	}

	chunk.biome = Biome::RAINFOREST;

	chunk.stage = ChunkStage::LOADED;

}


void Realm::generateDungeon() {

	WorldParameters params;
	params.elevation = pair(500, 500);
	params.temperature = pair(20, 20);
	params.percipitation = pair(100, 100);

	pair vegetation = pair(0, 0);
	pair variation = pair(0, 0);

	environment = std::make_unique<Environment>(noise::UInt(seed + 2), params);

	chunkManager.fixed = true;

	for (int x = -5; x <= 5; x++) {
		for (int y = -5; y <= 5; y++) {
			pair pos(x, y);
			chunkManager.chunks.emplace(pos, pos);
			Chunk& chunk = (*chunkManager.chunks.find(pos)).second;
			chunk.biome = Biome::RAINFOREST;
			chunk.stage = ChunkStage::LOADED;
		}
	}

	pair size(40, 40);
	Dungeon dungeon(-size/2, size/2, 0);

	for (auto& s : dungeon.layout) {
		chunkManager.setGround(s, GroundId::ROCK);
	}

	for (int x = -5; x <= 5; x++) {
		for (int y = -5; y <= 5; y++) {
			pair pos(x, y);
			Chunk& chunk = (*chunkManager.chunks.find(pos)).second;
			chunk.refreshMap(environment.get());
		}
	}

	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			pair position(x, y);
			chunkManager.updateStyle(position);
		}
	}
	

	
}

void Realm::generateFlat() {

	// map = std::make_unique<Map>(size, noise::UInt(seed + 1));
	
	WorldParameters params;
	params.elevation = pair(500, 500);
	params.temperature = pair(20, 20);
	params.percipitation = pair(100, 100);

	pair vegetation = pair(0, 0);
	pair variation = pair(0, 0);

	environment = std::make_unique<Environment>(noise::UInt(seed + 2), params);

	// int r = 5;
	// for (int x = -r; x <= r; x++) {
	// 	for (int y = -r; y <= r; y++) {
	// 		pair chunk(x, y);
	// 		chunkManager.generateChunk(chunk, ChunkStage::LOADED, environment.get());
	// 	}
	// }
	

	// Voronoi cells = Voronoi(noise::UInt(seed + 3), 20, 4);
	// PerlinDistortion pdist = PerlinDistortion(noise::UInt(seed + 4));

	// TopoGraph topo = TopoGraph(noise::UInt(seed + 3), 20, 5);

	// std::function<Direction::value(pair)> down = [this](pair pos) {
	// 	float height = this->environment->elevationMap->get(pos);
	// 	Direction::value res = Direction::NONE;
	// 	for (int i = 1; i < Direction::count; i++) {
	// 		pair neig = pos + Direction::taxi[i];
	// 		float h = this->environment->elevationMap->get(neig);
	// 		if (h < height) {
	// 			height = h;
	// 			res = Direction::from_int(i);
	// 		}
	// 	}
	// 	return res;
	// };

	// std::function<bool(pair)> valley = [this, &cells, &pdist, &down](pair pos) {
	// 	pair root = vec::round(cells(pdist(pos)));
	// 	Direction::value dir = down(root);
	// 	if (this->environment->elevationMap->get(root) < 0) return false;

	// 	for (int i = 1; i < Direction::count; i++) {
	// 		pair r = vec::round(cells(pdist(pos + Direction::taxi[i])));
	// 		Direction::value dir2 = down(r);
	// 		// int x = pair::dot(Direction::taxi[dir], r-root);
	// 		// int y = pair::dot(Direction::taxi[dir2], root-r);
	// 		int x = pair::dot(Direction::taxi[dir], Direction::taxi[dir2]);
	// 		if (r != root && this->environment->elevationMap->get(r) > 0 && x < 0) return true;
	// 	}
	// 	return false;
	// };

	// std::function<bool(pair)> border = [this, &cells, &pdist](pair pos) {
	// 	pair root = vec::round(cells(pdist(pos)));
	// 	if (this->environment->elevationMap->get(root) < 0) return false;
	// 	for (int i = 1; i < Direction::count; i++) {
	// 		pair r = vec::round(cells(pdist(pos + Direction::taxi[i])));
	// 		if (r != root && this->environment->elevationMap->get(r) > 0) return true;
	// 	}
	// 	return false;
	// };

	// std::function<bool(pair, int)> river = [this, &border, &river](pair pos, int d) {
	// 	if (!border(pos)) return false;

	// 	pair low = pos;
	// 	float height = this->environment->elevationMap->get(pos);
	// 	if (height < 0) return true;
	// 	if (d == 0) return false;

	// 	for (int i = 1; i < Direction::count; i++) {
	// 		pair npos = pos + Direction::taxi[i];
	// 		if (!border(npos)) continue;
	// 		float h = this->environment->elevationMap->get(npos);
	// 		if (h < height) {
	// 			height = h;
	// 			low = npos;
	// 		}
	// 	}

	// 	if (low == pos) {
	// 		return false;
	// 	}

	// 	return river(low, d-1);
	// };

	// for (int x = 0; x < size.x; x++) {
	// 	for (int y = 0; y < size.y; y++) {
	// 		pair pos(x, y);
	// 		// pos = pdist(pos);

	// 		pair node = topo.node(pos);

	// 		// GroundId::value ground = GroundId::GRASS;
	// 		// if (environment->elevationMap->get(root) < 0) {
	// 		// 	ground = GroundId::WATER;
	// 		// }

	// 		// if (valley(pos)) {
	// 		// 	ground = GroundId::DIRT;
	// 		// }

	// 		// if (river(pos, 5)) {
	// 		// 	ground = GroundId::ROCK;
	// 		// }

	// 		GroundId::value ground = topo.ground(node, this->environment.get());

	// 		if (vec::round(topo.position(node)) == pos) ground = GroundId::PLANKS;

	// 		map->tiles[x][y] = std::make_unique<Tile>(ground);
	// 	}
	// }

	// spawn = pair(0, 0);

	// using namespace generation;
	// auto bp = new ResourceNode(ResourceId::BASALT_PEBBLE);
	// auto br = new ResourceNode(ResourceId::BASALT_ROCK);
	// auto bb = new ResourceNode(ResourceId::BASALT_BOULDER);

	// auto sbr = new SpreadNode(br, 2, 5);
	// auto sbp = new SpreadNode(bp, 3, 8);

	// auto sn = SequenceNode({bb, sbr, sbp});

	// auto st = new StructureNode(StructureId::SMALL_TENT);
	// auto lt = new StructureNode(StructureId::TENT);
	// auto t = new ChoiceNode({st, lt});

	// auto cf = new StationNode(StationId::CAMP_FIRE);

	// auto s1 = new StationNode(StationId::CHEST);
	// auto s2 = new StationNode(StationId::WORK_STATION);
	// auto s3 = new StationNode(StationId::FURNACE);
	// auto s4 = new StationNode(StationId::ANVIL);

	// auto sc = new ChoiceNode({s1, s2, s3, s4});
	// auto ssc = new SpreadNode(sc, 3, 2);
	
	// auto s = new SpreadNode(t, 5, 3);
	// auto camp = SequenceNode({cf, s, ssc});
	

	// SpacedAllocation boulders(seed, 10, 5);
	// SpacedAllocation camps(seed, 40, 10);


	// for (int x = 0; x < size.x; x++) {
	// 	for (int y = 0; y < size.y; y++) {
	// 		pair pos(x, y);
	// 		if (camps.get(pos)) {
	// 			camp.generate(seed++, this, pos);
	// 		}
	// 		if (boulders.get(pos)) {
	// 			sn.generate(seed++, this, pos);
	// 		}
	// 	}
	// }
}

// void Realm::generateCave(int count, int length) {
// 	uint s = seed;
// 	std::unordered_set<pair> cave;
// 	for (int i = 0; i < count; i++) {
// 		pair pos(0, 0);
// 		for (int j = 0; j < length; j++) {
// 			for (int x = 0; x <= 1; x++) {
// 				for (int y = 0; y <= 1; y++) {
// 					pair offset(x, y);
// 					cave.insert(pos + offset);
// 				}
// 			}
// 			int dx = noise::Int(s++, -1, 2);
// 			int dy = noise::Int(s++, -1, 2);
// 			pos += {dx, dy};
// 		}
// 	}

// 	int u = 0;
// 	int d = 0;
// 	int l = 0;
// 	int r = 0;
// 	pair entry;

// 	for (pair p : cave) {
// 		r = std::max(r, p.x);
// 		l = std::min(l, p.x);
// 		u = std::min(u, p.y);
// 		if (p.y >= d) {
// 			d = p.y;
// 			entry = p;
// 		}
// 	}

// 	int padding = 2;
// 	pair size( r - l + 1 + 2 * padding, d - u + 1 + 2 * padding);

// 	environment = std::make_unique<Environment>(noise::UInt(seed + 2), WorldParameters());

// 	pair offset(l - padding, u - padding);

// 	for (int x = 0; x < size.x; x++) {
// 		for (int y = 0; y < size.y; y++) {

// 			pair position(x, y);
// 			int c = 0;
// 			for (int dx = -1; dx <= 1; dx++) {
// 				for (int dy = -1; dy <= 1; dy++) {
// 					if (cave.find(position + pair(dx, dy) + offset) != cave.end()) c++;
// 				}
// 			}
// 			if (c > 4) {
// 				// map->tiles[x][y] = std::make_unique<Tile>(GroundId::ROCK);
// 			}
// 		}
// 	}


// 	// for (int x = 1; x < size.x - 1; x++) {
// 	// 	for (int y = 1; y < size.y - 1; y++) {
// 	// 		pair pos(x, y);
// 	// 		if (map->getGroundId(pos)) continue;
// 	// 		bool wall = false;
// 	// 		for (int dx = -1; dx <= 1; dx++) {
// 	// 			for (int dy = -1; dy <= 1; dy++) {
// 	// 				pair neig(x + dx, y + dy);
// 	// 				if (map->getGroundId(neig) == GroundId::ROCK) wall = true;
// 	// 			}
// 	// 		}
// 	// 		if (wall) map->tiles[x][y] = std::make_unique<Tile>(GroundId::ROCK_WALL);
// 	// 	}
// 	// }

// 	// s = seed;
// 	// for (int x = 0; x < map->size.x; x++) {
// 	// 	for (int y = 0; y < map->size.y; y++) {
// 	// 		pair position(x, y);
// 	// 		if (!free(position)) continue;
// 	// 		Biome::value biome = environment->getBiome(position);
// 	// 		int variation = environment->variationMap->get(position);
// 	// 		int vegetation = environment->vegetationMap->get(position);
// 	// 		int choice = noise::Int(s++, 0, 50 + vegetation);
// 	// 		BiomeGroundTemplate& ground = BiomeTemplate::templates[biome].getGround(variation);
// 	// 		for (auto& p : ground.resources) {
// 	// 			choice -= p.second;
// 	// 			if (choice < 0) {
// 	// 				Entity resource = EntityFactory::createResource(p.first, realmId, position);
// 	// 				break;
// 	// 			}
// 	// 		}
// 	// 	}
// 	// }

// 	spawn = entry;
// }

void Realm::link(Entity entity, pair anker, pair size, bool solid, bool opaque) {
	assert(entity);
	if (!free(anker, size)) {
		WARNING("Trying to link to occupied grid");
		return;
	}
	for (int x = 0; x < size.x; x++) {
		for (int y = 0; y < size.y; y++) {
			pair pos = anker + pair(x, y);
			chunkManager.link(pos, entity, solid, opaque);
		}
	}
}

void Realm::unlink(Entity entity, pair anker, pair size) {
	assert(entity);
	for (int x = 0; x <size.x; x++) {
		for (int y = 0; y <size.y; y++) {
			pair pos = anker + pair(x, y);
			chunkManager.unlink(pos, entity);
		}
	}
}

void Realm::attach(Entity entity, pair chunk) {
	chunkManager.attach(entity, chunk);
}

void Realm::detach(Entity entity) {
	chunkManager.detach(entity);
}

GroundId::value Realm::ground(pair position) {
	return chunkManager.getGround(position);
}

bool Realm::free(pair anker, pair size) {
	for (int x = 0; x < size.x; x++) {
		for (int y = 0; y < size.y; y++) {
			pair pos = anker + pair(x, y);
			if (chunkManager.gridEntity(pos)) return false;
		}
	}
	return true;
}

bool Realm::walkable(pair position) {
	if (chunkManager.solid(position)) return false;
	if (chunkManager.getWall(position)) return false;
	GroundId::value groundId = chunkManager.getGround(position);
	if (!GroundTemplate::templates[groundId].walk) return false;
	return true;
}

bool Realm::opaque(pair position) {
	return chunkManager.opaque(position);
}

pair Realm::findFree(pair pos, int radius, bool origin) {
	pair guess = pos;
	uint s = 123;
	while (!free(guess) || (!origin && guess == pos)) {
		guess.x = noise::Int(s++, pos.x - radius, pos.x + radius);
		guess.y = noise::Int(s++, pos.y - radius, pos.y + radius);
	}
	return guess;
}

void Realm::serialise(std::fstream& stream) {
	serialise_object(stream, realmId);
	serialise_object(stream, seed);
	// map->serialize(stream);
}


void Realm::serialise2(std::filesystem::path path) {
	std::string idStr = std::to_string(realmId);
	std::filesystem::create_directory(path / idStr);

	std::fstream file = std::fstream(path / idStr / "realm.binary", std::ios::out | std::ios::binary);
	if (!file) ERROR("Failed to create save files for realm", realmId);

	serialise_object(file, realmId);
	serialise_object(file, seed);

	file.close();

	chunkManager.serialise2(path / idStr);
}