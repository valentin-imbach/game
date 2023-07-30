
#include "Tile.hpp"
#include "Sprite.hpp"

Tile::Tile(TileId::value tileId)
	: tileId(tileId) {
	sprites = std::vector<std::pair<TileId::value, Sprite>>();
}

std::array<SpriteSheet::value, TileId::count> Tile::spriteSheets = {
	SpriteSheet::NONE,
	SpriteSheet::PLANKS,
	SpriteSheet::GRASS,
	SpriteSheet::DIRT,
	SpriteSheet::MUD,
	SpriteSheet::ROCK,
	SpriteSheet::GRAVEL,
	SpriteSheet::SAND,
	SpriteSheet::WATER,
};

std::array<uint, TileId::count> Tile::tileColours = {
	0x00000000,
	0xb89b65FF,
	0x4db349FF,
	0x786146FF,
	0x59462eFF,
    0x635f5aFF,
	0x75726dFF,
	0xd9c771FF,
	0x68c6d9FF,
};