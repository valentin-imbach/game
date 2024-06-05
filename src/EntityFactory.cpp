
#include "EntityFactory.hpp"
#include "ECS.hpp"
#include "ECS_types.hpp"
#include "Item.hpp"
#include "Components.hpp"
#include "World.hpp"

#include "ResourceTemplates.hpp"
#include "StructureTemplates.hpp"
#include "AnimalTemplates.hpp"

World* EntityFactory::world = nullptr;
uint EntityFactory::seed = 1729;

Entity EntityFactory::createStaticEntity(RealmId realmId, pair position, pair size, bool solid, bool opaque) {
	Entity entity = world->ecs.createEntity();
	if (!entity) return 0;

	pair chunk = vec::round(vec(position) / CHUNK_SIZE);
	world->ecs.addComponent<PositionComponent>({position, realmId, chunk}, entity);
	world->realmManager.getRealm(realmId)->linkChunk(entity, chunk);

	world->ecs.addComponent<GridComponent>({position, realmId, size, solid, opaque}, entity);
	world->realmManager.getRealm(realmId)->linkGrid(entity, position, size, solid, opaque);

	return entity;
}

Entity EntityFactory::createDynamicEntity(RealmId realmId, vec position) {
	Entity entity = world->ecs.createEntity();
	if (!entity) return 0;

	pair chunk = vec::round(position / CHUNK_SIZE);
	world->ecs.addComponent<PositionComponent>({position, realmId, chunk}, entity);
	world->realmManager.getRealm(realmId)->linkChunk(entity, chunk);
	world->ecs.addComponent<ChunkComponent>({}, entity);

	return entity;
}

Entity EntityFactory::createPlayer(RealmId realmId, vec position) {
	Entity player = createDynamicEntity(realmId, position);
	if (!player) return 0;

	world->ecs.addComponent<ActionComponent>({}, player);
	world->ecs.addComponent<DirectionComponent>({Direction::EAST}, player);
	world->ecs.addComponent<MovementComponent>({2, 3}, player);
	world->ecs.addComponent<ForceComponent>({vec(0, 0)}, player);
	world->ecs.addComponent<ControllerComponent>({}, player);
	world->ecs.addComponent<HandComponent>({}, player);

	world->ecs.addComponent<ColliderComponent>({Shape(vec(0.6f, 0.6f))}, player);
	world->ecs.addComponent<HitboxComponent>({Shape(vec(0.8f, 1.5f), vec(0, -0.6))}, player);

	world->ecs.addComponent<HealthComponent>({20, 20}, player);
	world->ecs.addComponent<ParticleComponent>({}, player);

	world->ecs.addComponent<EffectComponent>({}, player);

	world->ecs.addComponent<InventoryComponent>({Inventory(pair(7, 5))}, player);

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
	
	world->ecs.addComponent<PlayerComponent>({Inventory(pair(7, 1)), equipment, 0}, player);

	TagComponent tagComponent = {};
	tagComponent.tags.set(EntityTag::PLAYER);
	world->ecs.addComponent<TagComponent>(tagComponent, player);

	world->ecs.addComponent<SpriteComponent>({}, player);
	CreatureAnimationComponent creatureAnimationComponent = {};

	for (int i = 1; i < CreatureLayer::count; i++) {
		creatureAnimationComponent.sprites[MovementState::IDLE].first.setSprite(i, Sprite(SpriteSheet::MODULAR_PLAYER, pair(3, 2*i-2), pair(1, 2)), pair(0, -1));
		creatureAnimationComponent.sprites[MovementState::IDLE].second.setSprite(i, Sprite(SpriteSheet::MODULAR_PLAYER, pair(3, 16 + 2*i-2), pair(1, 2)), pair(0, -1));
	}

	for (int i = 1; i < CreatureLayer::count; i++) {
		creatureAnimationComponent.sprites[MovementState::WALK].first.setSprite(i, Sprite(SpriteSheet::MODULAR_PLAYER, pair(0, 2*i-2), pair(1, 2), 8, 100), pair(0, -1));
		creatureAnimationComponent.sprites[MovementState::WALK].second.setSprite(i, Sprite(SpriteSheet::MODULAR_PLAYER, pair(0, 16 + 2*i-2), pair(1, 2), 8, 100), pair(0, -1));
	}

	world->ecs.addComponent<CreatureAnimationComponent>(creatureAnimationComponent, player);

	return player;
}

