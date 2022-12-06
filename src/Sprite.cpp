
#include "Sprite.hpp"

std::array<SDL_Texture*, int(SpriteSheet::MAX)> Sprite::spriteSheets = {};

Sprite::Sprite(SpriteSheet spriteSheet, pair source, pair size) : spriteSheet(spriteSheet), source(source), size(size) {}

void Sprite::draw(pair position, int scale, bool centered) {
	if (spriteSheet == SpriteSheet::NONE) {
		WARNING("Trying to draw NULL sprite");
		return;
	}
	SDL_Texture* texture = spriteSheets[int(spriteSheet)];
	TextureManager::drawTexture(texture, BIT * source, BIT * size, position, scale, centered);
}

void Sprite::loadSpriteSheets() {
	spriteSheets[int(SpriteSheet::HOLE)] = TextureManager::loadTexture("hole.png");
	spriteSheets[int(SpriteSheet::GRASS)] = TextureManager::loadTexture("grass.png");
	spriteSheets[int(SpriteSheet::DIRT)] = TextureManager::loadTexture("dirt.png");
}

void SpriteStack::addSprite(Sprite sprite, pair offset) {
	stack.emplace_back(sprite, offset);
}

void SpriteStack::draw(pair position, int scale, bool centered) {
	for (auto& layer : stack) {
		layer.first.draw(position + BIT * layer.second, scale, centered);
	}
}