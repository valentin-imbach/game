
#include "Generation.hpp"
#include "Window.hpp"
#include "json.hpp"

std::array<BiomeTemplate, Biome::count> BiomeTemplate::templates = {};

BiomeGroundTemplate& BiomeTemplate::getGround(int variation) {
	if (grounds.size() > 1 && variation < 30) return grounds[1];
	return grounds[0];
}

void BiomeTemplate::setTemplates() {
	// std::ifstream file(Window::instance->root / "json/Generation.json");
	// if (!file) ERROR("File not found");
	// nlohmann::json data = nlohmann::json::parse(file);
	// file.close();

	json::Value data = json::parseFile(Window::instance->root / "json/Generation.json");

	BiomeTemplate::templates = {};

	for (auto& [biome_key, biome_value] : data.get<json::Object>()) {
        Biome::value biome = Biome::from_string(biome_key);
        if (!biome) {
            WARNING("Unrecognised Biome:", biome_key);
            continue;
        }

		for (auto ground : biome_value["grounds"].get<json::Array>()) {
			GroundId::value groundId = GroundId::from_string(std::string(ground["tile"]));
			if (!groundId) {
				WARNING("Unrecognised GroundId:", ground["tile"]);
				continue;
			}

			BiomeGroundTemplate biomeGround;
			biomeGround.groundId = groundId;

			for (auto [resource_key, resource_value] : ground["resources"].get<json::Object>()) {
				ResourceId::value resourceId = ResourceId::from_string(resource_key);
				if (!resourceId) {
					WARNING("Unrecognised ResourceId:", resource_key);
					continue;
				}
				biomeGround.resources.emplace_back(resourceId, int(resource_value));
			}
			
			templates[biome].grounds.push_back(biomeGround);
		}
    }
}