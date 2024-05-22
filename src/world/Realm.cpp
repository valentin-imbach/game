
#include "Realm.hpp"
#include "World.hpp"
#include "EntityFactory.hpp"
#include "Dungeon.hpp"

Realm::Realm(RealmId realmId, uint seed) : realmId(realmId), seed(seed) {}

Realm::Realm(std::fstream& stream) {
	deserialise_object(stream, realmId);
	deserialise_object(stream, seed);
	map = std::make_unique<Map>(stream);
	environment = std::make_unique<Environment>(seed + 2);
	
	SDL_Surface* surface = map->makeMiniMap();
	minimap = SDL_CreateTextureFromSurface(Window::instance->renderer, surface);
	SDL_FreeSurface(surface);
}

void Realm::generate(RealmType::value realmType) {
	if (realmType == RealmType::WORLD) generateWorld(pair(100, 100));
	if (realmType == RealmType::HOUSE) generateHouse(pair(10, 7));
	if (realmType == RealmType::CAVE) generateCave(3, 500);
	if (realmType == RealmType::DUNGEON) generateDungeon();

	map->updateStyle();

	SDL_Surface* surface = map->makeMiniMap();
	minimap = SDL_CreateTextureFromSurface(Window::instance->renderer, surface);
	SDL_FreeSurface(surface);
}

void Realm::generateWorld(pair size) {
	map = std::make_unique<Map>(size, noise::UInt(seed + 1));
	environment = std::make_unique<Environment>(noise::UInt(seed + 2));

	for (int x = 0; x < size.x; x++) {
		for (int y = 0; y < size.y; y++) {
			pair position(x, y);
			Biome::value biome = environment->getBiome(position);
			int variation = environment->variationMap->get(position);
			BiomeGroundTemplate& ground = BiomeTemplate::templates[biome].getGround(variation);
			map->tiles[x][y] = std::make_unique<Tile>(ground.groundId);
		}
	}

	uint s = seed;
	for (int x = 0; x < map->size.x; x++) {
		for (int y = 0; y < map->size.y; y++) {
			pair position(x, y);
			if (!free(position)) continue;
			Biome::value biome = environment->getBiome(position);
			int variation = environment->variationMap->get(position);
			int vegetation = environment->vegetationMap->get(position);
			int choice = noise::Int(s++, 0, 50 + vegetation);
			BiomeGroundTemplate& ground = BiomeTemplate::templates[biome].getGround(variation);
			for (auto& p : ground.resources) {
				choice -= p.second;
				if (choice < 0) {
					Entity resource = EntityFactory::createResource(p.first, realmId, position);
					break;
				}
			}
		}
	}

	spawn = pair(0, 0);
}

void Realm::generateDungeon() {

	std::unique_ptr<dungeon::Node> dungeon = std::make_unique<dungeon::EntryNode>(seed);
	dungeon->build();

	int u = 0;
	int d = 0;
	int l = 0;
	int r = 0;

	for (pair p : dungeon->layout.grid) {
		r = std::max(r, p.x);
		l = std::min(l, p.x);
		u = std::min(u, p.y);
		d = std::max(d, p.y);
	}

	int padding = 2;
	pair size(r - l + 1 + 2 * padding, d - u + 1 + 2 * padding);
	pair offset(l - padding, u - padding);

	map = std::make_unique<Map>(size, noise::UInt(seed + 1));
	environment = std::make_unique<Environment>(noise::UInt(seed + 2), RealmType::CAVE);

	for (int x = 0; x < map->size.x; x++) {
		for (int y = 0; y < map->size.y; y++) {
			pair pos(x,y);
			if (dungeon->layout.grid.find(pos + offset) != dungeon->layout.grid.end()) {
				map->tiles[x][y] = std::make_unique<Tile>(GroundId::ROCK);
			}
		}
	}

	spawn = -offset;
}

void Realm::generateHouse(pair size) {
	map = std::make_unique<Map>(size, noise::UInt(seed + 1));
	environment = std::make_unique<Environment>(noise::UInt(seed + 2));

	for (int x = 0; x < size.x; x++) {
		for (int y = 0; y < size.y; y++) {
			map->tiles[x][y] = std::make_unique<Tile>(GroundId::PLANKS);
		}
	}

	spawn = pair(0, 0);
}

