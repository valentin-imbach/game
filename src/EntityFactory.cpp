
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

	world->ecs.addComponent<ActionComponent>({}, player);
	world->ecs.addComponent<DirectionComponent>({Direction::EAST}, player);
	world->ecs.addComponent<MovementComponent>({2, 3}, player);
	world->ecs.addComponent<ForceComponent>({{0, 0}}, player);
	world->ecs.addComponent<ControllerComponent>({}, player);

	world->ecs.addComponent<ColliderComponent>({Shape(vec(0.6f, 0.6f))}, player);
	world->ecs.addComponent<HitboxComponent>({Shape(vec(0.8f, 1.5f), vec(0, -0.6))}, player);

	world->ecs.addComponent<HealthComponent>({20, 20}, player);
	world->ecs.addComponent<ParticleComponent>({ParticleStyle::templates[ParticleId::DIRT]}, player);

	world->ecs.addComponent<EffectComponent>({}, player);

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

	for (int i = 1; i < CreatureLayer::count; i++) {
		creatureAnimationComponent.sprites[MovementState::IDLE].first.setSprite(i, Sprite(SpriteSheet::MODULAR_PLAYER, {3, 2*i-2}, {1, 2}), {0, -1});
		creatureAnimationComponent.sprites[MovementState::IDLE].second.setSprite(i, Sprite(SpriteSheet::MODULAR_PLAYER, {3, 16 + 2*i-2}, {1, 2}), {0, -1});
	}

	//creatureAnimationComponent.sprites[MovementState::IDLE].first.stack[CreatureLayer::HEAD].sprite.tint = {255, 100, 100};

	for (int i = 1; i < CreatureLayer::count; i++) {
		creatureAnimationComponent.sprites[MovementState::WALK].first.setSprite(i, Sprite(SpriteSheet::MODULAR_PLAYER, {0, 2*i-2}, {1, 2}, 8, 100), {0, -1});
		creatureAnimationComponent.sprites[MovementState::WALK].second.setSprite(i, Sprite(SpriteSheet::MODULAR_PLAYER, {0, 16 + 2*i-2}, {1, 2}, 8, 100), {0, -1});
	}

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

