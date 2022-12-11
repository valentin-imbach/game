
#pragma once
#include "utils/utils.hpp"
#include "TextureManager.hpp"

enum class SpriteSheet {
	NONE,

	HOLE,
	GRASS,
	DIRT,
	PLAYER,
	RESOURCES,
	ITEMS,
	COW,
	ROCK,
	SAND,
	WATER,
	INVENTORY,

	MAX
};

class Sprite {
public:
	Sprite(SpriteSheet spriteSheet = SpriteSheet::NONE, pair source = {0, 0}, pair size = {1, 1}, uint8_t frameCount = 1, uint frameDuration = 0);
	void draw(pair position, int scale = 1, bool centered = true);
	static void loadSpriteSheets();
private:
	void animationReset();
	SpriteSheet spriteSheet;
	pair source;
	pair size;
	uint8_t frameCount;
	uint frameDuration;
	uint animationStart;
	static std::array<SDL_Texture*, int(SpriteSheet::MAX)> spriteSheets;

	friend class CreatureAnimationSystem;
};

class SpriteStack {
public:
	void addSprite(Sprite sprite, pair offset = {0, 0});
	void draw(pair position, int scale = 1, bool centered = true);
	void clear();
private:
	std::vector<std::pair<Sprite, pair>> stack;
	friend class CreatureAnimationSystem;
};