
#pragma once
#include "utils.hpp"
#include "ECS_types.hpp"

class ECS;
class Item;

ENUM(AnimalId,
COW,
PIG,
HORSE)

ENUM(ResourceId,
ROCK,
TREE,
GRASS)

class EntityFactory {
public:
	EntityFactory() = delete;

	static ECS* ecs;
	static GridMap* gridMap;

	static Entity createPlayer(vec position);
	static Entity createCamera(vec position, uint8_t zoom);
	static Entity createItemEntity(Item item, vec position);
	static Entity createAnimal(AnimalId::value animalId, vec position);
	static Entity createResource(ResourceId::value resourceId, pair position);
};