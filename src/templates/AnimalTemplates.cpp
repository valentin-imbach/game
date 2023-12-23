
#include "AnimalTemplates.hpp"
#include "json.hpp"
#include "Window.hpp"

std::array<AnimalTemplate, AnimalId::count> AnimalTemplate::templates = {};

void AnimalTemplate::setTemplates() {
	std::ifstream file(Window::instance->root / "json/Animals.json");
	if (!file) ERROR("File not found");
	nlohmann::json data = nlohmann::json::parse(file);
	file.close();

	AnimalTemplate::templates = {};

	for (auto &[key, value] : data.items()) {
		AnimalId::value animalId = AnimalId::from_string(key);
		if (!animalId) {
			WARNING("Unrecognised Animal:", key);
			continue;
		}

		SpriteSheet::value spriteSheet = SpriteSheet::from_string(value["sheet"]);
		
		for (auto &[key, value] : value["sprites"].items()) {
			MovementState::value movementState = MovementState::from_string(key);
			if (!movementState) {
				WARNING("Unrecognised MovementState:", key);
				continue;
			}
			pair source1(value["source"][0][0], value["source"][0][1]);
			pair source2(value["source"][1][0], value["source"][1][1]);
			pair size(value["size"][0], value["size"][1]);
			uchar frames = value["frames"];
			templates[animalId].sprites[movementState].first = Sprite(spriteSheet, source1, size, frames, 100);
			templates[animalId].sprites[movementState].second = Sprite(spriteSheet, source2, size, frames, 100);
		}

		auto col = value["collider"];
		templates[animalId].collider = Shape(vec(col["size"][0], col["size"][1]));
		if (col.contains("offset")) templates[animalId].collider.offset = vec(col["offset"][0], col["offset"][1]);

		auto hit = value["hitbox"];
		templates[animalId].hitbox = Shape(vec(hit["size"][0], hit["size"][1]));
		if (hit.contains("offset")) templates[animalId].hitbox.offset = vec(hit["offset"][0], hit["offset"][1]);

		if (value.contains("health")) templates[animalId].health = value["health"];
		if (value.contains("sound")) templates[animalId].sound = SoundId::from_string(value["sound"]);
		if (value.contains("speed")) templates[animalId].speed = vec(value["speed"][0], value["speed"][1]);

		for (auto &[key, value] : value["loot"].items()) {
			ItemId::value itemId = ItemId::from_string(key);
			if (!itemId) {
				WARNING("Unrecognised ItemId:", key);
				continue;
			}
			pair range(value[0], value[1]);
			templates[animalId].lootTable.addLoot({itemId, range});
		}
	}
}