
#include "Tile.hpp"
#include "Sprite.hpp"
#include "Window.hpp"
#include "json.hpp"

std::array<GroundTemplate, GroundId::count> GroundTemplate::templates = {};

Tile::Tile(GroundId::value groundId, GroundId::value wallId) : groundId(groundId), wallId(wallId) {
	sprites = {};
}

void GroundTemplate::setTemplates() {
	std::ifstream file(Window::instance->root / "json/Grounds.json");
	if (!file) ERROR("File not found");
	nlohmann::json data = nlohmann::json::parse(file);
	file.close();

	GroundTemplate::templates = {};

	for (auto &[key, value] : data.items()) {
		GroundId::value groundId = GroundId::from_string(key);
		if (!groundId) {
			WARNING("Unrecognised Tile:", key);
			continue;
		}

		if (value.contains("sprite")) templates[groundId].spriteSheet = SpriteSheet::from_string(value["sprite"]);
		if (value.contains("colour")) {
			uint r = value["colour"][0];
			uint g = value["colour"][1];
			uint b = value["colour"][2];
			uint a = value["colour"][3];
			
			templates[groundId].colour = (r << 24) | (g << 16) | (b << 8) | a;
		}
		if (value.contains("walk")) templates[groundId].walk = value["walk"];
		if (value.contains("build")) templates[groundId].build = value["build"];
		if (value.contains("liquid")) templates[groundId].liquid = value["liquid"];
		if (value.contains("speed")) templates[groundId].speed = value["speed"];
		if (value.contains("frames")) templates[groundId].frames = value["frames"];
		if (value.contains("wall")) templates[groundId].wall = value["wall"];
	}
}