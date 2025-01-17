
#include "ItemTemplates.hpp"
#include "json.hpp"

std::array<ItemPropertyTemplate, ItemProperty::count> ItemPropertyTemplate::templates = {};
std::array<ItemKindTemplate, ItemKind::count> ItemKindTemplate::templates = {};
std::array<ItemTemplate, ItemId::count> ItemTemplate::templates = {};

void ItemPropertyTemplate::setTemplates(std::filesystem::path root) {
    json::Value data = json::parseFile(root / "json/ItemProperties.json");

    ItemPropertyTemplate::templates = {};

	for (auto& [key, value] : data.get<json::Object>()) {
        ItemProperty::value property = ItemProperty::from_string(key);
        if (!property) {
            WARNING("Unrecognised ItemProperty:", key);
            continue;
        }

        templates[property].name = std::string(value["name"]);
    }
}

void ItemKindTemplate::setTemplates(std::filesystem::path root) {
    json::Value data = json::parseFile(root / "json/ItemKinds.json");

    ItemKindTemplate::templates = {};

	for (auto& [key, value] : data.get<json::Object>()) {
        ItemKind::value kind = ItemKind::from_string(key);
        if (!kind) {
            WARNING("Unrecognised ItemKind:", key);
            continue;
        }

        templates[kind].name = std::string(value["name"]);
		for (auto& prop : value["properties"].get<json::Array>()) {
            ItemProperty::value property = ItemProperty::from_string(std::string(prop));
            if (!property) {
                WARNING("Unrecognised ItemProperty", std::string(prop));
                continue;
            }
            templates[kind].properties.set(property, true);
        }
    }
}

void ItemTemplate::setTemplates(std::filesystem::path root) {	
    json::Value data = json::parseFile(root / "json/Items.json");

    ItemTemplate::templates = {};

	for (auto& [key, value] : data.get<json::Object>()) {
        ItemId::value id = ItemId::from_string(key);
        if (!id) {
            WARNING("Unrecognised ItemKind:", key);
            continue;
        }

        if (value["value"]) templates[id].value = int(value["value"]);

        templates[id].name = std::string(value["name"]);
		for (auto& k : value["kinds"].get<json::Array>()) {
            ItemKind::value kind = ItemKind::from_string(std::string(k));
            if (!kind) {
                WARNING("Unrecognised ItemProperty", std::string(k));
                continue;
            }
            templates[id].kinds.set(kind, true);
        }

        for (auto& [pkey, pvalue] : value["properties"].get<json::Object>()) {
            ItemProperty::value property = ItemProperty::from_string(pkey);
            if (!property) {
                WARNING("Unrecognised ItemProperty", pkey);
                continue;
            }
            templates[id].properties[property] = int(pvalue);
        }
    }
}
