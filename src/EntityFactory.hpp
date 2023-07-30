
#pragma once
#include "utils.hpp"
#include "ECS_types.hpp"
#include "Item_types.hpp"
#include "ResourceTemplates.hpp"
#include "Components.hpp"

class World;
class Realm;

ENUM(AnimalId,
COW,
PIG,
HORSE,
MONSTER)

class EntityFactory {
public:
	EntityFactory() = delete;
	static Entity createPlayer(Realm* realm, vec position);
	static Entity createAnimal(AnimalId::value animalId, Realm* realm, vec position);
	static Entity createResource(ResourceId::value resourceId, Realm* realm, pair position);
	static Entity createStation(StationId::value stationId, Realm* realm, pair position);
	static Entity createStation(StationId::value stationId, Realm* realm, pair position, bool link = true);
	
	static Entity createItem(ItemId::value itemId, uchar count);
	static Entity createItem(ItemId::value itemId, uchar count, Realm* realm, vec position);

	static Entity createProjectile(Realm* realm, vec position, vec direction);

	static World* world;

private:
	static uint seed;
};