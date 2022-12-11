
#include "Map.hpp"
#include "utils.hpp"

Map::Map() {
	seed = 0;
	for (int x = 0; x < MAP_WIDTH; x++) {
		for (int y = 0; y < MAP_HEIGHT; y++) {
			pair position(x, y);

			int height = perlin(seed, vec(position) / 30) * 200;

			TileId tileId = TileId::NONE;
			if (height < -20) {
				tileId = TileId::WATER;
			} else if (height < 0) {
				tileId = TileId::SAND;
			} else if (height < 10) {
				tileId = TileId::ROCK;
			} else if (height < 20) {
				tileId = TileId::DIRT;
			} else {
				tileId = TileId::GRASS;
			}

			tiles[x][y] = std::make_unique<Tile>(tileId, SpriteStack());
		}
	}

	for (int x = 0; x < MAP_WIDTH; x++) {
		for (int y = 0; y < MAP_HEIGHT; y++) {
			pair position(x, y);
			updateStyle(position);
		}
	}
}

TileId Map::getTileId(pair position) {
	if (position.x < 0 || position.y < 0) return TileId::NONE;
	if (position.x >= MAP_WIDTH || position.y >= MAP_HEIGHT) return TileId::NONE;
	return tiles[position.x][position.y]->tileId;
}

void Map::updateStyle(pair position) {
	SpriteStack& spriteStack = tiles[position.x][position.y]->spriteStack;
	spriteStack.clear();
	TileId tileId = getTileId(position);
	pair baseVariant = rand_choice<pair>(seed++, {{4, 1}, {3, 1}, {2, 1}, {1, 1}, {1, 2}, {1, 3}, {1, 4}});
	Sprite baseSprite = Sprite(Tile::spriteSheets[int(tileId)], baseVariant);
	spriteStack.addSprite(baseSprite);

	for (int d = 0; d < 8; d += 2) {
		TileId id = getTileId(position + taxiSteps[d]);
		if (id == TileId::NONE || id >= tileId) continue;
		TileId left = getTileId(position + taxiSteps[(d + 2) % 8]);
		TileId right = getTileId(position + taxiSteps[(d + 6) % 8]);
		TileId opposite = getTileId(position + taxiSteps[(d + 4) % 8]);

		// Straights
		if (left != id && right != id) {
			std::vector<pair> variants[4] = {{{0, 2}, {0, 3}}, {{2, 5}, {3, 5}}, {{5, 2}, {5, 3}}, {{2, 0}, {3, 0}}};
			spriteStack.addSprite(Sprite(Tile::spriteSheets[int(id)], rand_choice<pair>(seed++, variants[d / 2])));
		}

		// Us
		if (left == id && right == id && opposite != id) {
			pair variants[4] = {{3, 4}, {4, 2}, {2, 4}, {4, 3}};
			spriteStack.addSprite(Sprite(Tile::spriteSheets[int(id)], variants[d / 2]));
		}
	}

	for (int d = 1; d < 8; d += 2) {
		TileId id = getTileId(position + taxiSteps[d]);
		if (id == TileId::NONE) continue;
		TileId left = getTileId(position + taxiSteps[(d + 1) % 8]);
		TileId left2 = getTileId(position + taxiSteps[(d + 3) % 8]);
		TileId right = getTileId(position + taxiSteps[(d + 7) % 8]);
		TileId right2 = getTileId(position + taxiSteps[(d + 5) % 8]);

		// Curves
		if (left < tileId && left == right && left2 != left && right2 != right) {
			pair variants[4] = {{3, 2}, {2, 2}, {2, 3}, {3, 3}};
			spriteStack.addSprite(Sprite(Tile::spriteSheets[int(left)], variants[d / 2]));
		}

		// Corners
		if (id < tileId && left != id && right != id) {
			pair variants[4] = {{0, 5}, {5, 5}, {5, 0}, {0, 0}};
			spriteStack.addSprite(Sprite(Tile::spriteSheets[int(id)], variants[d / 2]));
		}
	}

	// Os
	TileId id1 = getTileId(position + taxiSteps[0]);
	TileId id2 = getTileId(position + taxiSteps[2]);
	TileId id3 = getTileId(position + taxiSteps[4]);
	TileId id4 = getTileId(position + taxiSteps[6]);
	if (id1 < tileId && id1 != TileId::NONE && id1 == id2 && id1 == id3 && id1 == id4) {
		spriteStack.addSprite(Sprite(Tile::spriteSheets[int(id1)], {4, 4}));
	}
}
