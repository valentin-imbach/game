
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
	static Entity createPlayer(RealmId realmId, vec position);
	static Entity createAnimal(AnimalId::value animalId, RealmId realmId, vec position);
	static Entity createEnemy(EnemyId::value enemyId, RealmId realmId, vec position);
	static Entity createResource(ResourceId::value resourceId, RealmId realmId, pair position);
	static Entity createStructure(StructureId::value structurId, RealmId realmId, pair position);
	static Entity createCrop(CropId::value cropId, RealmId realmId, pair position);
	static Entity createStation(StationId::value stationId, RealmId realmId, pair position, bool link = true);
	static Entity createPortal(RealmId realmId, pair position, RealmId otherRealmId, pair entry);
	
	static Entity createItem(ItemId::value itemId = ItemId::NONE, uchar count = 1);
	static Entity createItem(ItemId::value itemId, uchar count, RealmId realmId, vec position);

	static Entity createTool(ItemKind::value itemKind);

	static Entity createProjectile(RealmId realmId, vec position, vec direction);
	static Entity createExplosive(RealmId realmId, vec position);
	static Entity createDamageArea(RealmId realmId, vec position, Shape shape, uint start, uint duration, vec force = {0,0}, Entity imune = 0);

	static World* world;

private:
	static Entity createStaticEntity(RealmId realmId, pair position, pair size, bool solid, bool opaque);
	static Entity createDynamicEntity(RealmId realmId, vec position);
	static uint seed;
};