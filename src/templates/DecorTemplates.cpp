
#include "DecorTemplates.hpp"
#include "json.hpp"

std::array<DecorTemplate, ResourceId::count> DecorTemplate::templates = {};

void DecorTemplate::setTemplates(std::filesystem::path root) {
	json::Value data = json::parseFile(root / "json/Decorations.json");
	DecorTemplate::templates = {};

	for (auto [key, value] : data.get<json::Object>()) {
		DecorId::value decorId = DecorId::from_string(key);
		if (!decorId) {
			WARNING("Unrecognised Resource:", key);
			continue;
		}

		for (auto sprite : value["sprites"].get<json::Array>()) {
			pair anker = parsePair(sprite["anker"]);
			pair size = parsePair(sprite["size"]);
			pair offset = parsePair(sprite["offset"]);
			uchar variations = int(sprite["variations"]);
			templates[decorId].spriteTemplates.push_back({anker, size, offset, variations});
		}

		if (value["size"]) templates[decorId].size = parsePair(value["size"]);
		if (value["solid"]) templates[decorId].solid = value["solid"].get<bool>();
		if (value["opaque"]) templates[decorId].opaque = value["opaque"].get<bool>();
	}
}
