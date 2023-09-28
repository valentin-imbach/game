
#pragma once
#include "utils.hpp"
#include "ECS_types.hpp"
#include "Item_types.hpp"
#include "ResourceTemplates.hpp"
#include "Components.hpp"

class World;
class Realm;

ENUM(CreatureId,
COW,
PIG,
HORSE,
MONSTER)

ENUM(CropId,
PARSNIP)

class EntityFactory {
public:
	EntityFactory() = delete;
	static Entity createPlayer(Realm* realm, vec position);
	static Entity createAnimal(CreatureId::value creatureId, Realm* realm, vec position);
	static Entity createMonster(CreatureId::value creatureId, Realm* realm, vec position);
	static Entity createResource(ResourceId::value resourceId, Realm* realm, pair position);
	static Entity createCrop(CropId::value cropId, Realm* realm, pair position);
	static Entity createStation(StationId::value stationId, Realm* realm, pair position, bool link = true);
	
	static Entity createItem(ItemId::value itemId = ItemId::NONE, uchar count = 1);
	static Entity createItem(ItemId::value itemId, uchar count, Realm* realm, vec position);

	static Entity createProjectile(Realm* realm, vec position, vec direction);
	static Entity createDamageArea(Realm* realm, vec position, Shape shape, uint start, uint duration, vec force = {0,0}, Entity imune = 0);

	static World* world;

private:
	static Entity createStaticEntity(Realm* realm, pair position, pair size, bool solid, bool opaque);
	static Entity createDynamicEntity(Realm* realm, vec position);
	static uint seed;
};