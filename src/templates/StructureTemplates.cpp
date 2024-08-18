
#include "StructureTemplates.hpp"
#include "Window.hpp"
#include "json.hpp"

std::array<StructureTemplate, StructureId::count> StructureTemplate::templates = {};

void StructureTemplate::setTemplates() {
	// std::ifstream file(Window::instance->root / "json/Structures.json");
	// if (!file) ERROR("File not found");
	// nlohmann::json data = nlohmann::json::parse(file);
	// file.close();

	json::Value data = json::parseFile(Window::instance->root / "json/Structures.json");

	StructureTemplate::templates = {};

	for (auto &[key, value] : data.get<json::Object>()) {
		StructureId::value structureId = StructureId::from_string(key);
		if (!structureId) {
			WARNING("Unrecognised Resource:", key);
			continue;
		}

		templates[structureId].spriteSheet = SpriteSheet::from_string(std::string(value["sheet"]));

		for (auto sprite : value["sprites"].get<json::Array>()) {
			pair anker = parsePair(sprite["anker"]);
			pair size = parsePair(sprite["size"]);
			pair offset = parsePair(sprite["offset"]);
			uchar variations = int(sprite["variations"]);
			templates[structureId].spriteTemplates.push_back({anker, size, offset, variations});
		}

		if (value["size"]) templates[structureId].size = parsePair(value["size"]);
	}
}
