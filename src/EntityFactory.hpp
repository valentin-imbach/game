
#pragma once
#include "utils.hpp"
#include "ECS_types.hpp"
#include "Item_types.hpp"
#include "ResourceTemplates.hpp"

class ECS;

ENUM(AnimalId,
COW,
PIG,
HORSE)

class EntityFactory {
public:
	EntityFactory() = delete;

	static ECS* ecs;
	static GridMap* gridMap;

	static Entity createPlayer(vec position);
	static Entity createCamera(vec position, uint8_t zoom);
	static Entity createItem(ItemId::value itemId, uint8_t count);
	static Entity createItem(ItemId::value itemId, uint8_t count, vec position);
	static Entity createAnimal(AnimalId::value animalId, vec position);
	static Entity createResource(ResourceId::value resourceId, pair position);
};