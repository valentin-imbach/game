
#include "EntityFactory.hpp"
#include "ECS.hpp"
#include "ECS_types.hpp"
#include "Item.hpp"
#include "Components.hpp"
#include "World.hpp"
#include "AnimalTemplates.hpp"

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
	world->ecs.addComponent<ForceComponent>({{0, 0}}, player);
	world->ecs.addComponent<ControllerComponent>({}, player);

	world->ecs.addComponent<ColliderComponent>({Shape(vec(0.6f, 0.6f))}, player);
	world->ecs.addComponent<HitboxComponent>({Shape(vec(0.8f, 1.5f), vec(0, -0.6))}, player);

	world->ecs.addComponent<HealthComponent>({20, 20}, player);
	world->ecs.addComponent<ParticleComponent>({ParticleStyle::templates[ParticleId::DIRT]}, player);

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

	TagComponent tagComponent = {};
	tagComponent.tags.set(EntityTag::PLAYER);
	world->ecs.addComponent<TagComponent>(tagComponent, player);

	world->ecs.addComponent<SpriteComponent>({}, player);
	CreatureAnimationComponent creatureAnimationComponent = {};
	creatureAnimationComponent.sprites[CreatureState::IDLE].first = Sprite(SpriteSheet::PLAYER, {3, 0}, {1, 2});
	creatureAnimationComponent.sprites[CreatureState::IDLE].second = Sprite(SpriteSheet::PLAYER, {3, 2}, {1, 2});
	creatureAnimationComponent.sprites[CreatureState::WALKING].first = Sprite(SpriteSheet::PLAYER, {0, 0}, {1, 2}, 8, 100);
	creatureAnimationComponent.sprites[CreatureState::WALKING].second = Sprite(SpriteSheet::PLAYER, {0, 2}, {1, 2}, 8, 100);
	world->ecs.addComponent<CreatureAnimationComponent>(creatureAnimationComponent, player);

	return player;
}

Entity EntityFactory::createResource(ResourceId::value resourceId, Realm* realm, pair position) {
	if (!resourceId) return 0;
	ResourceTemplate& resourceTemplate = ResourceTemplate::templates[resourceId];

	Entity resource = createStaticEntity(realm, position, resourceTemplate.size, resourceTemplate.solid, resourceTemplate.opaque);
	if (!resource) return 0;

	SpriteStack spriteStack;
	for (int i = 0; i < resourceTemplate.spriteTemplates.size(); i++) {
		SpriteTemplate& sprite = resourceTemplate.spriteTemplates[i];
		uint var = noise::Int(seed++, 0, sprite.variations);
		pair spritePosition(sprite.anker.x + var * sprite.size.x, sprite.anker.y);
		spriteStack.setSprite(i, Sprite(SpriteSheet::RESOURCES, spritePosition, sprite.size), sprite.offset);
	}
	world->ecs.addComponent<SpriteComponent>({spriteStack}, resource);

	world->ecs.addComponent<ResourceComponent>({resourceTemplate.toolId, resourceTemplate.sound, resourceTemplate.level}, resource);
	world->ecs.addComponent<LootComponent>({resourceTemplate.lootTable}, resource);
	world->ecs.addComponent<HealthComponent>({resourceTemplate.health, resourceTemplate.health}, resource);
	// if (6 <= resourceId && resourceId <= 10) {
	// 	world->ecs.addComponent<MaturityComponent>({world->ticks, 10000, 5}, resource);
	// }

	return resource;
}

Entity EntityFactory::createCrop(CropId::value cropId, Realm *realm, pair position) {
	if (!cropId) return 0;
	Entity crop = createStaticEntity(realm, position, pair(1, 1), false, false);
	if (!crop) return 0;

	SpriteStack spriteStack;
	spriteStack.setSprite(0, Sprite(SpriteSheet::CROPS, pair(0, 2 * (cropId - 1)), pair(1, 2)), pair(0, -1));
	world->ecs.addComponent<SpriteComponent>({spriteStack}, crop);
	world->ecs.addComponent<MaturityComponent>({world->ticks, 5000, 5}, crop);
	return crop;
}

Entity EntityFactory::createMonster(AnimalId::value animalId, Realm* realm, vec position) {
	Entity monster = createDynamicEntity(realm, position);
	if (!monster) return 0;
	
	world->ecs.addComponent<CreatureStateComponent>({CreatureState::IDLE, Direction::EAST}, monster);
	world->ecs.addComponent<DirectionComponent>({Direction::EAST}, monster);
	world->ecs.addComponent<ForceComponent>({{0, 0}}, monster);
	world->ecs.addComponent<MovementComponent>({0.5f}, monster);

	world->ecs.addComponent<ColliderComponent>({Shape(vec(0.6f, 0.6f))}, monster);
	world->ecs.addComponent<HitboxComponent>({Shape(vec(0.8f, 1.5f), vec(0, -0.6))}, monster);

	world->ecs.addComponent<HealthComponent>({20, 20}, monster);
	world->ecs.addComponent<ParticleComponent>({ParticleStyle::templates[ParticleId::DIRT]}, monster);

	world->ecs.addComponent<SensorComponent>({10, EntityTag::ANIMAL}, monster);
	world->ecs.addComponent<AiComponent>({}, monster);
	world->ecs.addComponent<AiWanderComponent>({position, {1, 0}}, monster);
	world->ecs.addComponent<AiChaseComponent>({}, monster);
	world->ecs.addComponent<AiMeleeComponent>({5, 1000, 0}, monster);

	world->ecs.addComponent<SpriteComponent>({}, monster);
	CreatureAnimationComponent creatureAnimationComponent = {};
	creatureAnimationComponent.sprites[CreatureState::IDLE].first = Sprite(SpriteSheet::MONSTER, {3, 0}, {1, 2});
	creatureAnimationComponent.sprites[CreatureState::IDLE].second = Sprite(SpriteSheet::MONSTER, {3, 2}, {1, 2});
	creatureAnimationComponent.sprites[CreatureState::WALKING].first = Sprite(SpriteSheet::MONSTER, {0, 0}, {1, 2}, 8, 100);
	creatureAnimationComponent.sprites[CreatureState::WALKING].second = Sprite(SpriteSheet::MONSTER, {0, 2}, {1, 2}, 8, 100);
	creatureAnimationComponent.sprites[CreatureState::RUNNING].first = Sprite(SpriteSheet::MONSTER, {0, 0}, {1, 2}, 8, 100);
	creatureAnimationComponent.sprites[CreatureState::RUNNING].second = Sprite(SpriteSheet::MONSTER, {0, 2}, {1, 2}, 8, 100);
	world->ecs.addComponent<CreatureAnimationComponent>(creatureAnimationComponent, monster);

	return monster;
}


