
#include "Map.hpp"
#include "utils/random.hpp"

Map::Map() {
    uint seed = 123;
    for (int x = 0; x < MAP_WIDTH; x++) {
        for (int y = 0; y < MAP_HEIGHT; y++) {
            if (bernoulli(seed++, 0.3)) tiles[x][y] = Tile(TileId::GRASS);
            else tiles[x][y] = Tile(TileId::DIRT);
        }
    }
}