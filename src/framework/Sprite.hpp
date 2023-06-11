
#pragma once
#include "utils.hpp"
#include "TextureManager.hpp"

#define SPRITE_LAYERS 3

ENUM(SpriteSheet,
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
STATION_ICONS)

class Sprite {
public:
	Sprite(SpriteSheet::value spriteSheet = SpriteSheet::NONE, pair source = {0, 0}, pair size = {1, 1}, uint8_t frameCount = 1, uint frameDuration = 0, uint animationStart = 0);
	void draw(pair position, float scale = 1, TextureStyle style = TextureStyle(), uint ticks = 0);
	static void loadSpriteSheets();
private:
	void animationReset(uint ticks);
	SpriteSheet::value spriteSheet;
	pair source;
	pair size;
	uint8_t frameCount;
	uint8_t frameDuration;
	uint animationStart;
	static std::array<SDL_Texture*, SpriteSheet::count> spriteSheets;

	friend class CreatureAnimationSystem;
};

class SpriteStack {
public:
	void addSprite(Sprite sprite, pair offset = {0, 0});
	void draw(pair position, float scale = 1, TextureStyle style = TextureStyle(), uint ticks = 0);
	void clear();

private:
	std::array<std::pair<Sprite, pair>, SPRITE_LAYERS> stack;
	uint8_t depth = 0;
	friend class CreatureAnimationSystem;
};

ENUM(SpriteEffectId,
SHAKE,
BOUNCE,
RED,
HIGHLIGHT,
HURT)

struct SpriteTemplate {
	pair anker;
	pair size = {1, 1};
	pair offset = {0, 0};
	uint8_t variations = 1;
};