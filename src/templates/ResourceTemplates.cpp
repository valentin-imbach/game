
#include "ResourceTemplates.hpp"
#include "json.hpp"

std::array<ResourceTemplate, ResourceId::count> ResourceTemplate::templates = {};

void ResourceTemplate::setTemplates(std::filesystem::path root) {
	json::Value data = json::parseFile(root / "json/Resources.json");
	// LOG(data.print());
	ResourceTemplate::templates = {};

	for (auto [key, value] : data.get<json::Object>()) {
		ResourceId::value resourceId = ResourceId::from_string(key);
		if (!resourceId) {
			WARNING("Unrecognised Resource:", key);
			continue;
		}

		for (auto sprite : value["sprites"].get<json::Array>()) {
			pair anker = parsePair(sprite["anker"]);
			pair size = parsePair(sprite["size"]);
			pair offset = parsePair(sprite["offset"]);
			uchar variations = int(sprite["variations"]);
			templates[resourceId].spriteTemplates.push_back({anker, size, offset, variations});
		}

		if (value["size"]) templates[resourceId].size = parsePair(value["size"]);
		if (value["solid"]) templates[resourceId].solid = value["solid"].get<bool>();
		if (value["opaque"]) templates[resourceId].opaque = value["opaque"].get<bool>();
		if (value["tool"]) templates[resourceId].toolId = ItemKind::from_string(std::string(value["tool"]));
		if (value["level"]) templates[resourceId].level = int(value["level"]);
		if (value["health"]) templates[resourceId].health = int(value["health"]);
		if (value["sound"]) templates[resourceId].sound = SoundId::from_string(std::string(value["sound"]));

		for (auto [key, value] : value["loot"].get<json::Object>()) {
			ItemId::value itemId = ItemId::from_string(key);
			if (!itemId) {
				WARNING("Unrecognised ItemId:", key);
				continue;
			}
			pair range = parsePair(value);
			templates[resourceId].lootTable.addLoot({itemId, range});
		}
	}
}
