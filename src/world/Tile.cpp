
#include "Tile.hpp"
#include "Sprite.hpp"

Tile::Tile(TileId::value tileId)
	: tileId(tileId) {
	sprites = std::vector<std::pair<TileId::value, Sprite>>();
}

std::array<SpriteSheet::value, TileId::count> Tile::spriteSheets = {
	SpriteSheet::NONE,
	SpriteSheet::ROCK_WALL,
	SpriteSheet::PLANKS,
	SpriteSheet::GRASS,
	SpriteSheet::SOIL,
	SpriteSheet::DIRT,
	SpriteSheet::MUD,
	SpriteSheet::ROCK,
	SpriteSheet::GRAVEL,
	SpriteSheet::SAND,
	SpriteSheet::WATER,
};

std::array<uint, TileId::count> Tile::tileColours = {
	0x00000000,
	0x000000FF,
	0xb89b65FF,
	0x4db349FF,
	0x605136FF,
	0x786146FF,
	0x59462eFF,
    0x635f5aFF,
	0x75726dFF,
	0xd9c771FF,
	0x68c6d9FF,
};

bool TileId::wall(value tileId) {
	if (tileId == ROCK_WALL) return true;
	return false;
}

bool TileId::liquid(value tileId) {
	if (tileId == WATER) return true;
	return false;
}

float TileId::speedMul(value tileId) {
	if (tileId == GRASS || tileId == DIRT || tileId == SOIL) return 0.9f;
	if (tileId == SAND || tileId == GRAVEL) return 0.8f;
	if (tileId == MUD) return 0.7f;
	return 1.0f;
}

bool TileId::walkable(value tileId) {
	return tileId && !wall(tileId) && !liquid(tileId);
}