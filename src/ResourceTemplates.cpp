
#include "ResourceTemplates.hpp"
#include "json.hpp"

std::array<std::unique_ptr<ResourceTemplate>, ResourceId::count> ResourceTemplate::templates = {};

using namespace nlohmann;

void ResourceTemplate::setTemplates() {
	std::ifstream file("../json/Resources.json");
	if (!file) ERROR("File not found");
	json data = json::parse(file);

	for (auto &[key, value] : data.items()) {
		ResourceId::value resourceId = ResourceId::from_string(key);
		if (!resourceId) {
			WARNING("Unrecognised Resource:", key);
			continue;
		}

		pair anker(value["anker"][0], value["anker"][1]);

		pair size(1, 1);
		if (value.contains("size")) size = pair(value["size"][0], value["size"][1]);
        
		int height = 0;
		if (value.contains("height")) height = value["height"];

		templates[resourceId] = std::make_unique<ResourceTemplate>(anker, size, height);

		if (value.contains("variations")) templates[resourceId]->variations = value["variations"];
		if (value.contains("solid")) templates[resourceId]->solid = value["solid"];
		if (value.contains("tool")) templates[resourceId]->toolId = ToolId::from_string(value["tool"]);
		if (value.contains("level")) templates[resourceId]->level = value["level"];
		if (value.contains("health")) templates[resourceId]->health = value["health"];

		for (auto &[key, value] : value["loot"].items()) {
			ItemId::value itemId = ItemId::from_string(key);
			if (!itemId) {
				WARNING("Unrecognised ItemId:", key);
				continue;
			}
			pair range(value[0], value[1]);
			templates[resourceId]->lootTable.addLoot({itemId, range});
		}
	}
}
