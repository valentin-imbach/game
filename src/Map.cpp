
#include "Map.hpp"
#include <memory>
#include "Sprite.hpp"
#include "utils/random.hpp"

Map::Map() {
	uint seed = 123;
	for (int x = 0; x < MAP_WIDTH; x++) {
		for (int y = 0; y < MAP_HEIGHT; y++) {
			TileId tileId = TileId::NONE;
			if (bernoulli(seed++, 0.3)) {
				tileId = TileId::GRASS;
			} else {
				tileId = TileId::DIRT;
			}

			SpriteStack spriteStack;
			Sprite sprite(Tile::spriteSheets[int(tileId)], {1, 1}, {1, 1});
			spriteStack.addSprite(sprite);
			tiles[x][y] = std::make_unique<Tile>(TileId::DIRT, spriteStack);
		}
	}
}

void Map::updateStyle(int x, int y) {}
