
#include "EntityFactory.hpp"
#include "ECS.hpp"
#include "ECS_types.hpp"
#include "Item.hpp"
#include "Components.hpp"
#include "World.hpp"

World* EntityFactory::world = nullptr;
uint EntityFactory::seed = 1729;

Entity EntityFactory::createPlayer(Realm* realm, vec position) {
	Entity player = world->ecs.createEntity();
	world->ecs.addComponent<PositionComponent>({position, realm->realmId, vec::round(position / CHUNK_SIZE)}, player);
	world->ecs.addComponent<CreatureStateComponent>({CreatureState::IDLE, Direction::EAST}, player);
	world->ecs.addComponent<DirectionComponent>({Direction::EAST}, player);
	world->ecs.addComponent<MovementComponent>({2}, player);
	world->ecs.addComponent<ControllerComponent>({}, player);
	SpriteStack spriteStack;
	spriteStack.addSprite({SpriteSheet::PLAYER, {0, 0}, {1, 2}, 1, 100}, pair(0, -1));
	world->ecs.addComponent<SpriteComponent>({spriteStack}, player);
	Collider collider({0, 0}, {0.6f, 0.6f});
	world->ecs.addComponent<ColliderComponent>({collider}, player);
	world->ecs.addComponent<InventoryComponent>({Inventory({7, 5})}, player);
	world->ecs.addComponent<HealthComponent>({20, 20}, player);
	Inventory equipment({3,4});
	equipment.itemContainers[0][0].itemKind = ItemKind::CLOTHING_HEAD;
	equipment.itemContainers[0][1].itemKind = ItemKind::CLOTHING_BODY;
	equipment.itemContainers[0][2].itemKind = ItemKind::CLOTHING_LEGS;
	equipment.itemContainers[0][3].itemKind = ItemKind::CLOTHING_FEET;
	equipment.itemContainers[1][0].itemKind = ItemKind::ARMOR_HEAD;
	equipment.itemContainers[1][1].itemKind = ItemKind::ARMOR_BODY;
	equipment.itemContainers[1][2].itemKind = ItemKind::ARMOR_LEGS;
	equipment.itemContainers[1][3].itemKind = ItemKind::ARMOR_FEET;
	for (int y = 0; y < 4; y++) equipment.itemContainers[2][y].itemKind = ItemKind::ACCESSORY;
	world->ecs.addComponent<PlayerComponent>({Inventory({7, 1}), equipment, 0}, player);
	world->ecs.addComponent<ParticleComponent>({ParticleSystem::DIRT}, player);
	world->ecs.addComponent<ChunkComponent>({}, player);

	return player;
}

bool EntityFactory::free(Realm* realm, pair position, pair size) {
	for (int x = 0; x < size.x; x++) {
		for (int y = 0; y < size.y; y++) {
			pair offset(x,y);
			if (realm->gridMap.find(position + offset) != realm->gridMap.end()) return false;
		}
	}
	return true;
}

Entity EntityFactory::createResource(ResourceId::value resourceId, Realm* realm, pair position) {
	if (!resourceId || !ResourceTemplate::templates[resourceId]) return 0;
	ResourceTemplate* resourceTemplate = ResourceTemplate::templates[resourceId].get();
	//if (!free(position, resourceTemplate->size)) return 0;

	Entity resource = world->ecs.createEntity();
	if (!resource) return 0;

	world->ecs.addComponent<PositionComponent>({position, realm->realmId, vec::round(position / CHUNK_SIZE)}, resource);
	world->ecs.addComponent<GridComponent>({position, realm->realmId, resourceTemplate->size, resourceTemplate->solid, resourceTemplate->opaque}, resource);

	SpriteStack spriteStack;
	for (SpriteTemplate& sprite : resourceTemplate->spriteTemplates) {
		uint var = noise::Int(seed++, 0, sprite.variations);
		pair spritePosition(sprite.anker.x + var * sprite.size.x, sprite.anker.y);
		spriteStack.addSprite({SpriteSheet::RESOURCES, spritePosition, sprite.size}, sprite.offset);
	}

	world->ecs.addComponent<SpriteComponent>({spriteStack}, resource);
	world->ecs.addComponent<ResourceComponent>({resourceTemplate->toolId, resourceTemplate->sound, resourceTemplate->level}, resource);
	world->ecs.addComponent<LootComponent>({resourceTemplate->lootTable}, resource);
	world->ecs.addComponent<HealthComponent>({resourceTemplate->health, resourceTemplate->health}, resource);
	return resource;
}

