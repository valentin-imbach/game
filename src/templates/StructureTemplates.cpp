
#include "StructureTemplates.hpp"
#include "json.hpp"

std::array<StructureTemplate, StructureId::count> StructureTemplate::templates = {};

void StructureTemplate::setTemplates(std::filesystem::path root) {
	json::Value data = json::parseFile(root / "json/Structures.json");

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
