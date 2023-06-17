
#include "Crafting.hpp"
#include "Components.hpp"
#include "ECS.hpp"
#include "EntityFactory.hpp"
#include "World.hpp"
#include "json.hpp"

using namespace nlohmann;

std::vector<std::unique_ptr<CraftingRecipe>> CraftingRecipe::recipes = {};
std::array<CraftingKindRecipe, CraftingRecipeId::count> CraftingKindRecipe::recipes = {};

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
	std::ifstream file("../json/CraftingRecipes.json");
	if (!file) ERROR("File not found");
	json data = json::parse(file);
	file.close();

	for (auto &[recipe, value] : data.items()) {
		CraftingRecipeId::value recipeId = CraftingRecipeId::from_string(recipe);
		if (!recipeId) {
			WARNING("Unrecognised crafting recipe id:", recipe);
			continue;
		}

		for (auto& sprite : value["product"]["sprites"]) {
			pair anker(sprite["anker"][0], sprite["anker"][1]);
			pair size(sprite["size"][0], sprite["size"][1]);
			pair offset(sprite["offset"][0], sprite["offset"][1]);
			uchar variations = sprite["variations"];
			recipes[recipeId].product.spriteTemplates.push_back({anker, size, offset, variations});
		}

		recipes[recipeId].product.name = value["product"]["name"];

		for (auto &[kind, props] : value["product"]["kinds"].items()) {
			ItemKind::value itemKind = ItemKind::from_string(kind);
			if (!itemKind) {
				WARNING("Unrecognised item kind:", itemKind);
				continue;
			}
			recipes[recipeId].product.productKinds.push_back(itemKind);
			for (auto &[property, factors] : props.items()) {
				CraftingProductProperty prodProp;
				ItemProperty::value itemProp = ItemProperty::from_string(property);
				if (!itemProp) {
					WARNING("Unrecognised item property:", property);
					continue;
				}
				prodProp.property = itemProp;
				for (auto& fact : factors) {
					if (fact.empty()) {
						prodProp.factors.emplace_back();
						continue;
					}
					ItemProperty::value factorProp = ItemProperty::from_string(fact[0]);
					if (!factorProp) {
						WARNING("Unrecognised item property:", fact[0]);
						continue;
					}
					prodProp.factors.emplace_back(factorProp, fact[1]);
				}
				//LOG(prodProp.factors.size());
				recipes[recipeId].product.productProperties.push_back(prodProp);
			}
		}

		for (auto &ing : value["ingredients"]) {
			ItemKind::value itemKind = ItemKind::from_string(ing[0]);
			if (!itemKind) {
				WARNING("Unrecognised item kind:", itemKind);
				continue;
			}
			recipes[recipeId].ingredients.emplace_back(itemKind, ing[1]);
		}
	}

// 	recipes[1].ingredients.push_back({ItemKind::ROD, 1});
// 	recipes[1].ingredients.push_back({ItemKind::PLATE, 2});
// 	recipes[1].ingredients.push_back({ItemKind::ROPE, 4});
// 	recipes[1].product.name = "Axe";
// 	recipes[1].product.spriteTemplates = {{{2, 0}, {1, 1}, {0, 0}, 1}};
// 	recipes[1].product.productKinds = {ItemKind::AXE};
// 	recipes[1].product.productProperties.push_back({ItemProperty::EFFICIENCY, {{ItemProperty::FLEXIBILITY, 1},{},{}}});
// 	recipes[1].product.productProperties.push_back({ItemProperty::LEVEL, {{},{ItemProperty::STRENGTH, 1},{}}});
// 	recipes[1].product.productProperties.push_back({ItemProperty::DURABILITY, {{},{},{ItemProperty::STRENGTH, 1}}});

// 	recipes[2].ingredients.push_back({ItemKind::ROD, 1});
// 	recipes[2].ingredients.push_back({ItemKind::PLATE, 4});
// 	recipes[2].product.name = "Pick Axe";
// 	recipes[2].product.spriteTemplates = {{{2, 0}, {1, 1}, {0, 0}, 1}};
// 	recipes[2].product.productKinds = {ItemKind::PICK_AXE};
// 	recipes[2].product.productProperties.push_back({ItemProperty::EFFICIENCY, {{ItemProperty::FLEXIBILITY, 1},{}}});
// 	recipes[2].product.productProperties.push_back({ItemProperty::LEVEL, {{},{ItemProperty::STRENGTH, 1}}});
// 	recipes[2].product.productProperties.push_back({ItemProperty::DURABILITY, {{},{ItemProperty::STRENGTH, 1}}});
}

bool CraftingKindIngredient::check(Entity item) {
	if (!item || !EntityFactory::world->ecs.hasComponent<ItemComponent>(item)) return false;
	ItemComponent& itemComponent = EntityFactory::world->ecs.getComponent<ItemComponent>(item);
	if (itemComponent.count < count) return false;
	if (EntityFactory::world->ecs.hasComponent<ItemKindComponent>(item)) {
		return EntityFactory::world->ecs.getComponent<ItemKindComponent>(item).itemKinds[itemKind];
	}
	return (itemComponent.itemId && ItemTemplate::templates[itemComponent.itemId]->kinds[itemKind]);
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