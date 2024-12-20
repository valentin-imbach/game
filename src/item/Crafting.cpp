
#include "Crafting.hpp"
#include "Components.hpp"
#include "ECS.hpp"
#include "EntityFactory.hpp"
#include "World.hpp"
#include "json.hpp"

// using namespace nlohmann;

std::vector<std::unique_ptr<CraftingRecipe>> CraftingRecipe::recipes = {};
std::array<CraftingKindRecipe, CraftingRecipeId::count> CraftingKindRecipe::recipes = {};

std::array<BuildKindRecipe, BuildRecipeId::count> BuildKindRecipe::recipes = {};

void CraftingRecipe::setRecipes() {
	std::unique_ptr<CraftingRecipe> recipe = std::make_unique<CraftingRecipe>();
	recipe->product = {ItemId::OAK_PLANK, 4};
	recipe->ingredients.push_back({ItemId::OAK_LOG, 1});
	recipe->ingredients.push_back({ItemId::OAK_LOG, 1});
	recipes.push_back(std::move(recipe));
}

bool CraftingIngredient::check(Entity item) {
	if (!item) return false;
	ItemComponent &itemComponent = EntityFactory::world->ecs.getComponent<ItemComponent>(item);
	return itemComponent.itemId == itemId && itemComponent.count >= count;
}

Entity CraftingIngredient::take(Entity item) {
	ItemComponent &itemComponent = EntityFactory::world->ecs.getComponent<ItemComponent>(item);
	itemComponent.count -= count;
	if (itemComponent.count <= 0) {
		EntityFactory::world->ecs.addComponent(DeathComponent(), item);
		return 0;
	}
	return item;
}

void CraftingKindRecipe::setRecipes() {
	// std::ifstream file(Window::instance->root / "json/CraftingRecipes.json");
	// if (!file) ERROR("File not found");
	// json data = json::parse(file);
	// file.close();

	json::Value data = json::parseFile(Window::instance->root / "json/CraftingRecipes.json");

	for (auto &[recipe, value] : data.get<json::Object>()) {
		CraftingRecipeId::value recipeId = CraftingRecipeId::from_string(recipe);
		if (!recipeId) {
			WARNING("Unrecognised crafting recipe id:", recipe);
			continue;
		}

		for (auto& sprite : value["product"]["sprites"].get<json::Array>()) {
			pair anker = parsePair(sprite["anker"]);
			pair size = parsePair(sprite["size"]);
			pair offset = parsePair(sprite["offset"]);
			uchar variations = int(sprite["variations"]);
			recipes[recipeId].product.spriteTemplates.push_back({anker, size, offset, variations});
		}

		recipes[recipeId].product.name = std::string(value["product"]["name"]);

		for (auto& [kind, props] : value["product"]["kinds"].get<json::Object>()) {
			ItemKind::value itemKind = ItemKind::from_string(kind);
			if (!itemKind) {
				WARNING("Unrecognised item kind:", kind);
				// LOG(props.print());
				continue;
			}
			recipes[recipeId].product.productKinds.push_back(itemKind);
			for (auto &[property, factors] : props.get<json::Object>()) {
				CraftingProductProperty prodProp;
				ItemProperty::value itemProp = ItemProperty::from_string(property);
				if (!itemProp) {
					WARNING("Unrecognised item property:", property);
					continue;
				}
				prodProp.property = itemProp;
				for (auto& fact : factors.get<json::Array>()) {
					if (fact.size() == 0) {
						prodProp.factors.emplace_back();
						continue;
					}

					ItemProperty::value factorProp = ItemProperty::from_string(std::string(fact[0]));
					if (!factorProp) {
						WARNING("Unrecognised item property:", std::string(fact[0]));
						continue;
					}
					prodProp.factors.emplace_back(factorProp, int(fact[1]));
				}
				//LOG(prodProp.factors.size());
				recipes[recipeId].product.productProperties.push_back(prodProp);
			}
		}

		for (auto &ing : value["ingredients"].get<json::Array>()) {
			ItemKind::value itemKind = ItemKind::from_string(std::string(ing[0]));
			if (!itemKind) {
				WARNING("Unrecognised item kind:", std::string(ing[0]));
				continue;
			}
			recipes[recipeId].ingredients.emplace_back(itemKind, int(ing[1]));
		}
	}
}


void BuildKindRecipe::setRecipes() {
	recipes[1].ingredients.push_back({ItemKind::LOG, 5});
	recipes[1].ingredients.push_back({ItemKind::PLATE, 8});
	recipes[1].stationId = StationId::CHEST;

	recipes[2].ingredients.push_back({ItemKind::LOG, 4});
	recipes[2].ingredients.push_back({ItemKind::PLATE, 6});
	recipes[2].ingredients.push_back({ItemKind::PEBBLE, 4});
	recipes[2].stationId = StationId::WORK_STATION;
}

bool CraftingKindIngredient::check(Entity item) {
	if (!item || !EntityFactory::world->ecs.hasComponent<ItemComponent>(item)) return false;
	ItemComponent& itemComponent = EntityFactory::world->ecs.getComponent<ItemComponent>(item);
	if (itemComponent.count < count) return false;
	if (EntityFactory::world->ecs.hasComponent<ItemKindComponent>(item)) {
		return EntityFactory::world->ecs.getComponent<ItemKindComponent>(item).itemKinds[itemKind];
	}
	return (itemComponent.itemId && ItemTemplate::templates[itemComponent.itemId].kinds[itemKind]);
}

Entity CraftingKindIngredient::take(Entity item) {
	ItemComponent &itemComponent = EntityFactory::world->ecs.getComponent<ItemComponent>(item);
	itemComponent.count -= count;
	if (itemComponent.count <= 0) {
		EntityFactory::world->ecs.addComponent(DeathComponent(), item);
		return 0;
	}
	return item;
}