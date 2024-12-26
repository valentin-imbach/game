
#include "EntityFactory.hpp"
#include "ECS.hpp"
#include "Item.hpp"
#include "Components.hpp"
#include "World.hpp"

#include "ResourceTemplates.hpp"
#include "StructureTemplates.hpp"
#include "AnimalTemplates.hpp"

ECS* EntityFactory::ecs = nullptr;
World* EntityFactory::world = nullptr;
uint EntityFactory::seed = 1729;

Entity EntityFactory::createStaticEntity(RealmId realmId, pair position, pair size, bool solid, bool opaque) {
	Entity entity = ecs->createEntity();
	if (!entity) return 0;

	Realm* realm = world->realmManager.getRealm(realmId);
	pair chunk = realm->chunkManager.getChunk(position);

	if (!realm->free(position, size)) {
		WARNING("Trying to create static entity in occupied space");
		return 0;
	}

	ecs->addComponent<PositionComponent>({position, realmId}, entity);
	ecs->addComponent<GridComponent>({position, realmId, size, solid, opaque}, entity);

	realm->attach(entity, chunk);
	realm->link(entity, position, size, solid, opaque);
	return entity;
}

Entity EntityFactory::createDynamicEntity(RealmId realmId, vec position) {
	Entity entity = ecs->createEntity();
	if (!entity) return 0;

	Realm* realm = world->realmManager.getRealm(realmId);
	pair chunk = realm->chunkManager.getChunk(position);

	ecs->addComponent<PositionComponent>({position, realmId}, entity);
	realm->attach(entity, chunk);
	return entity;
}

Entity EntityFactory::createPlayer(RealmId realmId, vec position) {
	Entity player = createDynamicEntity(realmId, position);
	if (!player) return 0;

	ecs->addComponent<ActionComponent>({}, player);
	ecs->addComponent<DirectionComponent>({Direction::EAST}, player);
	ecs->addComponent<MovementComponent>({2, 3}, player);
	ecs->addComponent<ForceComponent>({vec(0, 0)}, player);
	ecs->addComponent<ControllerComponent>({}, player);
	ecs->addComponent<HandComponent>({}, player);

	ecs->addComponent<ColliderComponent>({Shape(vec(0.8f, 0.4f))}, player);
	ecs->addComponent<HitboxComponent>({Shape(vec(0.8f, 1.5f), vec(0, -0.6))}, player);

	ecs->addComponent<HealthComponent>({20, 20}, player);
	ecs->addComponent<ParticleComponent>({}, player);

	ecs->addComponent<EffectComponent>({}, player);

	ecs->addComponent<InventoryComponent>({Inventory(pair(7, 6))}, player);

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
	
	ecs->addComponent<PlayerComponent>({equipment, 0}, player);

	ecs->addComponent<FacingComponent>({}, player);

	TagComponent tagComponent = {};
	tagComponent.tags.set(EntityTag::PLAYER);
	ecs->addComponent<TagComponent>(tagComponent, player);

	ecs->addComponent<SpriteComponent>({}, player);
	CreatureAnimationComponent creatureAnimationComponent = {};

	for (int i = 1; i < CreatureLayer::count; i++) {
		creatureAnimationComponent.movements[MovementState::IDLE].setSprite(i, Sprite(SpriteSheet::MODULAR_PLAYER, pair(3, 2*i-2), pair(1, 2)), pair(0, -1));
		// creatureAnimationComponent.sprites[MovementState::IDLE].second.setSprite(i, Sprite(SpriteSheet::MODULAR_PLAYER, pair(3, 16 + 2*i-2), pair(1, 2)), pair(0, -1));
	}

	for (int i = 1; i < CreatureLayer::count; i++) {
		creatureAnimationComponent.movements[MovementState::WALK].setSprite(i, Sprite(SpriteSheet::MODULAR_PLAYER, pair(0, 2*i-2), pair(1, 2), 8, 100), pair(0, -1));
		// creatureAnimationComponent.sprites[MovementState::WALK].second.setSprite(i, Sprite(SpriteSheet::MODULAR_PLAYER, pair(0, 16 + 2*i-2), pair(1, 2), 8, 100), pair(0, -1));
	}

	ecs->addComponent<CreatureAnimationComponent>(creatureAnimationComponent, player);

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
	ecs->addComponent<SpriteComponent>({spriteStack}, resource);

	ecs->addComponent<ResourceComponent>({resourceTemplate.toolId, resourceTemplate.sound, resourceTemplate.level}, resource);
	ecs->addComponent<LootComponent>({resourceTemplate.lootTable}, resource);
	ecs->addComponent<HealthComponent>({resourceTemplate.health, resourceTemplate.health}, resource);
	// if (6 <= resourceId && resourceId <= 10) {
	// 	ecs->addComponent<MaturityComponent>({world->ticks, 10000, 5}, resource);
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
	ecs->addComponent<SpriteComponent>({spriteStack}, structure);
	return structure;
}

