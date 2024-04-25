
#include "Sprite.hpp"

std::array<SDL_Texture*, SpriteSheet::count> Sprite::spriteSheets = {};
std::array<SDL_Texture*, SpriteSheet::count> Sprite::outlineSpriteSheets = {};

Sprite::Sprite(SpriteSheet::value spriteSheet, pair source, pair size, uchar frameCount, uint frameDuration, uint animationStart, pair animationOffset) : spriteSheet(spriteSheet), source(source), size(size), frameCount(frameCount), frameDuration(frameDuration), animationStart(animationStart), animationOffset(animationOffset) {}

void Sprite::draw(pair position, float scale, TextureStyle style, uint ticks) {
	if (!spriteSheet) return;
	uint past = ticks - animationStart;
	int frame = frameCount > 1 ? ((past / frameDuration) % frameCount) : 0;
	pair offset = frame * animationOffset;
	SDL_Texture* texture = style.outline ? outlineSpriteSheets[spriteSheet] : spriteSheets[spriteSheet];
	pair dsize = vec::round(scale * BIT * vec(size));
	style.tint = style.tint.mult(tint);
	TextureManager::drawTexture(texture, nullptr, BIT * (source + offset), BIT * size, position, dsize, style);
}

void Sprite::animationReset(uint ticks) {
	animationStart = ticks;
}

void Sprite::loadSpriteSheets() {
	for (uint i = 1; i < SpriteSheet::count; i++) {
		std::string fileName = SpriteSheet::strings[i] + ".png";
		spriteSheets[i] = TextureManager::loadTexture(fileName);
		outlineSpriteSheets[i] = TextureManager::loadTexture(fileName, true);
	}
	TextureManager::lightTexture = TextureManager::loadTexture("light.png");
}

//* SpriteStack

void SpriteStack::setSprite(uchar index, Sprite sprite, pair offset) {
	assert(index < SPRITE_LAYER_COUNT);
	stack[index] = {sprite, offset};
}

void SpriteStack::draw(pair position, float scale, TextureStyle style, uint ticks) {
	for (int layer = 0; layer < SPRITE_LAYER_COUNT; layer++) {
		if (!stack[layer].sprite.spriteSheet) continue;
		pair offset = vec::round(scale * BIT * vec(stack[layer].offset));
		if (style.centered) offset /= 2;
		stack[layer].sprite.draw(position + offset, scale, style, ticks);
	}
}

void SpriteStack::clear() {
	stack = {};
}

std::pair<pair, pair> SpriteStack::bounds() {
	pair tl(0, 0);
	pair br(0, 0);
	for (int layer = 0; layer < SPRITE_LAYER_COUNT; layer++) {
		if (!stack[layer].sprite.spriteSheet) continue;
		tl.y = std::min(tl.y, stack[layer].offset.y);
		tl.x = std::min(tl.x, stack[layer].offset.x);
		br.y = std::max(br.y, stack[layer].offset.y + stack[layer].sprite.size.y);
		br.x = std::max(br.x, stack[layer].offset.x + stack[layer].sprite.size.x);
	}
	return {tl, br};
}