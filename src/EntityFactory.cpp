
#include "EntityFactory.hpp"
#include "ECS.hpp"
#include "ECS_types.hpp"
#include "Item.hpp"
#include "Components.hpp"
#include "World.hpp"

World* EntityFactory::world = nullptr;
uint EntityFactory::seed = 1729;

Entity EntityFactory::createStaticEntity(Realm* realm, pair position, pair size, bool solid, bool opaque) {
	Entity entity = world->ecs.createEntity();
	if (!entity) return 0;

	pair chunk = vec::round(vec(position) / CHUNK_SIZE);
	world->ecs.addComponent<PositionComponent>({position, realm->realmId, chunk}, entity);
	realm->linkChunk(entity, chunk);

	world->ecs.addComponent<GridComponent>({position, realm->realmId, size, solid, opaque}, entity);
	realm->linkGrid(entity, position, size, solid, opaque);

	return entity;
}

Entity EntityFactory::createDynamicEntity(Realm* realm, vec position) {
	Entity entity = world->ecs.createEntity();
	if (!entity) return 0;

	pair chunk = vec::round(position / CHUNK_SIZE);
	world->ecs.addComponent<PositionComponent>({position, realm->realmId, chunk}, entity);
	realm->linkChunk(entity, chunk);
	world->ecs.addComponent<ChunkComponent>({}, entity);

	return entity;
}

Entity EntityFactory::createPlayer(Realm* realm, vec position) {
	Entity player = createDynamicEntity(realm, position);
	if (!player) return 0;

	world->ecs.addComponent<CreatureStateComponent>({CreatureState::IDLE, Direction::EAST}, player);
	world->ecs.addComponent<DirectionComponent>({Direction::EAST}, player);
	world->ecs.addComponent<MovementComponent>({2}, player);
	world->ecs.addComponent<ControllerComponent>({}, player);

	SpriteStack spriteStack;
	spriteStack.addSprite({SpriteSheet::PLAYER, {0, 0}, {1, 2}, 1, 100}, pair(0, -1));
	world->ecs.addComponent<SpriteComponent>({spriteStack}, player);

	Shape collider({0, 0}, {0.6f, 0.6f});
	world->ecs.addComponent<ColliderComponent>({collider}, player);

	Shape hitbox({0, -0.6}, {0.8f, 1.5f});
	world->ecs.addComponent<HitboxComponent>({hitbox}, player);

	world->ecs.addComponent<HealthComponent>({20, 20}, player);
	world->ecs.addComponent<ParticleComponent>({ParticleSystem::DIRT}, player);

	world->ecs.addComponent<InventoryComponent>({Inventory({7, 5})}, player);
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

	return player;
}

Entity EntityFactory::createResource(ResourceId::value resourceId, Realm* realm, pair position) {
	if (!resourceId || !ResourceTemplate::templates[resourceId]) return 0;
	ResourceTemplate* resourceTemplate = ResourceTemplate::templates[resourceId].get();

	Entity resource = createStaticEntity(realm, position, resourceTemplate->size, resourceTemplate->solid, resourceTemplate->opaque);
	if (!resource) return 0;

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
	// if (6 <= resourceId && resourceId <= 10) {
	// 	world->ecs.addComponent<MaturityComponent>({world->ticks, 10000, 5}, resource);
	// }

	return resource;
}

Entity EntityFactory::createMonster(CreatureId::value creatureId, Realm* realm, vec position) {
	Entity monster = createDynamicEntity(realm, position);
	if (!monster) return 0;
	
	world->ecs.addComponent<CreatureStateComponent>({CreatureState::IDLE, Direction::EAST}, monster);
	world->ecs.addComponent<DirectionComponent>({Direction::EAST}, monster);
	world->ecs.addComponent<ForceComponent>({{0, 0}}, monster);
	world->ecs.addComponent<MovementComponent>({0.5f}, monster);

	SpriteStack monsterSprites;
	monsterSprites.addSprite({SpriteSheet::MONSTER, {0, 0}, {1, 2}, 1, 100}, pair(0, -1));
	world->ecs.addComponent<SpriteComponent>({monsterSprites}, monster);

	Shape collider({0, 0}, {0.6f, 0.6f});
	world->ecs.addComponent<ColliderComponent>({collider}, monster);

	Shape hitbox({0, -0.6}, {0.8f, 1.5f});
	world->ecs.addComponent<HitboxComponent>({hitbox}, monster);

	world->ecs.addComponent<HealthComponent>({20, 20}, monster);
	world->ecs.addComponent<ParticleComponent>({ParticleSystem::DIRT}, monster);

	world->ecs.addComponent<SensorComponent>({10}, monster);
	world->ecs.addComponent<AiComponent>({}, monster);
	world->ecs.addComponent<AiWanderComponent>({position, {1, 0}}, monster);
	world->ecs.addComponent<AiChaseComponent>({}, monster);
	world->ecs.addComponent<AiMeleeComponent>({5, 1000, 0}, monster);

	return monster;
}


