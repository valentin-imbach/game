
#include "Sprite.hpp"

std::array<SDL_Texture*, SpriteSheet::count> Sprite::spriteSheets = {};

Sprite::Sprite(SpriteSheet::value spriteSheet, pair source, pair size, uint8_t frameCount, uint frameDuration, uint animationStart) : spriteSheet(spriteSheet), source(source), size(size), frameCount(frameCount), frameDuration(frameDuration), animationStart(animationStart) {}

void Sprite::draw(pair position, int scale, bool centered, uint ticks) {
	if (!spriteSheet) return;
	uint past = ticks - animationStart;
	int frame = frameCount > 1 ? ((past / frameDuration) % frameCount) : 0;
	pair offset(frame, 0);
	SDL_Texture* texture = spriteSheets[spriteSheet];
	TextureManager::drawTexture(texture, BIT * (source + offset), BIT * size, position, scale, centered);
}

void Sprite::animationReset(uint ticks) {
	animationStart = ticks;
}

void Sprite::loadSpriteSheets() {
	for (int i = 1; i < SpriteSheet::count; i++) {
		std::string fileName = SpriteSheet::strings[i] + ".png";
		spriteSheets[i] = TextureManager::loadTexture(fileName);
	}
}

//* SpriteStack

void SpriteStack::addSprite(Sprite sprite, pair offset) {
	assert(depth < SPRITE_LAYERS);
	stack[depth] = std::make_pair(sprite, offset);
	depth += 1;
}

void SpriteStack::draw(pair position, int scale, bool centered, uint ticks) {
	for (int layer = 0; layer < depth; layer++) {
		stack[layer].first.draw(position + scale * BIT * stack[layer].second, scale, centered, ticks);
	}
}

void SpriteStack::clear() {
	stack = {};
}