void Realm::generateCave(int count, int length) {
	uint s = seed;
	std::unordered_set<pair> cave;
	for (int i = 0; i < count; i++) {
		pair pos(0, 0);
		for (int j = 0; j < length; j++) {
			for (int x = 0; x <= 1; x++) {
				for (int y = 0; y <= 1; y++) {
					pair offset(x, y);
					cave.insert(pos + offset);
				}
			}
			int dx = noise::Int(s++, -1, 2);
			int dy = noise::Int(s++, -1, 2);
			pos += {dx, dy};
		}
	}

	int u = 0;
	int d = 0;
	int l = 0;
	int r = 0;
	pair entry;

	for (pair p : cave) {
		r = std::max(r, p.x);
		l = std::min(l, p.x);
		u = std::min(u, p.y);
		if (p.y >= d) {
			d = p.y;
			entry = p;
		}
	}

	int padding = 2;
	pair size( r - l + 1 + 2 * padding, d - u + 1 + 2 * padding);

	map = std::make_unique<Map>(size, noise::UInt(seed + 1));
	environment = std::make_unique<Environment>(noise::UInt(seed + 2), RealmType::CAVE);

	pair offset(l - padding, u - padding);

	for (int x = 0; x < size.x; x++) {
		for (int y = 0; y < size.y; y++) {

			pair position(x, y);
			int c = 0;
			for (int dx = -1; dx <= 1; dx++) {
				for (int dy = -1; dy <= 1; dy++) {
					if (cave.find(position + pair(dx, dy) + offset) != cave.end()) c++;
				}
			}
			if (c > 4) {
				map->tiles[x][y] = std::make_unique<Tile>(GroundId::ROCK);
			}
		}
	}


	for (int x = 1; x < size.x - 1; x++) {
		for (int y = 1; y < size.y - 1; y++) {
			pair pos(x, y);
			if (map->getGroundId(pos)) continue;
			bool wall = false;
			for (int dx = -1; dx <= 1; dx++) {
				for (int dy = -1; dy <= 1; dy++) {
					pair neig(x + dx, y + dy);
					if (map->getGroundId(neig) == GroundId::ROCK) wall = true;
				}
			}
			if (wall) map->tiles[x][y] = std::make_unique<Tile>(GroundId::ROCK_WALL);
		}
	}

	s = seed;
	for (int x = 0; x < map->size.x; x++) {
		for (int y = 0; y < map->size.y; y++) {
			pair position(x, y);
			if (!free(position)) continue;
			Biome::value biome = environment->getBiome(position);
			int variation = environment->variationMap->get(position);
			int vegetation = environment->vegetationMap->get(position);
			int choice = noise::Int(s++, 0, 50 + vegetation);
			BiomeGroundTemplate& ground = BiomeTemplate::templates[biome].getGround(variation);
			for (auto& p : ground.resources) {
				choice -= p.second;
				if (choice < 0) {
					Entity resource = EntityFactory::createResource(p.first, realmId, position);
					break;
				}
			}
		}
	}

	spawn = entry;
}

void Realm::linkGrid(Entity entity, pair anker, pair size, bool solid, bool opaque) {
	assert(entity);
	for (int x = 0; x < size.x; x++) {
		for (int y = 0; y < size.y; y++) {
			pair key = anker + pair(x, y);
			if (gridMap.find(key) != gridMap.end()) {
				WARNING("Trying to link more than one entity to tile");
				continue;
			}
			gridMap[key] = entity;
			if (solid) solidMap.insert(key);
			if (opaque) opaqueMap.insert(key);
		}
	}
}

void Realm::unlinkGrid(Entity entity, pair anker, pair size, bool solid, bool opaque) {
	assert(entity);
	for (int x = 0; x <size.x; x++) {
		for (int y = 0; y <size.y; y++) {
			pair key = anker + pair(x, y);
			if (gridMap.find(key) == gridMap.end()) {
				WARNING("Trying to remove non-existing grid link");
				continue;
			}
			gridMap.erase(key);
			if (solid) solidMap.erase(key);
			if (opaque) opaqueMap.erase(key);
		}
	}
}

void Realm::linkChunk(Entity entity, pair chunk) {
	assert(entity);
	if (chunks[chunk].find(entity) != chunks[chunk].end()) {
		WARNING("Trying to link entity", entity, "to chunk", chunk, "twice");
		return;
	}
	chunks[chunk].insert(entity);
}

void Realm::unlinkChunk(Entity entity, pair chunk) {
	assert(entity);
	if (chunks[chunk].find(entity) == chunks[chunk].end()) {
		WARNING("Trying to unlink non-existing entity", entity, "from chunk", chunk);
		return;
	}
	chunks[chunk].erase(entity);
}

bool Realm::free(pair anker, pair size) {
	for (int x = 0; x < size.x; x++) {
		for (int y = 0; y < size.y; y++) {
			pair pos = anker + pair(x, y);
			if (gridMap.find(pos) != gridMap.end()) return false;
			if (!map->getGroundId(pos) || GroundTemplate::templates[map->getGroundId(pos)].wall) return false;
		}
	}
	return true;
}

bool Realm::walkable(pair position) {
	GroundId::value groundId = map->getGroundId(position);
	if (map->getWallId(position)) return false;
	if (!GroundTemplate::templates[groundId].walk) return false;
	if (solidMap.find(position) != solidMap.end()) return false;
	return true;
}

bool Realm::opaque(pair position) {
	return opaqueMap.find(position) != opaqueMap.end();
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
	map->serialize(stream);
}