
#include "Sprite.hpp"

std::array<SDL_Texture*, SpriteSheet::count> Sprite::spriteSheets = {};

Sprite::Sprite(SpriteSheet::value spriteSheet, pair source, pair size, uint8_t frameCount, uint frameDuration) : spriteSheet(spriteSheet), source(source), size(size), frameCount(frameCount), frameDuration(frameDuration) {
	animationStart = SDL_GetTicks();
}

void Sprite::draw(pair position, int scale, bool centered) {
	if (spriteSheet == SpriteSheet::NONE) {
		WARNING("Trying to draw NULL sprite");
		return;
	}
	uint ticks = SDL_GetTicks();
	int frame = frameCount > 1 ? (((ticks - animationStart) / frameDuration) % frameCount) : 0;
	pair offset(frame, 0);
	SDL_Texture* texture = spriteSheets[int(spriteSheet)];
	TextureManager::drawTexture(texture, BIT * (source + offset), BIT * size, position, scale, centered);
}

void Sprite::animationReset() {
	animationStart = SDL_GetTicks();
}

void Sprite::loadSpriteSheets() {
	for (int i = 1; i < SpriteSheet::count; i++) {
		std::string fileName = SpriteSheet::strings[i] + ".png";
		spriteSheets[i] = TextureManager::loadTexture(fileName);
	}
}

//* SpriteStack

void SpriteStack::addSprite(Sprite sprite, pair offset) {
	stack.emplace_back(sprite, offset);
}

void SpriteStack::draw(pair position, int scale, bool centered) {
	for (auto& layer : stack) {
		layer.first.draw(position + BIT * layer.second, scale, centered);
	}
}

void SpriteStack::clear() {
	stack.clear();
}