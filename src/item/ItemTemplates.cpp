
#include "ItemTemplates.hpp"
#include "json.hpp"
#include <fstream>

std::array<std::unique_ptr<ItemPropertyTemplate>, ItemProperty::count + 1> ItemPropertyTemplate::templates;
std::array<std::unique_ptr<ItemKindTemplate>, ItemKind::count + 1> ItemKindTemplate::templates;
std::array<std::unique_ptr<ItemTemplate>, ItemId::count + 1> ItemTemplate::templates;

using namespace nlohmann;

void ItemPropertyTemplate::setTemplates() {
	std::ifstream file("../json/ItemProperties.json");
	if (!file) ERROR("File not found");
	json data = json::parse(file);

	for (auto& [key, value] : data.items()) {
        ItemProperty::value property = ItemProperty::from_string(key);
        if (!property) {
            WARNING("Unrecognised ItemProperty:", key);
            continue;
        }

        std::string name = value["name"];
        templates[property] = std::make_unique<ItemPropertyTemplate>(name);
    }
}

void ItemKindTemplate::setTemplates() {
	std::ifstream file("../json/ItemKinds.json");
	if (!file) ERROR("File not found");
	json data = json::parse(file);

	for (auto& [key, value] : data.items()) {
        ItemKind::value kind = ItemKind::from_string(key);
        if (!kind) {
            WARNING("Unrecognised ItemKind:", key);
            continue;
        }

        std::string name = value["name"];
		ItemPropertyBitset itemProperties;
		for (auto& prop : value["properties"]) {
            ItemProperty::value property = ItemProperty::from_string(prop);
            if (!property) {
                WARNING("Unrecognised ItemProperty", prop);
                continue;
            }
            itemProperties.set(property, true);
        }

        templates[kind] = std::make_unique<ItemKindTemplate>(name, itemProperties);
    }
}

void ItemTemplate::setTemplates() {	
	std::ifstream file("../json/Items.json");
	if (!file) ERROR("File not found");
	json data = json::parse(file);

	for (auto& [key, value] : data.items()) {
        ItemId::value id = ItemId::from_string(key);
        if (!id) {
            WARNING("Unrecognised ItemKind:", key);
            continue;
        }

        std::string name = value["name"];

		ItemKindBitset itemKinds;
		for (auto& k : value["kinds"]) {
            ItemKind::value kind = ItemKind::from_string(k);
            if (!kind) {
                WARNING("Unrecognised ItemProperty", k);
                continue;
            }
            itemKinds.set(kind, true);
        }

        templates[id] = std::make_unique<ItemTemplate>(name, itemKinds);

        for (auto& [pkey, pvalue] : value["properties"].items()) {
            ItemProperty::value property = ItemProperty::from_string(pkey);
            if (!property) {
                WARNING("Unrecognised ItemProperty", pkey);
                continue;
            }
            templates[id]->properties[property] = pvalue;
        }
    }
}