Entity EntityFactory::createCrop(CropId::value cropId, RealmId realmId, pair position) {
	if (!cropId) return 0;
	Entity crop = createStaticEntity(realmId, position, pair(1, 1), false, false);
	if (!crop) return 0;

	SpriteStack spriteStack;
	spriteStack.setSprite(0, Sprite(SpriteSheet::CROPS, pair(0, 2 * (cropId - 1)), pair(1, 2)), pair(0, -1));
	ecs->addComponent<SpriteComponent>({spriteStack}, crop);
	ecs->addComponent<MaturityComponent>({world->ticks, 5000, 5}, crop);
	return crop;
}

Entity EntityFactory::createEnemy(EnemyId::value enemyId, RealmId realmId, vec position) {
	Entity enemy = createDynamicEntity(realmId, position);
	if (!enemy) return 0;
	
	ecs->addComponent<ActionComponent>({}, enemy);
	ecs->addComponent<DirectionComponent>({Direction::EAST}, enemy);
	ecs->addComponent<ForceComponent>({{0, 0}}, enemy);
	ecs->addComponent<MovementComponent>({0.5f, 1}, enemy);

	ecs->addComponent<ColliderComponent>({Shape(vec(0.6f, 0.6f))}, enemy);
	ecs->addComponent<HitboxComponent>({Shape(vec(0.8f, 1.5f), vec(0, -0.6))}, enemy);

	ecs->addComponent<HealthComponent>({20, 20}, enemy);
	ecs->addComponent<ParticleComponent>({}, enemy);

	ecs->addComponent<SensorComponent>({10, EntityTag::PLAYER}, enemy);
	ecs->addComponent<AiComponent>({}, enemy);
	ecs->addComponent<AiWanderComponent>({position, {1, 0}}, enemy);
	ecs->addComponent<AiChaseComponent>({}, enemy);
	ecs->addComponent<AiFleeComponent>({10}, enemy);

	ecs->addComponent<AiPostComponent>({position, 10}, enemy);

	ecs->addComponent<FacingComponent>({}, enemy);

	Entity weapon = createTool(ItemKind::SWORD);
	InventoryComponent inventoryComponent = {Inventory(pair(1,1))};
	Entity rest = inventoryComponent.inventory.add(weapon);
	ecs->addComponent<InventoryComponent>(inventoryComponent, enemy);

	ecs->addComponent<AiMeleeComponent>({5, 1000, 0}, enemy);
	ecs->addComponent<HandComponent>({}, enemy);

	ecs->addComponent<SpriteComponent>({}, enemy);
	CreatureAnimationComponent creatureAnimationComponent = {};

	for (int i = 1; i < CreatureLayer::count; i++) {
		creatureAnimationComponent.movements[MovementState::IDLE].setSprite(i, Sprite(SpriteSheet::MODULAR_ZOMBIE, {3, 2*i-2}, {1, 2}), {0, -1});
		// creatureAnimationComponent.actions[MovementState::IDLE].setSprite(i, Sprite(SpriteSheet::MODULAR_ZOMBIE, {3, 16 + 2*i-2}, {1, 2}), {0, -1});
	}

	for (int i = 1; i < CreatureLayer::count; i++) {
		creatureAnimationComponent.movements[MovementState::WALK].setSprite(i, Sprite(SpriteSheet::MODULAR_ZOMBIE, {0, 2*i-2}, {1, 2}, 8, 100), {0, -1});
		// creatureAnimationComponent.actions[MovementState::WALK].setSprite(i, Sprite(SpriteSheet::MODULAR_ZOMBIE, {0, 16 + 2*i-2}, {1, 2}, 8, 100), {0, -1});
	}

	for (int i = 1; i < CreatureLayer::count; i++) {
		creatureAnimationComponent.movements[MovementState::RUN].setSprite(i, Sprite(SpriteSheet::MODULAR_ZOMBIE, {0, 2*i-2}, {1, 2}, 8, 100), {0, -1});
		// creatureAnimationComponent.actions[MovementState::RUN].setSprite(i, Sprite(SpriteSheet::MODULAR_ZOMBIE, {0, 16 + 2*i-2}, {1, 2}, 8, 100), {0, -1});
	}

	ecs->addComponent<CreatureAnimationComponent>(creatureAnimationComponent, enemy);

	return enemy;
}


