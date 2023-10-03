
#include "Tile.hpp"
#include "Sprite.hpp"
#include "Window.hpp"
#include "json.hpp"

std::array<TileTemplate, TileId::count> TileTemplate::templates = {};

Tile::Tile(TileId::value tileId) : tileId(tileId) {
	sprites = {};
}

void TileTemplate::setTemplates() {
	std::ifstream file(Window::instance->root / "json/Tiles.json");
	if (!file) ERROR("File not found");
	nlohmann::json data = nlohmann::json::parse(file);
	file.close();

	TileTemplate::templates = {};

	for (auto &[key, value] : data.items()) {
		TileId::value tileId = TileId::from_string(key);
		if (!tileId) {
			WARNING("Unrecognised Tile:", key);
			continue;
		}

		if (value.contains("sprite")) templates[tileId].spriteSheet = SpriteSheet::from_string(value["sprite"]);
		if (value.contains("colour")) {
			uint r = value["colour"][0];
			uint g = value["colour"][1];
			uint b = value["colour"][2];
			uint a = value["colour"][3];
			
			templates[tileId].colour = (r << 24) | (g << 16) | (b << 8) | a;
		}
		if (value.contains("walk")) templates[tileId].walk = value["walk"];
		if (value.contains("build")) templates[tileId].build = value["build"];
		if (value.contains("liquid")) templates[tileId].liquid = value["liquid"];
		if (value.contains("speed")) templates[tileId].speed = value["speed"];
		if (value.contains("frames")) templates[tileId].frames = value["frames"];
		if (value.contains("wall")) templates[tileId].wall = value["wall"];
	}
}