
#include "ItemTemplates.hpp"
#include "json.hpp"
#include "Window.hpp"
#include <fstream>

std::array<ItemPropertyTemplate, ItemProperty::count> ItemPropertyTemplate::templates;
std::array<ItemKindTemplate, ItemKind::count> ItemKindTemplate::templates;
std::array<ItemTemplate, ItemId::count> ItemTemplate::templates;

void ItemPropertyTemplate::setTemplates() {
	std::ifstream file(Window::instance->root / "json/ItemProperties.json");
	if (!file) ERROR("File not found");
	nlohmann::json data = nlohmann::json::parse(file);

	for (auto& [key, value] : data.items()) {
        ItemProperty::value property = ItemProperty::from_string(key);
        if (!property) {
            WARNING("Unrecognised ItemProperty:", key);
            continue;
        }

        templates[property].name = value["name"];
    }
}

void ItemKindTemplate::setTemplates() {
	std::ifstream file(Window::instance->root / "json/ItemKinds.json");
	if (!file) ERROR("File not found");
	nlohmann::json data = nlohmann::json::parse(file);
    file.close();

	for (auto& [key, value] : data.items()) {
        ItemKind::value kind = ItemKind::from_string(key);
        if (!kind) {
            WARNING("Unrecognised ItemKind:", key);
            continue;
        }

        templates[kind].name = value["name"];
		for (auto& prop : value["properties"]) {
            ItemProperty::value property = ItemProperty::from_string(prop);
            if (!property) {
                WARNING("Unrecognised ItemProperty", prop);
                continue;
            }
            templates[kind].properties.set(property, true);
        }
    }
}

void ItemTemplate::setTemplates() {	
	std::ifstream file(Window::instance->root / "json/Items.json");
	if (!file) ERROR("File not found");
	nlohmann::json data = nlohmann::json::parse(file);
    file.close();

	for (auto& [key, value] : data.items()) {
        ItemId::value id = ItemId::from_string(key);
        if (!id) {
            WARNING("Unrecognised ItemKind:", key);
            continue;
        }

        templates[id].name = value["name"];
		for (auto& k : value["kinds"]) {
            ItemKind::value kind = ItemKind::from_string(k);
            if (!kind) {
                WARNING("Unrecognised ItemProperty", k);
                continue;
            }
            templates[id].kinds.set(kind, true);
        }

        for (auto& [pkey, pvalue] : value["properties"].items()) {
            ItemProperty::value property = ItemProperty::from_string(pkey);
            if (!property) {
                WARNING("Unrecognised ItemProperty", pkey);
                continue;
            }
            templates[id].properties[property] = pvalue;
        }
    }
}