Entity EntityFactory::createAnimal(AnimalId::value animalId, RealmId realmId, vec position) {
	if (!animalId) return 0;
	Entity animal = createDynamicEntity(realmId, position);
	if (!animal) return 0;

	ecs->addComponent<ActionComponent>({}, animal);
	ecs->addComponent<DirectionComponent>({Direction::EAST}, animal);
	ecs->addComponent<ForceComponent>({{0, 0}}, animal);
	ecs->addComponent<ParticleComponent>({}, animal);
	ecs->addComponent<SensorComponent>({5, EntityTag::PLAYER}, animal);
	ecs->addComponent<AiComponent>({}, animal);
	ecs->addComponent<AiWanderComponent>({position, {1, 0}}, animal);
	ecs->addComponent<AiFleeComponent>({50}, animal);
	ecs->addComponent<SpriteComponent>({}, animal);
	ecs->addComponent<FacingComponent>({}, animal);

	AnimalTemplate& animalTemplate = AnimalTemplate::templates[animalId];
	ecs->addComponent<MovementComponent>({animalTemplate.speed.x, animalTemplate.speed.y}, animal);
	ecs->addComponent<ColliderComponent>({animalTemplate.collider}, animal);	
	ecs->addComponent<HitboxComponent>({animalTemplate.hitbox}, animal);
	ecs->addComponent<LootComponent>({animalTemplate.lootTable}, animal);
	ecs->addComponent<HealthComponent>({animalTemplate.health, animalTemplate.health}, animal);



	TagComponent tagComponent = {};
	tagComponent.tags.set(EntityTag::ANIMAL);
	ecs->addComponent<TagComponent>(tagComponent, animal);

	if (animalId == AnimalId::COW) {
		ecs->addComponent<AiLureComponent>({ItemId::WHEAT}, animal);
	}

	CreatureAnimationComponent creatureAnimationComponent = { animalTemplate.sprites };
	// for (int i = 1; i < MovementState::count; i++) {
	// 	creatureAnimationComponent.movements[i].setSprite(CreatureLayer::BODY, animalTemplate.movements[i]);	
	// }
	// for (int i = 1; i < ActionState::count; i++) {
	// 	creatureAnimationComponent.actions[i].setSprite(CreatureLayer::HEAD, animalTemplate.actions[i]);	
	// }
	// creatureAnimationComponent.sprites[MovementState::IDLE].first.setSprite(CreatureLayer::BODY, Sprite(SpriteSheet::COW, {3, 0}, {1, 2}), {0, -1});
	// creatureAnimationComponent.sprites[MovementState::IDLE].first.setSprite(CreatureLayer::HEAD, Sprite(SpriteSheet::COW, {3, 2}, {1, 2}), {0, -1});
	// creatureAnimationComponent.sprites[MovementState::IDLE].second.setSprite(CreatureLayer::BODY, Sprite(SpriteSheet::COW, {3, 4}, {1, 2}), {0, -1});
	// creatureAnimationComponent.sprites[MovementState::IDLE].second.setSprite(CreatureLayer::HEAD, Sprite(SpriteSheet::COW, {3, 6}, {1, 2}), {0, -1});

	// creatureAnimationComponent.sprites[MovementState::WALK].first.setSprite(CreatureLayer::BODY, Sprite(SpriteSheet::COW, {0, 0}, {1, 2}, 8, 150), {0, -1});
	// creatureAnimationComponent.sprites[MovementState::WALK].first.setSprite(CreatureLayer::HEAD, Sprite(SpriteSheet::COW, {0, 2}, {1, 2}, 8, 150), {0, -1});
	// creatureAnimationComponent.sprites[MovementState::WALK].second.setSprite(CreatureLayer::BODY, Sprite(SpriteSheet::COW, {0, 4}, {1, 2}, 8, 150), {0, -1});
	// creatureAnimationComponent.sprites[MovementState::WALK].second.setSprite(CreatureLayer::HEAD, Sprite(SpriteSheet::COW, {0, 6}, {1, 2}, 8, 150), {0, -1});

	// creatureAnimationComponent.sprites[MovementState::RUN].first.setSprite(CreatureLayer::BODY, Sprite(SpriteSheet::COW, {0, 0}, {1, 2}, 8, 100), {0, -1});
	// creatureAnimationComponent.sprites[MovementState::RUN].first.setSprite(CreatureLayer::HEAD, Sprite(SpriteSheet::COW, {0, 2}, {1, 2}, 8, 100), {0, -1});
	// creatureAnimationComponent.sprites[MovementState::RUN].second.setSprite(CreatureLayer::BODY, Sprite(SpriteSheet::COW, {0, 4}, {1, 2}, 8, 100), {0, -1});
	// creatureAnimationComponent.sprites[MovementState::RUN].second.setSprite(CreatureLayer::HEAD, Sprite(SpriteSheet::COW, {0, 6}, {1, 2}, 8, 100), {0, -1});
	ecs->addComponent<CreatureAnimationComponent>(creatureAnimationComponent, animal);
	
	return animal;
}

