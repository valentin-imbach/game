
#pragma once
#include "Loot.hpp"
#include "utils.hpp"
#include "Sprite.hpp"
#include "SoundManager.hpp"

ENUM(AnimalId,
COW,
PIG,
HORSE)

struct AnimalTemplate {
	std::array<std::pair<Sprite, Sprite>, CreatureState::count> sprites;
	Shape collider;
	Shape hitbox;
	SoundId::value sound = SoundId::NONE;
	uchar health = 10;
	vec speed = {0.5f, 1.0f};

	LootTable lootTable;

	static std::array<AnimalTemplate, AnimalId::count> templates;
	static void setTemplates();
};

