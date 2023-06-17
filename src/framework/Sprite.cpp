
#include "Sprite.hpp"

std::array<SDL_Texture*, SpriteSheet::count> Sprite::spriteSheets = {};
std::array<SDL_Texture*, SpriteSheet::count> Sprite::outlineSpriteSheets = {};

Sprite::Sprite(SpriteSheet::value spriteSheet, pair source, pair size, uint8_t frameCount, uint frameDuration, uint animationStart) : spriteSheet(spriteSheet), source(source), size(size), frameCount(frameCount), frameDuration(frameDuration), animationStart(animationStart) {}

void Sprite::draw(pair position, float scale, TextureStyle style, uint ticks) {
	if (!spriteSheet) return;
	uint past = ticks - animationStart;
	int frame = frameCount > 1 ? ((past / frameDuration) % frameCount) : 0;
	pair offset(frame, 0);
	SDL_Texture* texture = style.outline ? outlineSpriteSheets[spriteSheet] : spriteSheets[spriteSheet];
	pair dsize = vec::round(scale * BIT * vec(size));
	TextureManager::drawTexture(texture, nullptr, BIT * (source + offset), BIT * size, position, dsize, style);
}

void Sprite::animationReset(uint ticks) {
	animationStart = ticks;
}

void Sprite::loadSpriteSheets() {
	for (int i = 1; i < SpriteSheet::count; i++) {
		std::string fileName = SpriteSheet::strings[i] + ".png";
		spriteSheets[i] = TextureManager::loadTexture(fileName);
		outlineSpriteSheets[i] = TextureManager::loadTexture(fileName, true);
	}
	TextureManager::lightTexture = TextureManager::loadTexture("light.png");
}

//* SpriteStack

void SpriteStack::addSprite(Sprite sprite, pair offset) {
	assert(depth < SPRITE_LAYERS);
	stack[depth] = std::make_pair(sprite, offset);
	depth += 1;
}

void SpriteStack::draw(pair position, float scale, TextureStyle style, uint ticks) {
	for (int layer = 0; layer < depth; layer++) {
		pair offset = vec::round(scale * BIT * vec(stack[layer].second));
		if (style.centered) offset /= 2;
		stack[layer].first.draw(position + offset, scale, style, ticks);
	}
}

void SpriteStack::clear() {
	stack = {};
}

std::pair<pair, pair> SpriteStack::bounds() {
	pair tl(0, 0);
	pair br(0, 0);
	for (int layer = 0; layer < depth; layer++) {
		tl.y = std::min(tl.y, stack[layer].second.y);
		tl.x = std::min(tl.x, stack[layer].second.x);
		br.y = std::max(br.y, stack[layer].second.y + stack[layer].first.size.y);
		br.x = std::max(br.x, stack[layer].second.x + stack[layer].first.size.x);
	}
	return {tl, br};
}