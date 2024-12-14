
#pragma once
#include "Loot.hpp"
#include "utils.hpp"
#include "Sprite.hpp"
#include "SoundManager.hpp"

struct AnimalTemplate {
	std::array<SpriteStack, MovementState::count> sprites;
	Shape collider;
	Shape hitbox;
	SoundId::value sound = SoundId::NONE;
	uchar health = 10;
	vec speed = {0.5f, 1.0f};

	LootTable lootTable;

	static std::array<AnimalTemplate, AnimalId::count> templates;
	static void setTemplates();
};

