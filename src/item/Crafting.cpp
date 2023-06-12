
#include "Crafting.hpp"
#include "Components.hpp"
#include "ECS.hpp"
#include "EntityFactory.hpp"
#include "World.hpp"

std::vector<std::unique_ptr<CraftingRecipe>> CraftingRecipe::recipes = {};
std::vector<std::unique_ptr<CraftingKindRecipe>> CraftingKindRecipe::recipes = {};

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
	std::unique_ptr<CraftingKindRecipe> recipe = std::make_unique<CraftingKindRecipe>();
	recipe->ingredients.push_back({ItemKind::ROD, 1});
	recipe->ingredients.push_back({ItemKind::PLATE, 2});
	recipe->ingredients.push_back({ItemKind::ROPE, 4});
	recipe->product.name = "Axe";
	recipe->product.spriteTemplates = {{{2, 0}, {1, 1}, {0, 0}, 1}};
	recipe->product.productKinds = {ItemKind::AXE};
	recipe->product.productProperties.push_back({ItemProperty::EFFICIENCY, {{ItemProperty::FLEXIBILITY, 1},{},{}}});
	recipe->product.productProperties.push_back({ItemProperty::LEVEL, {{},{ItemProperty::STRENGTH, 1},{}}});
	recipe->product.productProperties.push_back({ItemProperty::DURABILITY, {{},{},{ItemProperty::STRENGTH, 1}}});
	recipes.push_back(std::move(recipe));
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