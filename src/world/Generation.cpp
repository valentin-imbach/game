
#include "Generation.hpp"
#include "Window.hpp"
#include "json.hpp"

std::array<BiomeTemplate, Biome::count> BiomeTemplate::templates = {};

BiomeGroundTemplate& BiomeTemplate::getGround(int variation) {
	if (grounds.size() > 1 && variation < 30) return grounds[1];
	return grounds[0];
}

void BiomeTemplate::setTemplates() {
	std::ifstream file(Window::instance->root / "json/Generation.json");
	if (!file) ERROR("File not found");
	nlohmann::json data = nlohmann::json::parse(file);
	file.close();

	BiomeTemplate::templates = {};

	for (auto& [biome_key, biome_value] : data.items()) {
        Biome::value biome = Biome::from_string(biome_key);
        if (!biome) {
            WARNING("Unrecognised Biome:", biome_key);
            continue;
        }

		for (auto& ground : biome_value["grounds"]) {
			TileId::value tileId = TileId::from_string(ground["tile"]);
			if (!tileId) {
				WARNING("Unrecognised TileId:", ground["tile"]);
				continue;
			}

			BiomeGroundTemplate biomeGround;
			biomeGround.tileId = tileId;

			for (auto& [resource_key, resource_value] : ground["resources"].items()) {
				ResourceId::value resourceId = ResourceId::from_string(resource_key);
				if (!resourceId) {
					WARNING("Unrecognised ResourceId:", resource_key);
					continue;
				}
				biomeGround.resources.emplace_back(resourceId, resource_value);
			}
			
			templates[biome].grounds.push_back(biomeGround);
		}
    }
}