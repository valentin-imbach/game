
#include "ResourceTemplates.hpp"
#include "json.hpp"

std::array<std::unique_ptr<ResourceTemplate>, ResourceId::count> ResourceTemplate::templates = {};

using namespace nlohmann;

void ResourceTemplate::setTemplates() {
    std::ifstream file("../json/Resources.json");
	if (!file) ERROR("File not found");
	json data = json::parse(file);

	for (auto& [key, value] : data.items()) {
        ResourceId::value resourceId = ResourceId::from_string(key);
        if (!resourceId) {
            WARNING("Unrecognised Resource:", key);
            continue;
        }

        ToolId::value tool = ToolId::from_string(value["tool"]);
        int level = value["level"];
        pair size(value["size"][0], value["size"][1]);
        pair position(value["position"][0], value["position"][1]);
        int height = value["height"];
        
        templates[resourceId] = std::make_unique<ResourceTemplate>(size, position, height, tool, level);

        for (auto& [key, value] : value["loot"].items()) {
            ItemId::value itemId = ItemId::from_string(key);
            if (!itemId) {
                WARNING("Unrecognised ItemId:", key);
                continue;
            }
            pair range(value[0], value[1]);
            templates[resourceId] -> lootTable.addLoot({itemId, range});
        }
    }
}
