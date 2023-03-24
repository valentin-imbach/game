
#include "Crafting.hpp"
#include "Components.hpp"
#include "ECS.hpp"
#include "EntityFactory.hpp"
#include "World.hpp"

std::vector<std::unique_ptr<CraftingRecipe>> CraftingRecipe::recipes = {};

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