Entity EntityFactory::createItem(ItemId::value itemId, uchar count, bool show) {
	Entity item = ecs->createEntity();
	if (!item) return 0;

	if (itemId) {
		SpriteStack spriteStack;
		spriteStack.setSprite(0, Sprite(SpriteSheet::ITEMS, pair((itemId - 1) % 5, (itemId - 1) / 5)));
		SpriteComponent spriteComponent = {spriteStack, 0.5f};
		spriteComponent.effects[SpriteEffectId::BOUNCE] = {true, 0};
		ecs->addComponent<SpriteComponent>(spriteComponent, item);
	}

	ecs->addComponent<ColliderComponent>({Shape(vec(0.4f, 0.4f))}, item);
	ecs->addComponent<ItemComponent>({itemId, count, show}, item);	
	return item;
}

Entity EntityFactory::createItem(ItemId::value itemId, uchar count, RealmId realmId, vec position) {
	Entity item = createItem(itemId, count);
	if (!item) return 0;

	Realm* realm = world->realmManager.getRealm(realmId); 
	pair chunk = vec::round(position / CHUNK_SIZE);
	
	ecs->addComponent<PositionComponent>({position, realmId}, item);
	realm->attach(item, chunk);
	return item;
}

Entity EntityFactory::createTool(ItemKind::value itemKind) {
	Entity tool = createItem(ItemId::NONE, 1, true);
	if (!tool) return 0;

	ecs->addComponent<DurabilityComponent>({255,255}, tool);

	ItemKindComponent itemKindComponent = {};
	itemKindComponent.itemKinds[itemKind] = true;

	SpriteComponent spriteComponent = {{}, 0.5f};
	spriteComponent.effects[SpriteEffectId::BOUNCE] = {true, 0};

	if (itemKind == ItemKind::SWORD) {
		ecs->addComponent<NameComponent>({Textblock("Sword")}, tool);
		spriteComponent.spriteStack.setSprite(0, Sprite(SpriteSheet::TOOLS, pair(0, 4)));
		spriteComponent.spriteStack.setSprite(1, Sprite(SpriteSheet::TOOLS, pair(0, 0)));
		spriteComponent.spriteStack.setSprite(2, Sprite(SpriteSheet::TOOLS, pair(1, 7)));	
		ecs->addComponent<MeleeItemComponent>({5}, tool);
		itemKindComponent.itemProperties[ItemProperty::DAMAGE] = 5;
		itemKindComponent.itemProperties[ItemProperty::PARRY] = 10;
	} else if (itemKind == ItemKind::BOW) {
		ecs->addComponent<NameComponent>({Textblock("Bow")}, tool);
		WindUpComponent windUpComponent;
		windUpComponent.sprites[0].setSprite(0, Sprite(SpriteSheet::TOOLS, pair(3, 4)));
		windUpComponent.sprites[0].setSprite(1, Sprite(SpriteSheet::TOOLS, pair(3, 5)));
		windUpComponent.sprites[1].setSprite(0, Sprite(SpriteSheet::TOOLS, pair(4, 4)));
		windUpComponent.sprites[1].setSprite(1, Sprite(SpriteSheet::TOOLS, pair(4, 5)));
		windUpComponent.sprites[2].setSprite(0, Sprite(SpriteSheet::TOOLS, pair(5, 4)));
		windUpComponent.sprites[2].setSprite(1, Sprite(SpriteSheet::TOOLS, pair(5, 5)));
		spriteComponent.spriteStack = windUpComponent.sprites[0];

		itemKindComponent.itemProperties[ItemProperty::DAMAGE] = 10;
		ecs->addComponent<WindUpComponent>(windUpComponent, tool);
		ecs->addComponent<LauncherComponent>({}, tool);
	} else if (itemKind == ItemKind::AXE) {
		ecs->addComponent<NameComponent>({Textblock("Axe")}, tool);
		spriteComponent.spriteStack.setSprite(0, Sprite(SpriteSheet::TOOLS, pair(0, 4)));
		spriteComponent.spriteStack.setSprite(1, Sprite(SpriteSheet::TOOLS, pair(2, 0)));
		spriteComponent.spriteStack.setSprite(2, Sprite(SpriteSheet::TOOLS, pair(2, 7)));
		itemKindComponent.itemProperties[ItemProperty::EFFICIENCY] = 10;
		itemKindComponent.itemProperties[ItemProperty::LEVEL] = 10;
	} else if (itemKind == ItemKind::HOE) {
		ecs->addComponent<NameComponent>({Textblock("Hoe")}, tool);
		spriteComponent.spriteStack.setSprite(0, Sprite(SpriteSheet::TOOLS, pair(0, 5)));
		spriteComponent.spriteStack.setSprite(1, Sprite(SpriteSheet::TOOLS, pair(4, 2)));
		spriteComponent.spriteStack.setSprite(2, Sprite(SpriteSheet::TOOLS, pair(2, 5)));
		itemKindComponent.itemProperties[ItemProperty::EFFICIENCY] = 10;
		itemKindComponent.itemProperties[ItemProperty::LEVEL] = 10;
	} else if (itemKind == ItemKind::PICK_AXE) {
		ecs->addComponent<NameComponent>({Textblock("Pick Axe")}, tool);
		spriteComponent.spriteStack.setSprite(0, Sprite(SpriteSheet::TOOLS, pair(0, 7)));
		spriteComponent.spriteStack.setSprite(1, Sprite(SpriteSheet::TOOLS, pair(1, 1)));
		spriteComponent.spriteStack.setSprite(2, Sprite(SpriteSheet::TOOLS, pair(2, 4)));
		itemKindComponent.itemProperties[ItemProperty::EFFICIENCY] = 10;
		itemKindComponent.itemProperties[ItemProperty::LEVEL] = 10;
	}

	ecs->addComponent<SpriteComponent>(spriteComponent, tool);
	ecs->addComponent<ItemKindComponent>(itemKindComponent, tool);

	return tool;
}

