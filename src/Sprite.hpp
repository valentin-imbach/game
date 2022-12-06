
#pragma once
#include "utils/utils.hpp"
#include "TextureManager.hpp"

enum class SpriteSheet {
	NONE,

	HOLE,
	GRASS,
	DIRT,

	MAX
};

class Sprite {
public:
	Sprite(SpriteSheet spriteSheet = SpriteSheet::NONE, pair source = {0, 0}, pair size = {1, 1});
	SpriteSheet spriteSheet;
	pair source;
	pair size;
	void draw(pair position, int scale = 1, bool centered = true);
	static void loadSpriteSheets();

private:
	static std::array<SDL_Texture*, int(SpriteSheet::MAX)> spriteSheets;
};

class SpriteStack {
public:
	void addSprite(Sprite sprite, pair offset = {0, 0});
	void draw(pair position, int scale = 1, bool centered = true);
	void clear();
private:
	std::vector<std::pair<Sprite, pair>> stack;
};