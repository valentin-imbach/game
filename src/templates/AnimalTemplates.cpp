
#include "AnimalTemplates.hpp"
#include "json.hpp"
#include "Window.hpp"

std::array<AnimalTemplate, AnimalId::count> AnimalTemplate::templates = {};

void AnimalTemplate::setTemplates() {
	// std::ifstream file(Window::instance->root / "json/Animals.json");
	// if (!file) ERROR("File not found");
	// nlohmann::json data = nlohmann::json::parse(file);
	// file.close();

	json::Value data = json::parseFile(Window::instance->root / "json/Animals.json");

	AnimalTemplate::templates = {};

	for (auto &[key, value] : data.get<json::Object>()) {
		AnimalId::value animalId = AnimalId::from_string(key);
		if (!animalId) {
			WARNING("Unrecognised Animal:", key);
			continue;
		}

		SpriteSheet::value spriteSheet = SpriteSheet::from_string(std::string(value["sheet"]));
		
		for (auto &[key, value] : value["sprites"].get<json::Object>()) {
			MovementState::value movementState = MovementState::from_string(key);
			if (!movementState) {
				WARNING("Unrecognised MovementState:", key);
				continue;
			}
			pair source1 = parsePair(value["source"][0]);
			pair source2 = parsePair(value["source"][1]);
			pair size = parsePair(value["size"]);
			uchar frames = int(value["frames"]);
			templates[animalId].sprites[movementState].first = Sprite(spriteSheet, source1, size, frames, 100);
			templates[animalId].sprites[movementState].second = Sprite(spriteSheet, source2, size, frames, 100);
		}

		auto col = value["collider"];
		templates[animalId].collider = Shape(parseVec(col["size"]));
		if (col["offset"]) templates[animalId].collider.offset = parseVec(col["offset"]);

		auto hit = value["hitbox"];
		templates[animalId].hitbox = Shape(parseVec(hit["size"]));
		if (hit["offset"]) templates[animalId].hitbox.offset = parseVec(hit["offset"]);

		if (value["health"]) templates[animalId].health = int(value["health"]);
		if (value["sound"]) templates[animalId].sound = SoundId::from_string(std::string(value["sound"]));
		if (value["speed"]) templates[animalId].speed = parseVec(value["speed"]);

		for (auto &[key, value] : value["loot"].get<json::Object>()) {
			ItemId::value itemId = ItemId::from_string(key);
			if (!itemId) {
				WARNING("Unrecognised ItemId:", key);
				continue;
			}
			pair range = parsePair(value);
			templates[animalId].lootTable.addLoot({itemId, range});
		}
	}
}