Entity EntityFactory::createBucket() {
	Entity bucket = createItem(ItemId::NONE, 1, true);
	if (!bucket) return 0;

	ecs->addComponent<NameComponent>({Textblock("Bucket")}, bucket);

	SpriteStack emptySprite;
	emptySprite.setSprite(0, Sprite(SpriteSheet::BUCKET, pair(0, 0)));
	SpriteStack fullSprite;
	fullSprite.setSprite(0, Sprite(SpriteSheet::BUCKET, pair(1, 0)));
	ecs->addComponent<TankComponent>({10, emptySprite, fullSprite}, bucket);

	ItemKindComponent itemKindComponent = {};
	itemKindComponent.itemKinds[ItemKind::TANK] = true;
	ecs->addComponent<ItemKindComponent>(itemKindComponent, bucket);

	SpriteComponent spriteComponent = {{}, 0.5f};
	spriteComponent.effects[SpriteEffectId::BOUNCE] = {true, 0};
	spriteComponent.spriteStack = emptySprite;
	ecs->addComponent<SpriteComponent>(spriteComponent, bucket);

	return bucket;
}

Entity EntityFactory::createStation(StationId::value stationId, RealmId realmId, pair position, bool link) {
	if (!stationId) return 0;
	Entity station = createStaticEntity(realmId, position, pair(1, 1), true, false);
	if (!station) return 0;

	ecs->addComponent<StationComponent>({stationId}, station);

	if (stationId == StationId::CAMP_FIRE) {
		FuelComponent fuelComponent;
		fuelComponent.hotSprites.setSprite(0, Sprite(SpriteSheet::FIRE, pair(1, 0), pair(1, 1), 4, 200));
		fuelComponent.coldSprites.setSprite(0, Sprite(SpriteSheet::FIRE, pair(0, 0), pair(1, 1)));
		fuelComponent.input.itemKind = ItemKind::FUEL;
		fuelComponent.light = true;
		fuelComponent.particle = ParticleId::SMOKE;
		ecs->addComponent<FuelComponent>(fuelComponent, station);
		ecs->addComponent<SpriteComponent>({fuelComponent.coldSprites}, station);

		ProcessingComponent processingComponent;
		processingComponent.input.itemKind = ItemKind::ORE;
		processingComponent.output.output = true;
		ecs->addComponent<ProcessingComponent>(processingComponent, station);
		
		return station;
	} else if (stationId == StationId::FURNACE) {
		FuelComponent fuelComponent;
		fuelComponent.hotSprites.setSprite(0, Sprite(SpriteSheet::FURNACE, pair(1, 0), pair(1, 2), 3, 200), pair(0, -1));
		fuelComponent.coldSprites.setSprite(0, Sprite(SpriteSheet::FURNACE, pair(0, 0), pair(1, 2)), pair(0, -1));
		fuelComponent.input.itemKind = ItemKind::FUEL;
		fuelComponent.light = true;
		fuelComponent.particle = ParticleId::SMOKE;
		ecs->addComponent<FuelComponent>(fuelComponent, station);
		ecs->addComponent<SpriteComponent>({fuelComponent.coldSprites}, station);

		ProcessingComponent processingComponent;
		processingComponent.input.itemKind = ItemKind::ORE;
		processingComponent.map[ItemId::BRONZE_ORE] = ItemId::BRONZE_INGOT;
		processingComponent.map[ItemId::SILVER_ORE] = ItemId::SILVER_INGOT;
		processingComponent.map[ItemId::GOLD_ORE] = ItemId::GOLD_INGOT;
		processingComponent.output.output = true;
		ecs->addComponent<ProcessingComponent>(processingComponent, station);
		
		return station;
	} else if (stationId == StationId::TOOL_STATION) {
		Inventory inventory(pair(3, 3));
		inventory.itemContainers[0][0].itemKind = ItemKind::PLATE;
		inventory.itemContainers[0][1].itemKind = ItemKind::ROPE;
		inventory.itemContainers[0][2].itemKind = ItemKind::ROD;
		inventory.itemContainers[1][0].itemKind = ItemKind::PLATE;
		inventory.itemContainers[1][1].itemKind = ItemKind::ROPE;
		inventory.itemContainers[1][2].itemKind = ItemKind::ROD;
		inventory.itemContainers[2][0].itemKind = ItemKind::PLATE;
		inventory.itemContainers[2][1].itemKind = ItemKind::ROPE;
		inventory.itemContainers[2][2].itemKind = ItemKind::ROD;
		ecs->addComponent<InventoryComponent>({inventory}, station);
	} else if (stationId == StationId::CHEST) {
		ecs->addComponent<InventoryComponent>({Inventory(pair(7, 5))}, station);
	}

	SpriteStack spriteStack;
	spriteStack.setSprite(0, Sprite(SpriteSheet::STATIONS, pair(int(stationId) - 1, 0), pair(1, 2)), pair(0, -1));
	ecs->addComponent<SpriteComponent>({spriteStack}, station);

	
	return station;
}

