
#pragma once
#include "utils.hpp"
#include "ECS_types.hpp"
#include "Item_types.hpp"
#include "ResourceTemplates.hpp"
#include "AnimalTemplates.hpp"
#include "Components.hpp"
#include "StructureTemplates.hpp"

class World;
class Realm;

class EntityFactory {
public:
	EntityFactory() = delete;
	static Entity createPlayer(Realm* realm, vec position);
	static Entity createAnimal(AnimalId::value animalId, Realm* realm, vec position);
	static Entity createEnemy(EnemyId::value enemyId, Realm* realm, vec position);
	static Entity createResource(ResourceId::value resourceId, Realm* realm, pair position);
	static Entity createStructure(StructureId::value structurId, Realm* realm, pair position);
	static Entity createCrop(CropId::value cropId, Realm* realm, pair position);
	static Entity createStation(StationId::value stationId, Realm* realm, pair position, bool link = true);
	static Entity createPortal(Realm* realm, pair position, Realm* otherRealm, pair entry);
	
	static Entity createItem(ItemId::value itemId = ItemId::NONE, uchar count = 1);
	static Entity createItem(ItemId::value itemId, uchar count, Realm* realm, vec position);

	static Entity createTool(ItemKind::value itemKind);

	static Entity createProjectile(Realm* realm, vec position, vec direction);
	static Entity createDamageArea(Realm* realm, vec position, Shape shape, uint start, uint duration, vec force = {0,0}, Entity imune = 0);

	static World* world;

private:
	static Entity createStaticEntity(Realm* realm, pair position, pair size, bool solid, bool opaque);
	static Entity createDynamicEntity(Realm* realm, vec position);
	static uint seed;
};