Entity EntityFactory::createAnimal(AnimalId::value animalId, Realm* realm, vec position) {
	Entity animal = createDynamicEntity(realm, position);
	if (!animal) return 0;
	
	world->ecs.addComponent<CreatureStateComponent>({CreatureState::IDLE, Direction::EAST}, animal);
	world->ecs.addComponent<DirectionComponent>({Direction::EAST}, animal);
	world->ecs.addComponent<ForceComponent>({{0, 0}}, animal);
	world->ecs.addComponent<ParticleComponent>({ParticleStyle::templates[ParticleId::DIRT]}, animal);
	world->ecs.addComponent<SensorComponent>({5, EntityTag::PLAYER}, animal);
	world->ecs.addComponent<AiComponent>({}, animal);
	world->ecs.addComponent<AiWanderComponent>({position, {1, 0}}, animal);
	world->ecs.addComponent<AiFleeComponent>({}, animal);
	world->ecs.addComponent<SpriteComponent>({}, animal);

	TagComponent tagComponent = {};
	tagComponent.tags.set(EntityTag::ANIMAL);
	world->ecs.addComponent<TagComponent>(tagComponent, animal);

	AnimalTemplate& animalTemplate = AnimalTemplate::templates[animalId];

	world->ecs.addComponent<MovementComponent>({animalTemplate.speed.x}, animal);
	world->ecs.addComponent<ColliderComponent>({animalTemplate.collider}, animal);	
	world->ecs.addComponent<HitboxComponent>({animalTemplate.hitbox}, animal);
	world->ecs.addComponent<LootComponent>({animalTemplate.lootTable}, animal);
	world->ecs.addComponent<HealthComponent>({animalTemplate.health, animalTemplate.health}, animal);
	world->ecs.addComponent<CreatureAnimationComponent>({animalTemplate.sprites}, animal);

	return animal;
}

Entity EntityFactory::createItem(ItemId::value itemId, uchar count) {
	Entity item = world->ecs.createEntity();
	if (!item) return 0;

	if (itemId) {
		SpriteStack spriteStack;
		spriteStack.setSprite(0, Sprite(SpriteSheet::ITEMS, pair((itemId - 1) % 5, (itemId - 1) / 5)));
		SpriteComponent spriteComponent = {spriteStack, 0.5f};
		spriteComponent.effects[SpriteEffectId::BOUNCE] = {true, 0};
		world->ecs.addComponent<SpriteComponent>(spriteComponent, item);
	}

	world->ecs.addComponent<ColliderComponent>({Shape(vec(0.4f, 0.4f))}, item);
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
		fireSprites.setSprite(0, Sprite(SpriteSheet::FIRE, pair(0, 0), pair(1, 1), 4, 200));
		world->ecs.addComponent<SpriteComponent>({fireSprites}, station);
		world->ecs.addComponent<ParticleComponent>({ParticleStyle::templates[ParticleId::SMOKE]}, station);
		world->ecs.addComponent<LightComponent>({true, 3, {255, 0, 0, 255}, 3, 0.2f}, station);
		return station;
	}

	SpriteStack spriteStack;
	spriteStack.setSprite(0, Sprite(SpriteSheet::STATIONS, pair(int(stationId) - 1, 0), pair(1, 2)), pair(0, -1));
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
	spriteStack.setSprite(0, Sprite(SpriteSheet::ITEMS, {5, 0}, {1, 1}));
	SpriteComponent spriteComponent = {spriteStack};
	spriteComponent.angle = 45 - vec::angle(direction) * 180/M_PI;
	world->ecs.addComponent<SpriteComponent>(spriteComponent, projectile);

	world->ecs.addComponent<ProjectileComponent>({direction * 70}, projectile);

	return projectile;
}

Entity EntityFactory::createDamageArea(Realm* realm, vec position, Shape shape, uint start, uint duration, vec force, Entity imune) {
	Entity damageArea = createDynamicEntity(realm, position);
	if (!damageArea) return 0;

	world->ecs.addComponent<HitboxComponent>({shape}, damageArea);
	world->ecs.addComponent<DamageComponent>({1, start, duration, force, imune}, damageArea);
	return damageArea;
}
