
#include "Generation.hpp"
#include "json.hpp"

using namespace nlohmann;

std::array<std::unique_ptr<BiomeTemplate>, Biome::count> BiomeTemplate::templates = {};

BiomeTemplate::BiomeTemplate(std::string name) : name(name) {}
BiomeGroundTemplate::BiomeGroundTemplate(TileId::value tileId) : tileId(tileId) {}

BiomeGroundTemplate* BiomeTemplate::getGround(int variation) {
	if (grounds.size() > 1 && variation < 30) return grounds[1].get();
	return grounds[0].get();
}

void BiomeTemplate::setTemplates() {
	std::ifstream file("../json/Generation.json");
	if (!file) ERROR("File not found");
	json data = json::parse(file);

	for (auto& [biome_key, biome_value] : data.items()) {
        Biome::value biome = Biome::from_string(biome_key);
        if (!biome) {
            WARNING("Unrecognised Biome:", biome_key);
            continue;
        }

		templates[biome] = std::make_unique<BiomeTemplate>("");

		for (auto& ground : biome_value["grounds"]) {
			TileId::value tileId = TileId::from_string(ground["tile"]);
			if (!tileId) {
				WARNING("Unrecognised TileId:", ground["tile"]);
				continue;
			}

			std::unique_ptr<BiomeGroundTemplate> biomeGround = std::make_unique<BiomeGroundTemplate>(tileId);

			for (auto& [resource_key, resource_value] : ground["resources"].items()) {
				ResourceId::value resourceId = ResourceId::from_string(resource_key);
				if (!resourceId) {
					WARNING("Unrecognised ResourceId:", resource_key);
					continue;
				}

				biomeGround->resources.emplace_back(resourceId, resource_value);
			}
			
			templates[biome]->grounds.push_back(std::move(biomeGround));
		}
    }
}