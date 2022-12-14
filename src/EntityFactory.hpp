
#pragma once
#include "utils.hpp"
#include "ECS_types.hpp"

enum class ItemId;
class ECS;
class Item;

enum class AnimalId {
	NONE,

	COW,
	PIG,
	HORSE,

	MAX
};

enum class ResourceId {
	NONE,

	ROCK,
	TREE,
	GRASS,

	MAX
};

class EntityFactory {
public:
	EntityFactory() = delete;

	static ECS* ecs;
	static GridMap* gridMap;

	static Entity createPlayer(vec position);
	static Entity createCamera(vec position, uint8_t zoom, Entity target = 0);
	static Entity createItemEntity(Item item, vec position);
	static Entity createAnimal(AnimalId animalId, vec position);
	static Entity createResource(ResourceId resourceId, pair position);
};