
#pragma once
#include "utils.hpp"
#include "ECS_types.hpp"
#include "Item_types.hpp"
#include "ResourceTemplates.hpp"
#include "World.hpp"

class ECS;

ENUM(AnimalId,
COW,
PIG,
HORSE)

class EntityFactory {
public:
	EntityFactory() = delete;
	static World* world;

	static Entity createPlayer(vec position);
	static Entity createCamera(vec position, uint8_t zoom);
	static Entity createAnimal(AnimalId::value animalId, vec position);
	static Entity createResource(ResourceId::value resourceId, pair position);
	
	static Entity createItem(ItemId::value itemId, uint8_t count);
	static Entity createItem(ItemId::value itemId, uint8_t count, vec position);

private:
	static bool free(pair position, pair size);
	static uint seed;
};