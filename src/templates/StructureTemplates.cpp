
#include "StructureTemplates.hpp"
#include "Window.hpp"
#include "json.hpp"

std::array<StructureTemplate, StructureId::count> StructureTemplate::templates = {};

void StructureTemplate::setTemplates() {
	std::ifstream file(Window::instance->root / "json/Structures.json");
	if (!file) ERROR("File not found");
	nlohmann::json data = nlohmann::json::parse(file);
	file.close();

	StructureTemplate::templates = {};

	for (auto &[key, value] : data.items()) {
		StructureId::value structureId = StructureId::from_string(key);
		if (!structureId) {
			WARNING("Unrecognised Resource:", key);
			continue;
		}

		templates[structureId].spriteSheet = SpriteSheet::from_string(value["sheet"]);

		for (auto& sprite : value["sprites"]) {
			pair anker(sprite["anker"][0], sprite["anker"][1]);
			pair size(sprite["size"][0], sprite["size"][1]);
			pair offset(sprite["offset"][0], sprite["offset"][1]);
			uchar variations = sprite["variations"];
			templates[structureId].spriteTemplates.push_back({anker, size, offset, variations});
		}

		if (value.contains("size")) templates[structureId].size = pair(value["size"][0], value["size"][1]);
	}
}