Entity EntityFactory::createProjectile( ProjectileId::value projectileId, RealmId realmId, vec position, vec direction, Entity imune) {
	Entity projectile = createDynamicEntity(realmId, position);
	if (!projectile) return 0;

	SpriteComponent spriteComponent = {};
	DamageComponent damageComponent = {};
	damageComponent.imune = imune;

	if (projectileId == ProjectileId::ARROW) {
		spriteComponent.spriteStack.setSprite(0, Sprite(SpriteSheet::ITEMS, {0, 9}, {1, 1}));
		spriteComponent.angle = vec::angle(direction) - M_PI/4;
		damageComponent.damage = 3;
		damageComponent.force = direction / 10;
	} else if (projectileId == ProjectileId::FIRE_BALL) {
		spriteComponent.spriteStack.setSprite(0, Sprite(SpriteSheet::FIRE_BALL));
		damageComponent.damage = 5;
		damageComponent.force = direction / 20;
	}
	
	ecs->addComponent<SpriteComponent>(spriteComponent, projectile);
	ecs->addComponent<HitboxComponent>({Shape(0.2f)}, projectile);
	ecs->addComponent<DamageComponent>(damageComponent, projectile);
	ecs->addComponent<ProjectileComponent>({direction * 70}, projectile);

	return projectile;
}