Entity EntityFactory::createStructure(StructureId::value structureId, Realm* realm, pair position) {
	if (!structureId) return 0;
	StructureTemplate& structureTemplate = StructureTemplate::templates[structureId];

	Entity structure = createStaticEntity(realm, position, structureTemplate.size, true, true);
	if (!structure) return 0;

	SpriteStack spriteStack;
	for (int i = 0; i < structureTemplate.spriteTemplates.size(); i++) {
		SpriteTemplate& sprite = structureTemplate.spriteTemplates[i];
		uint var = noise::Int(seed++, 0, sprite.variations);
		pair spritePosition(sprite.anker.x + var * sprite.size.x, sprite.anker.y);
		spriteStack.setSprite(i, Sprite(structureTemplate.spriteSheet, spritePosition, sprite.size), sprite.offset);
	}
	world->ecs.addComponent<SpriteComponent>({spriteStack}, structure);
	return structure;
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

Entity EntityFactory::createEnemy(EnemyId::value enemyId, Realm* realm, vec position) {
	Entity enemy = createDynamicEntity(realm, position);
	if (!enemy) return 0;
	
	world->ecs.addComponent<ActionComponent>({}, enemy);
	world->ecs.addComponent<DirectionComponent>({Direction::EAST}, enemy);
	world->ecs.addComponent<ForceComponent>({{0, 0}}, enemy);
	world->ecs.addComponent<MovementComponent>({0.5f, 1}, enemy);

	world->ecs.addComponent<ColliderComponent>({Shape(vec(0.6f, 0.6f))}, enemy);
	world->ecs.addComponent<HitboxComponent>({Shape(vec(0.8f, 1.5f), vec(0, -0.6))}, enemy);

	world->ecs.addComponent<HealthComponent>({20, 20}, enemy);
	world->ecs.addComponent<ParticleComponent>({ParticleStyle::templates[ParticleId::DIRT]}, enemy);

	world->ecs.addComponent<SensorComponent>({10, EntityTag::PLAYER}, enemy);
	world->ecs.addComponent<AiComponent>({}, enemy);
	world->ecs.addComponent<AiWanderComponent>({position, {1, 0}}, enemy);
	world->ecs.addComponent<AiChaseComponent>({}, enemy);
	world->ecs.addComponent<AiMeleeComponent>({5, 1000, 0}, enemy);

	world->ecs.addComponent<SpriteComponent>({}, enemy);
	CreatureAnimationComponent creatureAnimationComponent = {};

	for (int i = 1; i < CreatureLayer::count; i++) {
		creatureAnimationComponent.sprites[MovementState::IDLE].first.setSprite(i, Sprite(SpriteSheet::MODULAR_ZOMBIE, {3, 2*i-2}, {1, 2}), {0, -1});
		creatureAnimationComponent.sprites[MovementState::IDLE].second.setSprite(i, Sprite(SpriteSheet::MODULAR_ZOMBIE, {3, 16 + 2*i-2}, {1, 2}), {0, -1});
	}

	for (int i = 1; i < CreatureLayer::count; i++) {
		creatureAnimationComponent.sprites[MovementState::WALK].first.setSprite(i, Sprite(SpriteSheet::MODULAR_ZOMBIE, {0, 2*i-2}, {1, 2}, 8, 100), {0, -1});
		creatureAnimationComponent.sprites[MovementState::WALK].second.setSprite(i, Sprite(SpriteSheet::MODULAR_ZOMBIE, {0, 16 + 2*i-2}, {1, 2}, 8, 100), {0, -1});
	}

	for (int i = 1; i < CreatureLayer::count; i++) {
		creatureAnimationComponent.sprites[MovementState::RUN].first.setSprite(i, Sprite(SpriteSheet::MODULAR_ZOMBIE, {0, 2*i-2}, {1, 2}, 8, 100), {0, -1});
		creatureAnimationComponent.sprites[MovementState::RUN].second.setSprite(i, Sprite(SpriteSheet::MODULAR_ZOMBIE, {0, 16 + 2*i-2}, {1, 2}, 8, 100), {0, -1});
	}

	world->ecs.addComponent<CreatureAnimationComponent>(creatureAnimationComponent, enemy);

	return enemy;
}


Entity EntityFactory::createAnimal(AnimalId::value animalId, Realm* realm, vec position) {
	Entity animal = createDynamicEntity(realm, position);
	if (!animal) return 0;
	
	world->ecs.addComponent<ActionComponent>({}, animal);
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

	world->ecs.addComponent<MovementComponent>({animalTemplate.speed.x, animalTemplate.speed.y}, animal);
	world->ecs.addComponent<ColliderComponent>({animalTemplate.collider}, animal);	
	world->ecs.addComponent<HitboxComponent>({animalTemplate.hitbox}, animal);
	world->ecs.addComponent<LootComponent>({animalTemplate.lootTable}, animal);
	world->ecs.addComponent<HealthComponent>({animalTemplate.health, animalTemplate.health}, animal);
	// world->ecs.addComponent<CreatureAnimationComponent>({animalTemplate.sprites}, animal);

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

Entity EntityFactory::createPortal(Realm* realm, pair position, Realm* otherRealm, pair entry) {
	Entity portal = createStaticEntity(realm, position, {1, 1}, true, true);
	SpriteStack portalSprites;
	portalSprites.setSprite(0, Sprite(SpriteSheet::CAVE, pair(0, 0), pair(1, 1)));
	world->ecs.addComponent<SpriteComponent>({portalSprites}, portal);
	world->ecs.addComponent<PortalComponent>({otherRealm->realmId, entry}, portal);
	return portal;
}