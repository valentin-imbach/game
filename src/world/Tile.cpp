
#include "Tile.hpp"
#include "Sprite.hpp"
#include "Window.hpp"
#include "json.hpp"

std::array<GroundTemplate, GroundId::count> GroundTemplate::templates = {};
std::array<WallTemplate, WallId::count> WallTemplate::templates = {};

Tile::Tile(GroundId::value groundId, WallId::value wallId) : groundId(groundId), wallId(wallId) {
	sprites = {};
}

void GroundTemplate::setTemplates() {
	json::Value data = json::parseFile(Window::instance->root / "json/Grounds.json");

	GroundTemplate::templates = {};

	for (auto &[key, value] : data.get<json::Object>()) {
		GroundId::value groundId = GroundId::from_string(key);
		if (!groundId) {
			WARNING("Unrecognised ground:", key);
			continue;
		}

		if (value["sprite"]) templates[groundId].spriteSheet = SpriteSheet::from_string(std::string(value["sprite"]));
		if (value["sprite2"]) templates[groundId].spriteSheet2 = SpriteSheet::from_string(std::string(value["sprite2"]));
		if (value["colour"]) {
			uint r = int(value["colour"][0]);
			uint g = int(value["colour"][1]);
			uint b = int(value["colour"][2]);
			uint a = int(value["colour"][3]);
			
			templates[groundId].colour = (r << 24) | (g << 16) | (b << 8) | a;
		}
		if (value["walk"]) templates[groundId].walk = value["walk"].get<bool>();
		if (value["build"]) templates[groundId].build = value["build"].get<bool>();
		if (value["liquid"]) templates[groundId].liquid = value["liquid"].get<bool>();
		if (value["speed"]) templates[groundId].speed = float(value["speed"]);
		if (value["frames"]) templates[groundId].frames = int(value["frames"]);
	}
}

void WallTemplate::setTemplates() {
	json::Value data = json::parseFile(Window::instance->root / "json/Walls.json");

	WallTemplate::templates = {};

	for (auto &[key, value] : data.get<json::Object>()) {
		WallId::value wallId = WallId::from_string(key);
		if (!wallId) {
			WARNING("Unrecognised wall:", key);
			continue;
		}

		if (value["sprite"]) templates[wallId].spriteSheet = SpriteSheet::from_string(std::string(value["sprite"]));
		if (value["colour"]) {
			uint r = int(value["colour"][0]);
			uint g = int(value["colour"][1]);
			uint b = int(value["colour"][2]);
			uint a = int(value["colour"][3]);
			
			templates[wallId].colour = (r << 24) | (g << 16) | (b << 8) | a;
		}
		if (value["climb"]) templates[wallId].climb = value["walk"].get<bool>();
		if (value["build"]) templates[wallId].build = value["build"].get<bool>();
		if (value["liquid"]) templates[wallId].liquid = value["liquid"].get<bool>();
		if (value["frames"]) templates[wallId].frames = int(value["frames"]);
	}
}