Entity EntityFactory::createAnimal(CreatureId::value creatureId, Realm* realm, vec position) {
	Entity animal = createDynamicEntity(realm, position);
	if (!animal) return 0;
	
	world->ecs.addComponent<CreatureStateComponent>({CreatureState::IDLE, Direction::EAST}, animal);
	world->ecs.addComponent<DirectionComponent>({Direction::EAST}, animal);
	world->ecs.addComponent<ForceComponent>({{0, 0}}, animal);
	world->ecs.addComponent<MovementComponent>({0.5f}, animal);

	SpriteStack spriteStack;
	spriteStack.addSprite({SpriteSheet::COW, {0, 0}, {1, 2}, 1, 100}, pair(0, -1));
	world->ecs.addComponent<SpriteComponent>({spriteStack}, animal);

	Shape collider({0, -0.2f}, {0.6f, 0.6f});
	world->ecs.addComponent<ColliderComponent>({collider}, animal);

	Shape hitbox({0, -0.3}, {0.8f, 0.8f});
	world->ecs.addComponent<HitboxComponent>({hitbox}, animal);

	//world->ecs.addComponent<LootComponent>({ItemId::APPLE, {1,3}, 1.0f}, animal);
	world->ecs.addComponent<HealthComponent>({10, 10}, animal);
	world->ecs.addComponent<ParticleComponent>({ParticleSystem::DIRT}, animal);

	world->ecs.addComponent<SensorComponent>({5}, animal);
	world->ecs.addComponent<AiComponent>({}, animal);
	world->ecs.addComponent<AiWanderComponent>({position, {1, 0}}, animal);
	world->ecs.addComponent<AiFleeComponent>({}, animal);

	return animal;
}

Entity EntityFactory::createItem(ItemId::value itemId, uchar count) {
	Entity item = world->ecs.createEntity();
	if (!item) return 0;

	Shape collider({0, 0}, {0.4f, 0.4f});
	world->ecs.addComponent<ColliderComponent>({collider}, item);

	SpriteStack spriteStack;
	spriteStack.addSprite({SpriteSheet::ITEMS, {(itemId - 1) % 6, (itemId - 1) / 6}, {1, 1}});
	SpriteComponent spriteComponent = {spriteStack, 0.5f};
	spriteComponent.effects[SpriteEffectId::BOUNCE] = {true, 0};
	world->ecs.addComponent<SpriteComponent>(spriteComponent, item);

	world->ecs.addComponent<ItemComponent>({itemId, count}, item);

	return item;
}

Entity EntityFactory::createItem(ItemId::value itemId, uchar count, Realm* realm, vec position) {
	Entity item = createItem(itemId, count);
	if (!item) return 0;

	pair chunk = vec::round(position / CHUNK_SIZE);
	world->ecs.addComponent<PositionComponent>({position, realm->realmId, chunk}, item);
	realm->linkChunk(item, chunk);

	return item;
}

Entity EntityFactory::createStation(StationId::value stationId, Realm* realm, pair position, bool link) {
	Entity station = world->ecs.createEntity();
	if (!stationId || !station) return 0;
	
	pair chunk = vec::round(position / CHUNK_SIZE);
	world->ecs.addComponent<PositionComponent>({position, realm->realmId, chunk}, station);
	realm->linkChunk(station, chunk);

	world->ecs.addComponent<GridComponent>({position, realm->realmId, {1,1}, true, false}, station);
	if (link) realm->linkGrid(station, position, {1,1}, true, false);

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
	Entity projectile = createDynamicEntity(realm, position);
	if (!projectile) return 0;

	SpriteStack spriteStack;
	spriteStack.addSprite({SpriteSheet::ITEMS, {5, 0}, {1, 1}});
	SpriteComponent spriteComponent = {spriteStack};
	spriteComponent.angle = 45 - vec::angle(direction) * 180/M_PI;
	world->ecs.addComponent<SpriteComponent>(spriteComponent, projectile);

	world->ecs.addComponent<ProjectileComponent>({direction * 70}, projectile);

	return projectile;
}