Entity EntityFactory::createAnimal(AnimalId::value animalId, Realm* realm, vec position) {
	Entity animal = world->ecs.createEntity();
	world->ecs.addComponent<PositionComponent>({position, realm->realmId, vec::round(position / CHUNK_SIZE)}, animal);
	world->ecs.addComponent<CreatureStateComponent>({CreatureState::IDLE, Direction::EAST}, animal);
	world->ecs.addComponent<DirectionComponent>({Direction::EAST}, animal);
	world->ecs.addComponent<ForceComponent>({{0, 0}}, animal);
	world->ecs.addComponent<ChunkComponent>({}, animal);

	if (animalId == AnimalId::MONSTER) {
		world->ecs.addComponent<MovementComponent>({1}, animal);
		SpriteStack monsterSprites;
		monsterSprites.addSprite({SpriteSheet::MONSTER, {0, 0}, {1, 2}, 1, 100}, pair(0, -1));
		world->ecs.addComponent<SpriteComponent>({monsterSprites}, animal);
		Collider collider({0, 0}, {0.6f, 0.6f});
		world->ecs.addComponent<ColliderComponent>({collider}, animal);
		world->ecs.addComponent<HealthComponent>({20, 20}, animal);
		world->ecs.addComponent<MonsterAiComponent>({}, animal);
		world->ecs.addComponent<ParticleComponent>({ParticleSystem::DIRT}, animal);
		world->ecs.addComponent<SensorComponent>({10}, animal);
		return animal;
	}

	world->ecs.addComponent<MovementComponent>({0.5f}, animal);
	SpriteStack spriteStack;
	spriteStack.addSprite({SpriteSheet::COW, {0, 0}, {1, 2}, 1, 100}, pair(0, -1));
	world->ecs.addComponent<SpriteComponent>({spriteStack}, animal);
	Collider collider({0, 0}, {0.6f, 0.6f});
	world->ecs.addComponent<ColliderComponent>({collider}, animal);
	//world->ecs.addComponent<AnimalAiComponent>({0}, animal);
	//world->ecs.addComponent<LootComponent>({ItemId::APPLE, {1,3}, 1.0f}, animal);
	world->ecs.addComponent<HealthComponent>({10, 10}, animal);
	world->ecs.addComponent<ParticleComponent>({ParticleSystem::DIRT}, animal);

	world->ecs.addComponent<AiComponent>({}, animal);
	world->ecs.addComponent<AiWanderComponent>({position, {1, 0}}, animal);
	world->ecs.addComponent<AiMoveComponent>({}, animal);
	world->ecs.addComponent<AiFleeComponent>({}, animal);
	world->ecs.addComponent<SensorComponent>({5}, animal);

	return animal;
}

Entity EntityFactory::createItem(ItemId::value itemId, uchar count) {
	Entity item = world->ecs.createEntity();
	Collider collider({0, 0}, {0.4f, 0.4f});
	world->ecs.addComponent<ColliderComponent>({collider}, item);
	SpriteStack spriteStack;
	spriteStack.addSprite({SpriteSheet::ITEMS, {(itemId - 1) % 6, (itemId - 1) / 6}, {1, 1}});
	world->ecs.addComponent<SpriteComponent>({spriteStack, 0.5f}, item);
	world->ecs.getComponent<SpriteComponent>(item).effects[SpriteEffectId::BOUNCE] = {true, 0};
	ItemComponent itemComponent = {itemId, count};
	// if (itemId && ItemTemplate::templates[itemId]) {
	// 	ItemTemplate* temp = ItemTemplate::templates[itemId].get();
	// }
	world->ecs.addComponent<ItemComponent>(itemComponent, item);
	return item;
}

Entity EntityFactory::createItem(ItemId::value itemId, uchar count, Realm* realm, vec position) {
	Entity item = createItem(itemId, count);
	world->ecs.addComponent<PositionComponent>({position, realm->realmId}, item);
	return item;
}

Entity EntityFactory::createStation(StationId::value stationId, Realm* realm, pair position) {
	if (!stationId) return 0;
	Entity station = world->ecs.createEntity();
	if (!station) return 0;
	
	world->ecs.addComponent<PositionComponent>({position, realm->realmId}, station);
	world->ecs.addComponent<GridComponent>({position, realm->realmId, {1,1}, true, false}, station);
	// } else {
	// 	world->ecs.addComponent<PositionComponent>({position, 0}, station);
	// 	world->ecs.addComponent<ChunkComponent>({}, station);
	// }
	world->ecs.addComponent<StationComponent>({stationId}, station);

	if (stationId == StationId::CAMP_FIRE) {
		SpriteStack fireSprites;
		fireSprites.addSprite({SpriteSheet::FIRE, pair(0, 0), pair(1, 1), 4, 200});
		world->ecs.addComponent<SpriteComponent>({fireSprites}, station);
		world->ecs.addComponent<ParticleComponent>({ParticleSystem::SMOKE}, station);
		world->ecs.addComponent<LightComponent>({true, 3, {255, 0, 0, 255}, 3, 0.2f}, station);
		return station;
	}

	SpriteStack spriteStack;
	spriteStack.addSprite({SpriteSheet::STATIONS, {int(stationId) - 1, 0}, {1,2}}, {0, -1});
	world->ecs.addComponent<SpriteComponent>({spriteStack}, station);

	if (stationId == StationId::CHEST) {
		 world->ecs.addComponent<InventoryComponent>({Inventory({7, 5})}, station);
	}
	return station;
}

Entity EntityFactory::createProjectile(Realm* realm, vec position, vec direction) {
	Entity projectile = world->ecs.createEntity();
	if (!projectile) return 0;

	world->ecs.addComponent<PositionComponent>({position, realm->realmId}, projectile);

	SpriteStack spriteStack;
	spriteStack.addSprite({SpriteSheet::ITEMS, {5, 0}, {1, 1}});
	SpriteComponent spriteComponent = {spriteStack};
	
	spriteComponent.angle = 45 - vec::angle(direction) * 180/M_PI;
	world->ecs.addComponent<SpriteComponent>(spriteComponent, projectile);
	world->ecs.addComponent<ProjectileComponent>({direction * 70}, projectile);
	world->ecs.addComponent<ChunkComponent>({}, projectile);

	return projectile;
}