Entity EntityFactory::createExplosive(RealmId realmId, vec position) {
	Entity explosive = createDynamicEntity(realmId, position);
	if (!explosive) return 0;

	SpriteStack spriteStack;
	spriteStack.setSprite(0, Sprite(SpriteSheet::BOMB, {0, 0}, {1, 1}));
	ecs->addComponent<SpriteComponent>({spriteStack}, explosive);

	ecs->addComponent<ExplosiveComponent>({world->ticks + 5000}, explosive);

	return  explosive;
}

Entity EntityFactory::createDamageArea(RealmId realmId, vec position, Shape shape, uint start, uint duration, int damage, vec force, Entity imune) {
	Entity damageArea = createDynamicEntity(realmId, position);
	if (!damageArea) return 0;

	ecs->addComponent<HitboxComponent>({shape}, damageArea);
	ecs->addComponent<DamageComponent>({damage, start, duration, force, imune}, damageArea);
	return damageArea;
}

Entity EntityFactory::createPortal(RealmId realmId, pair position, RealmId otherRealmId, pair entry) {
	Entity portal = createStaticEntity(realmId, position, {1, 1}, true, true);
	SpriteStack portalSprites;
	portalSprites.setSprite(0, Sprite(SpriteSheet::CAVE, pair(0, 0), pair(1, 1)));
	ecs->addComponent<SpriteComponent>({portalSprites}, portal);
	ecs->addComponent<PortalComponent>({otherRealmId, entry}, portal);
	return portal;
}

