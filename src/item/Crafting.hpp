
#pragma once
#include "utils.hpp"
#include "Item_types.hpp"
#include "ECS_types.hpp"
#include "Sprite.hpp"

ENUM(CraftingRecipeId,
AXE,
PICK_AXE,
SWORD)

ENUM(BuildRecipeId,
CHEST,
WORK_TABLE)

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

struct CraftingKindIngredient {
	CraftingKindIngredient(ItemKind::value itemKind, uint count) : itemKind(itemKind), count(count) {}
	ItemKind::value itemKind;
	uint count;
	bool check(Entity item);
	Entity take(Entity item);
};

struct CraftingProductProperty {
	ItemProperty::value property;
	std::vector<std::pair<ItemProperty::value, int>> factors;
};

struct CraftingKindProduct {
	std::string name;
	std::vector<SpriteTemplate> spriteTemplates;
	std::vector<ItemKind::value> productKinds;
	std::vector<CraftingProductProperty> productProperties;
};

struct CraftingKindRecipe {
	std::vector<CraftingKindIngredient> ingredients;
	CraftingKindProduct product;

	static std::array<CraftingKindRecipe, CraftingRecipeId::count> recipes;
	static void setRecipes();
};

//* Build

struct BuildKindRecipe {
	std::vector<CraftingKindIngredient> ingredients;
	int stationId;
	static std::array<BuildKindRecipe, BuildRecipeId::count> recipes;
	static void setRecipes();
};