
#include "ResourceTemplates.hpp"
#include "Window.hpp"
#include "json.hpp"

std::array<std::unique_ptr<ResourceTemplate>, ResourceId::count> ResourceTemplate::templates = {};

using namespace nlohmann;

void ResourceTemplate::setTemplates() {
	std::ifstream file(Window::instance->root / "json/Resources.json");
	if (!file) ERROR("File not found");
	json data = json::parse(file);
	file.close();

	for (auto &[key, value] : data.items()) {
		ResourceId::value resourceId = ResourceId::from_string(key);
		if (!resourceId) {
			WARNING("Unrecognised Resource:", key);
			continue;
		}

		templates[resourceId] = std::make_unique<ResourceTemplate>();

		for (auto& sprite : value["sprites"]) {
			pair anker(sprite["anker"][0], sprite["anker"][1]);
			pair size(sprite["size"][0], sprite["size"][1]);
			pair offset(sprite["offset"][0], sprite["offset"][1]);
			uchar variations = sprite["variations"];
			templates[resourceId]->spriteTemplates.push_back({anker, size, offset, variations});
		}

		if (value.contains("size")) templates[resourceId]->size = pair(value["size"][0], value["size"][1]);
		if (value.contains("solid")) templates[resourceId]->solid = value["solid"];
		if (value.contains("opaque")) templates[resourceId]->opaque = value["opaque"];
		if (value.contains("tool")) templates[resourceId]->toolId = ItemKind::from_string(value["tool"]);
		if (value.contains("level")) templates[resourceId]->level = value["level"];
		if (value.contains("health")) templates[resourceId]->health = value["health"];
		if (value.contains("sound")) templates[resourceId]->sound = SoundId::from_string(value["sound"]);

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