Entity EntityFactory::createResource(ResourceId::value resourceId, RealmId realmId, pair position) {
	if (!resourceId) return 0;
	ResourceTemplate& resourceTemplate = ResourceTemplate::templates[resourceId];

	Entity resource = createStaticEntity(realmId, position, resourceTemplate.size, resourceTemplate.solid, resourceTemplate.opaque);
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

Entity EntityFactory::createStructure(StructureId::value structureId, RealmId realmId, pair position) {
	if (!structureId) return 0;
	StructureTemplate& structureTemplate = StructureTemplate::templates[structureId];

	Entity structure = createStaticEntity(realmId, position, structureTemplate.size, true, true);
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

Entity EntityFactory::createCrop(CropId::value cropId, RealmId realmId, pair position) {
	if (!cropId) return 0;
	Entity crop = createStaticEntity(realmId, position, pair(1, 1), false, false);
	if (!crop) return 0;

	SpriteStack spriteStack;
	spriteStack.setSprite(0, Sprite(SpriteSheet::CROPS, pair(0, 2 * (cropId - 1)), pair(1, 2)), pair(0, -1));
	world->ecs.addComponent<SpriteComponent>({spriteStack}, crop);
	world->ecs.addComponent<MaturityComponent>({world->ticks, 5000, 5}, crop);
	return crop;
}

Entity EntityFactory::createEnemy(EnemyId::value enemyId, RealmId realmId, vec position) {
	Entity enemy = createDynamicEntity(realmId, position);
	if (!enemy) return 0;
	
	world->ecs.addComponent<ActionComponent>({}, enemy);
	world->ecs.addComponent<DirectionComponent>({Direction::EAST}, enemy);
	world->ecs.addComponent<ForceComponent>({{0, 0}}, enemy);
	world->ecs.addComponent<MovementComponent>({0.5f, 1}, enemy);

	world->ecs.addComponent<ColliderComponent>({Shape(vec(0.6f, 0.6f))}, enemy);
	world->ecs.addComponent<HitboxComponent>({Shape(vec(0.8f, 1.5f), vec(0, -0.6))}, enemy);

	world->ecs.addComponent<HealthComponent>({20, 20}, enemy);
	world->ecs.addComponent<ParticleComponent>({}, enemy);

	world->ecs.addComponent<SensorComponent>({10, EntityTag::PLAYER}, enemy);
	world->ecs.addComponent<AiComponent>({}, enemy);
	world->ecs.addComponent<AiWanderComponent>({position, {1, 0}}, enemy);
	world->ecs.addComponent<AiChaseComponent>({}, enemy);
	// world->ecs.addComponent<AiFleeComponent>({}, enemy);

	Entity weapon = createTool(ItemKind::SWORD);
	world->ecs.addComponent<AiMeleeComponent>({5, 1000, 0, weapon}, enemy);
	world->ecs.addComponent<HandComponent>({weapon}, enemy);

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


Entity EntityFactory::createAnimal(AnimalId::value animalId, RealmId realmId, vec position) {
	Entity animal = createDynamicEntity(realmId, position);
	if (!animal) return 0;
	
	world->ecs.addComponent<ActionComponent>({}, animal);
	world->ecs.addComponent<DirectionComponent>({Direction::EAST}, animal);
	world->ecs.addComponent<ForceComponent>({{0, 0}}, animal);
	world->ecs.addComponent<ParticleComponent>({}, animal);
	world->ecs.addComponent<SensorComponent>({5, EntityTag::PLAYER}, animal);
	world->ecs.addComponent<AiComponent>({}, animal);
	world->ecs.addComponent<AiWanderComponent>({position, {1, 0}}, animal);
	world->ecs.addComponent<AiFleeComponent>({}, animal);

	TagComponent tagComponent = {};
	tagComponent.tags.set(EntityTag::ANIMAL);
	world->ecs.addComponent<TagComponent>(tagComponent, animal);

	AnimalTemplate& animalTemplate = AnimalTemplate::templates[animalId];

	world->ecs.addComponent<MovementComponent>({animalTemplate.speed.x, animalTemplate.speed.y}, animal);
	world->ecs.addComponent<ColliderComponent>({animalTemplate.collider}, animal);	
	world->ecs.addComponent<HitboxComponent>({animalTemplate.hitbox}, animal);
	world->ecs.addComponent<LootComponent>({animalTemplate.lootTable}, animal);
	world->ecs.addComponent<HealthComponent>({animalTemplate.health, animalTemplate.health}, animal);
	
	world->ecs.addComponent<SpriteComponent>({}, animal);
	CreatureAnimationComponent creatureAnimationComponent = {};
	
	creatureAnimationComponent.sprites[MovementState::IDLE].first.setSprite(CreatureLayer::BODY, Sprite(SpriteSheet::COW, {3, 0}, {1, 2}), {0, -1});
	creatureAnimationComponent.sprites[MovementState::IDLE].first.setSprite(CreatureLayer::HEAD, Sprite(SpriteSheet::COW, {3, 2}, {1, 2}), {0, -1});
	creatureAnimationComponent.sprites[MovementState::IDLE].second.setSprite(CreatureLayer::BODY, Sprite(SpriteSheet::COW, {3, 4}, {1, 2}), {0, -1});
	creatureAnimationComponent.sprites[MovementState::IDLE].second.setSprite(CreatureLayer::HEAD, Sprite(SpriteSheet::COW, {3, 6}, {1, 2}), {0, -1});

	creatureAnimationComponent.sprites[MovementState::WALK].first.setSprite(CreatureLayer::BODY, Sprite(SpriteSheet::COW, {0, 0}, {1, 2}, 8, 150), {0, -1});
	creatureAnimationComponent.sprites[MovementState::WALK].first.setSprite(CreatureLayer::HEAD, Sprite(SpriteSheet::COW, {0, 2}, {1, 2}, 8, 150), {0, -1});
	creatureAnimationComponent.sprites[MovementState::WALK].second.setSprite(CreatureLayer::BODY, Sprite(SpriteSheet::COW, {0, 4}, {1, 2}, 8, 150), {0, -1});
	creatureAnimationComponent.sprites[MovementState::WALK].second.setSprite(CreatureLayer::HEAD, Sprite(SpriteSheet::COW, {0, 6}, {1, 2}, 8, 150), {0, -1});

	creatureAnimationComponent.sprites[MovementState::RUN].first.setSprite(CreatureLayer::BODY, Sprite(SpriteSheet::COW, {0, 0}, {1, 2}, 8, 100), {0, -1});
	creatureAnimationComponent.sprites[MovementState::RUN].first.setSprite(CreatureLayer::HEAD, Sprite(SpriteSheet::COW, {0, 2}, {1, 2}, 8, 100), {0, -1});
	creatureAnimationComponent.sprites[MovementState::RUN].second.setSprite(CreatureLayer::BODY, Sprite(SpriteSheet::COW, {0, 4}, {1, 2}, 8, 100), {0, -1});
	creatureAnimationComponent.sprites[MovementState::RUN].second.setSprite(CreatureLayer::HEAD, Sprite(SpriteSheet::COW, {0, 6}, {1, 2}, 8, 100), {0, -1});

	world->ecs.addComponent<CreatureAnimationComponent>(creatureAnimationComponent, animal);

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

Entity EntityFactory::createItem(ItemId::value itemId, uchar count, RealmId realmId, vec position) {
	Entity item = createItem(itemId, count);
	if (!item) return 0;

	pair chunk = vec::round(position / CHUNK_SIZE);
	world->ecs.addComponent<PositionComponent>({position, realmId, chunk}, item);
	world->realmManager.getRealm(realmId)->linkChunk(item, chunk);

	return item;
}

Entity EntityFactory::createTool(ItemKind::value itemKind) {
	Entity tool = world->ecs.createEntity();

	world->ecs.addComponent<ItemComponent>({ItemId::NONE, 1, true}, tool);
	world->ecs.addComponent<ColliderComponent>({Shape(vec(0.4f, 0.4f))}, tool);
	world->ecs.addComponent<DurabilityComponent>({255,255}, tool);

	ItemKindComponent itemKindComponent = {};
	itemKindComponent.itemKinds[itemKind] = true;

	SpriteComponent spriteComponent = {{}, 0.5f};
	spriteComponent.effects[SpriteEffectId::BOUNCE] = {true, 0};

	if (itemKind == ItemKind::SWORD) {
		world->ecs.addComponent<NameComponent>({Textblock("Sword")}, tool);
		spriteComponent.spriteStack.setSprite(0, Sprite(SpriteSheet::TOOLS, pair(0, 4)));
		spriteComponent.spriteStack.setSprite(1, Sprite(SpriteSheet::TOOLS, pair(0, 0)));
		spriteComponent.spriteStack.setSprite(2, Sprite(SpriteSheet::TOOLS, pair(1, 7)));	
		world->ecs.addComponent<MeleeItemComponent>({1}, tool);
		itemKindComponent.itemProperties[ItemProperty::DAMAGE] = 10;
		itemKindComponent.itemProperties[ItemProperty::PARRY] = 10;
	} else if (itemKind == ItemKind::BOW) {
		world->ecs.addComponent<NameComponent>({Textblock("Bow")}, tool);
		spriteComponent.spriteStack.setSprite(0, Sprite(SpriteSheet::TOOLS, pair(3, 4)));
		spriteComponent.spriteStack.setSprite(1, Sprite(SpriteSheet::TOOLS, pair(3, 5)));
		itemKindComponent.itemProperties[ItemProperty::DAMAGE] = 10;
		world->ecs.addComponent<LauncherComponent>({}, tool);
	} else if (itemKind == ItemKind::AXE) {
		world->ecs.addComponent<NameComponent>({Textblock("Axe")}, tool);
		spriteComponent.spriteStack.setSprite(0, Sprite(SpriteSheet::TOOLS, pair(0, 4)));
		spriteComponent.spriteStack.setSprite(1, Sprite(SpriteSheet::TOOLS, pair(2, 0)));
		spriteComponent.spriteStack.setSprite(2, Sprite(SpriteSheet::TOOLS, pair(2, 7)));
		itemKindComponent.itemProperties[ItemProperty::EFFICIENCY] = 10;
		itemKindComponent.itemProperties[ItemProperty::LEVEL] = 10;
	} else if (itemKind == ItemKind::HOE) {
		world->ecs.addComponent<NameComponent>({Textblock("Hoe")}, tool);
		spriteComponent.spriteStack.setSprite(0, Sprite(SpriteSheet::TOOLS, pair(0, 5)));
		spriteComponent.spriteStack.setSprite(1, Sprite(SpriteSheet::TOOLS, pair(4, 2)));
		spriteComponent.spriteStack.setSprite(2, Sprite(SpriteSheet::TOOLS, pair(2, 5)));
		itemKindComponent.itemProperties[ItemProperty::EFFICIENCY] = 10;
		itemKindComponent.itemProperties[ItemProperty::LEVEL] = 10;
	} else if (itemKind == ItemKind::PICK_AXE) {
		world->ecs.addComponent<NameComponent>({Textblock("Pick Axe")}, tool);
		spriteComponent.spriteStack.setSprite(0, Sprite(SpriteSheet::TOOLS, pair(0, 7)));
		spriteComponent.spriteStack.setSprite(1, Sprite(SpriteSheet::TOOLS, pair(1, 1)));
		spriteComponent.spriteStack.setSprite(2, Sprite(SpriteSheet::TOOLS, pair(2, 4)));
		itemKindComponent.itemProperties[ItemProperty::EFFICIENCY] = 10;
		itemKindComponent.itemProperties[ItemProperty::LEVEL] = 10;
	}

	world->ecs.addComponent<SpriteComponent>(spriteComponent, tool);
	world->ecs.addComponent<ItemKindComponent>(itemKindComponent, tool);

	return tool;
}

Entity EntityFactory::createStation(StationId::value stationId, RealmId realmId, pair position, bool link) {
	if (!stationId) return 0;
	Entity station = createStaticEntity(realmId, position, pair(1, 1), true, false);
	if (!station) return 0;

	world->ecs.addComponent<StationComponent>({stationId}, station);

	if (stationId == StationId::CAMP_FIRE) {
		ProcessingComponent processingComponent;
		processingComponent.processingSprites.setSprite(0, Sprite(SpriteSheet::FIRE, pair(1, 0), pair(1, 1), 4, 200));
		processingComponent.normarSprites.setSprite(0, Sprite(SpriteSheet::FIRE, pair(0, 0), pair(1, 1)));
		processingComponent.light = true;
		processingComponent.particle = ParticleId::SMOKE;

		processingComponent.input.itemKind = ItemKind::FUEL;

		world->ecs.addComponent<ProcessingComponent>(processingComponent, station);
		world->ecs.addComponent<SpriteComponent>({processingComponent.normarSprites}, station);
		
		return station;
	} else if (stationId == StationId::FURNACE) {
		ProcessingComponent processingComponent;
		processingComponent.processingSprites.setSprite(0, Sprite(SpriteSheet::FURNACE, pair(1, 0), pair(1, 2), 3, 200), pair(0, -1));
		processingComponent.normarSprites.setSprite(0, Sprite(SpriteSheet::FURNACE, pair(0, 0), pair(1, 2)), pair(0, -1));
		processingComponent.light = true;
		processingComponent.particle = ParticleId::SMOKE;

		processingComponent.input.itemKind = ItemKind::FUEL;

		world->ecs.addComponent<ProcessingComponent>(processingComponent, station);
		world->ecs.addComponent<SpriteComponent>({processingComponent.normarSprites}, station);
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

Entity EntityFactory::createProjectile(RealmId realmId, vec position, vec direction, Entity imune) {
	Entity projectile = createDynamicEntity(realmId, position);
	if (!projectile) return 0;

	SpriteStack spriteStack;
	spriteStack.setSprite(0, Sprite(SpriteSheet::ITEMS, {0, 9}, {1, 1}));
	SpriteComponent spriteComponent = {spriteStack};
	spriteComponent.angle = vec::angle(direction) - M_PI/4;
	world->ecs.addComponent<SpriteComponent>(spriteComponent, projectile);

	world->ecs.addComponent<HitboxComponent>({Shape(0.2f)}, projectile);
	world->ecs.addComponent<DamageComponent>({1, 0, 0, direction/10, imune}, projectile);

	world->ecs.addComponent<ProjectileComponent>({direction * 70}, projectile);

	return projectile;
}

Entity EntityFactory::createExplosive(RealmId realmId, vec position) {
	Entity explosive = createDynamicEntity(realmId, position);
	if (!explosive) return 0;

	SpriteStack spriteStack;
	spriteStack.setSprite(0, Sprite(SpriteSheet::BOMB, {0, 0}, {1, 1}));
	world->ecs.addComponent<SpriteComponent>({spriteStack}, explosive);

	world->ecs.addComponent<ExplosiveComponent>({world->ticks + 5000}, explosive);

	return  explosive;
}

Entity EntityFactory::createDamageArea(RealmId realmId, vec position, Shape shape, uint start, uint duration, vec force, Entity imune) {
	Entity damageArea = createDynamicEntity(realmId, position);
	if (!damageArea) return 0;

	world->ecs.addComponent<HitboxComponent>({shape}, damageArea);
	world->ecs.addComponent<DamageComponent>({1, start, duration, force, imune}, damageArea);
	return damageArea;
}

Entity EntityFactory::createPortal(RealmId realmId, pair position, RealmId otherRealmId, pair entry) {
	Entity portal = createStaticEntity(realmId, position, {1, 1}, true, true);
	SpriteStack portalSprites;
	portalSprites.setSprite(0, Sprite(SpriteSheet::CAVE, pair(0, 0), pair(1, 1)));
	world->ecs.addComponent<SpriteComponent>({portalSprites}, portal);
	world->ecs.addComponent<PortalComponent>({otherRealmId, entry}, portal);
	return portal;
}