Entity EntityFactory::createSpawner(RealmId realmId, pair position, EnemyId::value enemyId) {
	Entity spawner = createStaticEntity(realmId, position, pair(1,1), true, true);
	SpriteStack spawnerSprites;
	spawnerSprites.setSprite(0, Sprite(SpriteSheet::SPAWNER, pair(0,0), pair(1,2)), pair(0, -1));
	ecs->addComponent<SpriteComponent>({spawnerSprites}, spawner);
	ecs->addComponent<SpawnerComponent>({enemyId, world->ticks}, spawner);
	return spawner;
}



Entity EntityFactory::createBoomerang() {
	Entity tool = createItem(ItemId::NONE, 1, true);
	if (!tool) return 0;

	ecs->addComponent<DurabilityComponent>({255,255}, tool);

	ItemKindComponent itemKindComponent = {};
	// itemKindComponent.itemKinds[itemKind] = true;

	SpriteComponent spriteComponent = {{}, 0.5f};
	spriteComponent.effects[SpriteEffectId::BOUNCE] = {true, 0};

	ecs->addComponent<NameComponent>({Textblock("Boomerang")}, tool);
	spriteComponent.spriteStack.setSprite(0, Sprite(SpriteSheet::BOOMERANG, pair(0, 0)));
	itemKindComponent.itemProperties[ItemProperty::DAMAGE] = 10;

	WindUpComponent winduUpComponent;
	winduUpComponent.sprites[0].setSprite(0, Sprite(SpriteSheet::BOOMERANG, pair(0, 0)));
	winduUpComponent.sprites[1].setSprite(0, Sprite(SpriteSheet::BOOMERANG, pair(1, 0)));
	winduUpComponent.sprites[2].setSprite(0, Sprite(SpriteSheet::BOOMERANG, pair(2, 0)));
	ecs->addComponent<WindUpComponent>(winduUpComponent, tool);
	
	ecs->addComponent<SpriteComponent>(spriteComponent, tool);
	ecs->addComponent<ItemKindComponent>(itemKindComponent, tool);

	return tool;
}



Entity EntityFactory::createStaff() {
	Entity tool = createItem(ItemId::NONE, 1, true);
	if (!tool) return 0;

	ecs->addComponent<DurabilityComponent>({255,255}, tool);

	ItemKindComponent itemKindComponent = {};
	SpriteComponent spriteComponent = {{}, 0.5f};
	spriteComponent.effects[SpriteEffectId::BOUNCE] = {true, 0};

	ecs->addComponent<NameComponent>({Textblock("Fire Staff")}, tool);
	spriteComponent.spriteStack.setSprite(0, Sprite(SpriteSheet::STAFF, pair(0, 0)));
	itemKindComponent.itemProperties[ItemProperty::DAMAGE] = 10;

	ecs->addComponent<SpriteComponent>(spriteComponent, tool);
	ecs->addComponent<ItemKindComponent>(itemKindComponent, tool);
	ecs->addComponent<LauncherComponent>({0.4f}, tool);

	return tool;
}