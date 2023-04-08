
#pragma once
#include "utils.hpp"
#include "ECS_types.hpp"
#include "Item_types.hpp"
#include "ResourceTemplates.hpp"
#include "Components.hpp"

class World;

ENUM(AnimalId,
COW,
PIG,
HORSE)

class EntityFactory {
public:
	EntityFactory() = delete;
	static Entity createPlayer(vec position);
	static Entity createCamera(vec position, float zoom);
	static Entity createAnimal(AnimalId::value animalId, vec position);
	static Entity createResource(ResourceId::value resourceId, pair position);
	static Entity createStation(StationId::value stationId, pair position);
	
	static Entity createItem(ItemId::value itemId, uint8_t count);
	static Entity createItem(ItemId::value itemId, uint8_t count, vec position);

	static World* world;

private:
	static bool free(pair position, pair size);
	static uint seed;
};