
#pragma once
#include "utils.hpp"
#include "Item_types.hpp"
#include "ECS_types.hpp"

struct CraftingIngredient {
	ItemId::value itemId;
	uint count;
	bool check(Entity item);
	Entity take(Entity item);
};

struct CraftingProduct {
	ItemId::value itemId;
	uint count;
};

struct CraftingRecipe {
	std::vector<CraftingIngredient> ingredients;
	CraftingProduct product;

	static std::vector<std::unique_ptr<CraftingRecipe>> recipes;
	static void setRecipes();
};