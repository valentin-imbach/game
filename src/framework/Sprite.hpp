
#pragma once
#include "utils.hpp"
#include "TextureManager.hpp"

#define SPRITE_LAYER_COUNT 10

ENUM(SpriteSheet,
HOLE,
GRASS,
DIRT,
PLAYER,
RESOURCES,
ITEMS,
COW,
ROCK,
WATER,
SAND,
INVENTORY,
CHEST,
HOTBAR,
SLOT,
HEART,
ICONS_WHITE,
ICONS_BLACK,
STATIONS,
MONSTER,
TAB,
MENU,
GRAVEL,
MUD,
PARTICLES,
FIRE,
STATION_ICONS,
HAMMER,
OUTPUT,
CRAFTING_ICONS,
PLANKS,
RAIN,
RESOURCES_NEW,
ROCK_WALL,
HOE,
SOIL,
CROPS,
BANNER,
DEATH_SCREEN)

class Sprite {
public:
	Sprite(SpriteSheet::value spriteSheet = SpriteSheet::NONE, pair source = {0, 0}, pair size = {1, 1}, uchar frameCount = 1, uint frameDuration = 0, uint animationStart = 0, pair animationOffset = {1,0});
	void draw(pair position, float scale = 1, TextureStyle style = TextureStyle(), uint ticks = 0);
	static void loadSpriteSheets();

	SpriteSheet::value spriteSheet;
	uchar frameCount;
	ushort frameDuration;

private:
	void animationReset(uint ticks);
	pair source;
	pair size;
	
	uint animationStart;
	pair animationOffset;
	static std::array<SDL_Texture*, SpriteSheet::count> spriteSheets;
	static std::array<SDL_Texture*, SpriteSheet::count> outlineSpriteSheets;

	friend class CreatureAnimationSystem;
	friend class MaturitySystem;
	friend class SpriteStack;
};

struct SpriteLayer {
	Sprite sprite;
	pair offset = {0, 0};
	uchar priority = 0;
};

class SpriteStack {
public:
	void addSprite(Sprite sprite, uchar priority = 0, pair offset = {0, 0});
	void draw(pair position, float scale = 1, TextureStyle style = TextureStyle(), uint ticks = 0);
	void clear();
	std::pair<pair, pair> bounds();

private:
	void sort();
	std::array<SpriteLayer, SPRITE_LAYER_COUNT> stack;
	uchar depth = 0;

	friend class CreatureAnimationSystem;
	friend class MaturitySystem;
};

ENUM(SpriteEffectId,
SHAKE,
BOUNCE,
RED,
HIGHLIGHT,
HURT,
OUTLINE)

struct SpriteTemplate {
	pair anker;
	pair size = {1, 1};
	pair offset = {0, 0};
	uchar variations = 1;
};

struct DrawCall {
	SpriteStack spriteStack;
	pair position;
	float z;
	int scale;
	TextureStyle style;
};