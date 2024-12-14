
#include "AnimalTemplates.hpp"
#include "json.hpp"
#include "Window.hpp"

std::array<AnimalTemplate, AnimalId::count> AnimalTemplate::templates = {};

void AnimalTemplate::setTemplates() {

	json::Value data = json::parseFile(Window::instance->root / "json/Animals.json");

	AnimalTemplate::templates = {};

	for (auto &[key, value] : data.get<json::Object>()) {
		AnimalId::value animalId = AnimalId::from_string(key);
		if (!animalId) {
			WARNING("Unrecognised Animal:", key);
			continue;
		}

		auto sprites = value["sprites"];

		SpriteSheet::value spriteSheet = SpriteSheet::from_string(std::string(sprites["sheet"]));
		pair size = parsePair(sprites["size"]);
		pair offset = parsePair(sprites["offset"]);
		
		for (auto &[key, value] : sprites["stack"].get<json::Object>()) {
			MovementState::value movementState = MovementState::from_string(key);
			if (!movementState) {
				WARNING("Unrecognised MovementState:", key);
				continue;
			}

			uchar frames = int(value["frames"]);
			for (auto &[l, src] : value["layers"].get<json::Object>()) {
				pair source = parsePair(src);
				CreatureLayer::value layer = CreatureLayer::from_string(l);
				templates[animalId].sprites[movementState].setSprite(layer, Sprite(spriteSheet, source, size, frames, 100), offset);
			}
		}

		// for (auto &[key, value] : sprites["actions"].get<json::Object>()) {
		// 	ActionState::value actionState = ActionState::from_string(key);
		// 	if (!actionState) {
		// 		WARNING("Unrecognised ActionState:", key);
		// 		continue;
		// 	}
		// 	pair source = parsePair(value["source"]);
		// 	uchar frames = int(value["frames"]);
		// 	templates[animalId].actions[actionState] = Sprite(spriteSheet, source, size, frames, 100);
		// }

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