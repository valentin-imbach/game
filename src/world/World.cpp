
#include "World.hpp"

#include "Components.hpp"
#include "Crafting.hpp"
#include "ECS_types.hpp"
#include "EntityFactory.hpp"
#include "Events.hpp"
#include "Generation.hpp"
#include "GuiElement.hpp"
#include "Item.hpp"
#include "ItemTemplates.hpp"
#include "Sprite.hpp"
#include "Tile.hpp"
#include "Window.hpp"
#include "utils.hpp"

void World::init() {
	rosterComponents();
	rosterSystems();

	ItemPropertyTemplate::setTemplates();
	ItemKindTemplate::setTemplates();
	ItemTemplate::setTemplates();
	ResourceTemplate::setTemplates();
	CraftingRecipe::setRecipes();
	CraftingKindRecipe::setRecipes();
	BuildKindRecipe::setRecipes();
	BiomeTemplate::setTemplates();
	TileTemplate::setTemplates();
	AnimalTemplate::setTemplates();

	guiManager.world = this;
	EntityFactory::world = this;
}

World::World(std::string name, uint seed, bool debug) : name(name), seed(seed), ticks(0), particleSystem(1000), realmManager(10) {
	init();

	spawnRealm = realmManager.addRealm(this, noise::UInt(seed + 1));
	spawn = spawnRealm->findFree(pair(50,50));
	Entity player = EntityFactory::createPlayer(spawnRealm, spawn);

	guiManager.add(std::make_unique<HotbarGui>(player));
	guiManager.add(std::make_unique<HealthBarGui>(player));

	if (debug) {
		Realm* house = realmManager.addRealm(this, noise::UInt(seed + 2), RealmType::HOUSE);
		Realm* cave = realmManager.addRealm(this, noise::UInt(seed + 3), RealmType::CAVE);

		Entity dam = EntityFactory::createDamageArea(spawnRealm, spawn + pair(1,1), Shape(1.0f), ticks, 0);
		
		//EntityFactory::createAnimal(CreatureId::COW, realm, realm->findFree(pair(52,52)));

		// Entity portal = EntityFactory::createResource(ResourceId::BASALT_ROCK, realm, spawn);
		// ecs.addComponent<PortalComponent>({otherRealm->realmId, pair(2, 2)}, portal);

		EntityFactory::createCrop(CropId::PARSNIP, spawnRealm, spawn + pair(1, -1));

		//LOG(ecs.getComponent<PositionComponent>(player).chunk);

		EntityFactory::createMonster(AnimalId::COW, spawnRealm, spawnRealm->findFree(pair(55,55)));

		Entity axe = ecs.createEntity();
		SpriteStack axeSprites;
		axeSprites.setSprite(0, Sprite(SpriteSheet::TOOLS, pair(0, 4)));
		axeSprites.setSprite(1, Sprite(SpriteSheet::TOOLS, pair(2, 0)));
		axeSprites.setSprite(2, Sprite(SpriteSheet::TOOLS, pair(2, 7)));
		ecs.addComponent<SpriteComponent>({axeSprites, 0.5f}, axe);
		ecs.getComponent<SpriteComponent>(axe).effects[SpriteEffectId::BOUNCE] = {true, 0};
		ecs.addComponent<ItemComponent>({ItemId::NONE, 1, true}, axe);
		ecs.addComponent<DurabilityComponent>({255,255}, axe);
		ItemKindComponent axeKindComponent = {};
		axeKindComponent.itemKinds[ItemKind::AXE] = true;
		axeKindComponent.itemProperties[ItemProperty::EFFICIENCY] = 3;
		axeKindComponent.itemProperties[ItemProperty::LEVEL] = 10;
		ecs.addComponent<ItemKindComponent>(axeKindComponent, axe);
		ecs.addComponent<ColliderComponent>({Shape(vec(0.4f, 0.4f))}, axe);
		ecs.addComponent<NameComponent>({Textblock("Axe")}, axe);
		Entity rest1 = ecs.getComponent<InventoryComponent>(player).inventory.add(axe);

		Entity hoe = ecs.createEntity();
		SpriteStack hoeSprites;
		hoeSprites.setSprite(0, Sprite(SpriteSheet::TOOLS, pair(0, 5)));
		hoeSprites.setSprite(1, Sprite(SpriteSheet::TOOLS, pair(4, 2)));
		hoeSprites.setSprite(2, Sprite(SpriteSheet::TOOLS, pair(2, 5)));
		ecs.addComponent<SpriteComponent>({hoeSprites, 0.5f}, hoe);
		ecs.getComponent<SpriteComponent>(hoe).effects[SpriteEffectId::BOUNCE] = {true, 0};
		ecs.addComponent<ItemComponent>({ItemId::NONE, 1, true}, hoe);
		ItemKindComponent hoeKindComponent = {};
		hoeKindComponent.itemKinds[ItemKind::HOE] = true;
		hoeKindComponent.itemProperties[ItemProperty::EFFICIENCY] = 3;
		hoeKindComponent.itemProperties[ItemProperty::LEVEL] = 2;
		ecs.addComponent<ItemKindComponent>(hoeKindComponent, hoe);
		ecs.addComponent<ColliderComponent>({Shape(vec(0.4f, 0.4f))}, hoe);
		ecs.addComponent<NameComponent>({Textblock("Hoe")}, hoe);
		Entity rest5 = ecs.getComponent<InventoryComponent>(player).inventory.add(hoe);

		Entity pick = ecs.createEntity();
		SpriteStack pickSprites;
		pickSprites.setSprite(0, Sprite(SpriteSheet::TOOLS, pair(0, 7)));
		pickSprites.setSprite(1, Sprite(SpriteSheet::TOOLS, pair(1, 1)));
		pickSprites.setSprite(2, Sprite(SpriteSheet::TOOLS, pair(2, 4)));
		ecs.addComponent<SpriteComponent>({pickSprites, 0.5f}, pick);
		ecs.getComponent<SpriteComponent>(pick).effects[SpriteEffectId::BOUNCE] = {true, 0};
		ecs.addComponent<ItemComponent>({ItemId::NONE, 1, true}, pick);
		ItemKindComponent pickKindComponent = {};
		pickKindComponent.itemKinds[ItemKind::PICK_AXE] = true;
		pickKindComponent.itemProperties[ItemProperty::EFFICIENCY] = 4;
		pickKindComponent.itemProperties[ItemProperty::LEVEL] = 3;
		ecs.addComponent<ItemKindComponent>(pickKindComponent, pick);
		ecs.addComponent<ColliderComponent>({Shape(vec(0.4f, 0.4f))}, pick);
		ecs.addComponent<NameComponent>({Textblock("Pick Axe")}, pick);
		Entity rest2 = ecs.getComponent<InventoryComponent>(player).inventory.add(pick);

		Entity sword = ecs.createEntity();
		SpriteStack swordSprites;
		swordSprites.setSprite(0, Sprite(SpriteSheet::TOOLS, pair(0, 4)));
		swordSprites.setSprite(1, Sprite(SpriteSheet::TOOLS, pair(0, 0)));
		swordSprites.setSprite(2, Sprite(SpriteSheet::TOOLS, pair(1, 7)));
		ecs.addComponent<SpriteComponent>({swordSprites, 0.5f}, sword);
		ecs.getComponent<SpriteComponent>(sword).effects[SpriteEffectId::BOUNCE] = {true, 0};
		ecs.addComponent<ItemComponent>({ItemId::NONE, 1, true}, sword);
		ecs.addComponent<MeleeItemComponent>({1}, sword);
		ecs.addComponent<ColliderComponent>({Shape(vec(0.4f, 0.4f))}, sword);
		ecs.addComponent<NameComponent>({Textblock("Sword")}, sword);
		Entity rest3 = ecs.getComponent<InventoryComponent>(player).inventory.add(sword);

		Entity bow = ecs.createEntity();
		SpriteStack bowSprites;
		bowSprites.setSprite(0, Sprite(SpriteSheet::TOOLS, pair(2, 8)));
		bowSprites.setSprite(1, Sprite(SpriteSheet::TOOLS, pair(1, 8)));
		ecs.addComponent<SpriteComponent>({bowSprites, 0.5f}, bow);
		ecs.getComponent<SpriteComponent>(bow).effects[SpriteEffectId::BOUNCE] = {true, 0};
		ecs.addComponent<ItemComponent>({ItemId::NONE, 1, true}, bow);
		ItemKindComponent bowKindComponent = {};
		bowKindComponent.itemKinds[ItemKind::BOW] = true;
		bowKindComponent.itemProperties[ItemProperty::DAMAGE] = 3;
		ecs.addComponent<ItemKindComponent>(bowKindComponent, bow);
		ecs.addComponent<ColliderComponent>({Shape(vec(0.4f, 0.4f))}, bow);
		ecs.addComponent<NameComponent>({Textblock("Bow")}, bow);
		ecs.addComponent<LauncherComponent>({}, bow);
		Entity rest4 = ecs.getComponent<InventoryComponent>(player).inventory.add(bow);

		// Entity chest = EntityFactory::createStation(StationId::CHEST, {10, 9});

		// Entity fire = ecs.createEntity();
		// ecs.addComponent<PositionComponent>({pair(11, 3)}, fire);
		// ecs.addComponent<GridComponent>({pair(11, 3), pair(1, 1), true, false}, fire);
		// SpriteStack fireSprites;
		// fireSprites.addSprite({SpriteSheet::FIRE, pair(0, 0), pair(1, 1), 4, 200});
		// ecs.addComponent<SpriteComponent>({fireSprites}, fire);
		// ecs.addComponent<ParticleComponent>({ParticleSystem::SMOKE}, fire);
		// ecs.addComponent<LightComponent>({true, 3, {255, 0, 0, 255}, 3, 0.2f}, fire);

		// Entity circle = ecs.createEntity();
		// ecs.addComponent<PositionComponent>({{10, 10}}, circle);
		// Shape circleCollider({0, 0}, 0.5f);
		// ecs.addComponent<ColliderComponent>({circleCollider}, circle);
	}
}

World::World(std::fstream& stream) : particleSystem(1000), realmManager(10) {
	init();

	deserialise_object(stream, seed);
	deserialise_object(stream, ticks);
	realmManager.deserialise(this, stream);
	ecs.deserialise(stream);

	player = playerSystem->getPlayer();

	guiManager.add(std::make_unique<HotbarGui>(player));
	guiManager.add(std::make_unique<HealthBarGui>(player));

	gridSystem->rebuild(realmManager);
	positionSystem->rebuild(realmManager);
}

void World::rosterComponents() {
	ecs.rosterComponent<PositionComponent>(ComponentId::POSITION);
	ecs.rosterComponent<SpriteComponent>(ComponentId::SPRITE);
	ecs.rosterComponent<CreatureStateComponent>(ComponentId::CREATURE_STATE);
	ecs.rosterComponent<ControllerComponent>(ComponentId::CONTROLLER);
	ecs.rosterComponent<DirectionComponent>(ComponentId::DIRECTION);
	ecs.rosterComponent<MovementComponent>(ComponentId::MOVEMENT);
	ecs.rosterComponent<ColliderComponent>(ComponentId::COLLIDER);
	ecs.rosterComponent<ItemComponent>(ComponentId::ITEM);
	ecs.rosterComponent<InventoryComponent>(ComponentId::INVENTORY);
	ecs.rosterComponent<HealthComponent>(ComponentId::HEALTH);
	ecs.rosterComponent<PlayerComponent>(ComponentId::PLAYER);
	ecs.rosterComponent<ResourceComponent>(ComponentId::RESOURCE);
	ecs.rosterComponent<LootComponent>(ComponentId::LOOT);
	ecs.rosterComponent<ItemKindComponent>(ComponentId::ITEM_KIND);
	ecs.rosterComponent<MeleeItemComponent>(ComponentId::MELEE_ITEM);
	ecs.rosterComponent<ForceComponent>(ComponentId::FORCE);
	ecs.rosterComponent<GridComponent>(ComponentId::GRID);
	ecs.rosterComponent<StationComponent>(ComponentId::INTERACTION);
	ecs.rosterComponent<NameComponent>(ComponentId::NAME);
	ecs.rosterComponent<DeathComponent>(ComponentId::DEATH);
	ecs.rosterComponent<ParticleComponent>(ComponentId::PARTICLE);
	ecs.rosterComponent<LightComponent>(ComponentId::LIGHT);
	ecs.rosterComponent<SensorComponent>(ComponentId::SENSOR);
	ecs.rosterComponent<ProjectileComponent>(ComponentId::PROJECTILE);
	ecs.rosterComponent<LauncherComponent>(ComponentId::LAUNCHER);
	ecs.rosterComponent<ChunkComponent>(ComponentId::CHUNK);
	ecs.rosterComponent<AiComponent>(ComponentId::AI);
	ecs.rosterComponent<AiWanderComponent>(ComponentId::AI_WANDER);
	ecs.rosterComponent<AiMoveComponent>(ComponentId::AI_MOVE);
	ecs.rosterComponent<AiFleeComponent>(ComponentId::AI_FLEE);
	ecs.rosterComponent<AiChaseComponent>(ComponentId::AI_CHASE);
	ecs.rosterComponent<AiMeleeComponent>(ComponentId::AI_MELEE);
	ecs.rosterComponent<PortalComponent>(ComponentId::PORTAL);
	ecs.rosterComponent<MaturityComponent>(ComponentId::MATURITY);
	ecs.rosterComponent<HitboxComponent>(ComponentId::HITBOX);
	ecs.rosterComponent<DamageComponent>(ComponentId::DAMAGE);
	ecs.rosterComponent<TagComponent>(ComponentId::TAG);
	ecs.rosterComponent<CreatureAnimationComponent>(ComponentId::CREATURE_ANIMATION);
	ecs.rosterComponent<DurabilityComponent>(ComponentId::DURABILITY);

	LOG("Components rostered");
}

void World::rosterSystems() {
	entityDrawSystem = ecs.rosterSystem<EntityDrawSystem>(SystemId::ENTITY_DRAW,
		{ComponentId::SPRITE, ComponentId::POSITION});
	creatureMovementSystem = ecs.rosterSystem<CreatureMovementSystem>(SystemId::CREATURE_MOVEMENT,
		{ComponentId::MOVEMENT, ComponentId::CREATURE_STATE, ComponentId::POSITION, ComponentId::COLLIDER});
	controllerSystem = ecs.rosterSystem<ControllerSystem>(SystemId::CONTROLLER,
		{ComponentId::CONTROLLER, ComponentId::CREATURE_STATE, ComponentId::DIRECTION});
	creatureAnimationSystem = ecs.rosterSystem<CreatureAnimationSystem>(SystemId::CREATURE_ANIMATION,
		{ComponentId::CREATURE_STATE, ComponentId::SPRITE, ComponentId::DIRECTION, ComponentId::CREATURE_ANIMATION});
	collisionSystem = ecs.rosterSystem<CollisionSystem>(SystemId::COLLISION,
		{ComponentId::COLLIDER, ComponentId::POSITION});
	itemPickupSystem = ecs.rosterSystem<ItemPickupSystem>(SystemId::ITEM_PICKUP,
		{ComponentId::PLAYER, ComponentId::INVENTORY});
	forageSystem = ecs.rosterSystem<ForageSystem>(SystemId::FORAGE,
		{ComponentId::RESOURCE, ComponentId::GRID, ComponentId::HEALTH});
	healthSystem = ecs.rosterSystem<HealthSystem>(SystemId::HEALTH,
		{ComponentId::HEALTH});
	lootSystem = ecs.rosterSystem<LootSystem>(SystemId::LOOT,
		{ComponentId::LOOT, ComponentId::DEATH, ComponentId::POSITION});
	playerSystem = ecs.rosterSystem<PlayerSystem>(SystemId::PLAYER,
		{ComponentId::PLAYER});
	colliderDrawSystem = ecs.rosterSystem<ColliderDrawSystem>(SystemId::COLLIDER_DRAW,
		{ComponentId::COLLIDER, ComponentId::POSITION});
	gridSystem = ecs.rosterSystem<GridSystem>(SystemId::GRID,
		{ComponentId::GRID});
	interactionSystem = ecs.rosterSystem<InteractionSystem>(SystemId::INTERACTION,
		{ComponentId::INTERACTION});
	deathSystem = ecs.rosterSystem<DeathSystem>(SystemId::DEATH,
		{ComponentId::DEATH});
	inventoryDeathSystem = ecs.rosterSystem<InventoryDeathSystem>(SystemId::INVENTORY_DEATH,
		{ComponentId::DEATH, ComponentId::INVENTORY});
	particleEmitSystem = ecs.rosterSystem<ParticleEmitSystem>(SystemId::PARTICLE_EMIT,
		{ComponentId::PARTICLE, ComponentId::POSITION});
	creatureParticleSystem = ecs.rosterSystem<CreatureParticleSystem>(SystemId::CREATURE_PARTICLE,
		{ComponentId::PARTICLE, ComponentId::CREATURE_STATE});
	handRenderSystem = ecs.rosterSystem<HandRenderSystem>(SystemId::HAND_RENDER,
		{ComponentId::POSITION, ComponentId::PLAYER, ComponentId::CREATURE_STATE});
	chunkSystem = ecs.rosterSystem<ChunkSystem>(SystemId::CHUNK,
		{ComponentId::POSITION, ComponentId::CHUNK});
	lightSystem = ecs.rosterSystem<LightSystem>(SystemId::LIGHT,
		{ComponentId::POSITION, ComponentId::LIGHT});
	sensorSystem = ecs.rosterSystem<SensorSystem>(SystemId::SENSOR,
		{ComponentId::POSITION, ComponentId::SENSOR});
	projectileSystem = ecs.rosterSystem<ProjectileSystem>(SystemId::PROJECTILE,
		{ComponentId::POSITION, ComponentId::PROJECTILE});
	aiSystem = ecs.rosterSystem<AiSystem>(SystemId::AI,
		{ComponentId::AI});
	aiWanderSystem = ecs.rosterSystem<AiWanderSystem>(SystemId::AI_WANDER,
		{ComponentId::AI, ComponentId::AI_WANDER});
	aiMoveSystem = ecs.rosterSystem<AiMoveSystem>(SystemId::AI_MOVE,
		{ComponentId::AI, ComponentId::AI_MOVE});
	aiFleeSystem = ecs.rosterSystem<AiFleeSystem>(SystemId::AI_FLEE,
		{ComponentId::AI, ComponentId::AI_FLEE});
	aiChaseSystem = ecs.rosterSystem<AiChaseSystem>(SystemId::AI_CHASE,
		{ComponentId::AI, ComponentId::AI_CHASE});
	aiMeleeSystem = ecs.rosterSystem<AiMeleeSystem>(SystemId::AI_MELEE,
		{ComponentId::AI, ComponentId::AI_MELEE});
	positionSystem = ecs.rosterSystem<PositionSystem>(SystemId::POSITION,
		{ComponentId::POSITION});
	maturitySystem = ecs.rosterSystem<MaturitySystem>(SystemId::MATURITY,
		{ComponentId::MATURITY});
	hitboxDrawSystem = ecs.rosterSystem<HitboxDrawSystem>(SystemId::HITBOX_DRAW,
		{ComponentId::HITBOX});
	damageSystem = ecs.rosterSystem<DamageSystem>(SystemId::DAMAGE,
		{ComponentId::POSITION, ComponentId::HITBOX, ComponentId::DAMAGE});
	hitboxSystem = ecs.rosterSystem<HitboxSystem>(SystemId::HITBOX,
		{ComponentId::POSITION, ComponentId::HITBOX});
	creatureActionSystem = ecs.rosterSystem<CreatureActionSystem>(SystemId::CREATURE_ACTION,
		{ComponentId::CREATURE_STATE});

	LOG("Systems rostered")
}

void World::update(uint dt) {
	ticks += tickSpeed * dt;
	time.update(tickSpeed * dt);
	player = playerSystem->getPlayer();

	if (player) {
		PositionComponent& positionComponent = ecs.getComponent<PositionComponent>(player);
		playerChunk = positionComponent.chunk;
		playerRealm = realmManager.getRealm(positionComponent.realmId);
	} else {
		if (playerAlive) {
			playerAlive = false;
			guiManager.add(makeDeathScreen());
		}
	}

	minimap.update(playerRealm);

	guiManager.update(dt);
	controllerSystem->update(inputState, state, ticks);

	updateSet.clear();
	const uchar updateDistance = 1;
	for (int x = -updateDistance; x <= updateDistance; x++) {
		for (int y = -updateDistance; y <= updateDistance; y++) {
			pair chunk(playerChunk.x + x, playerChunk.y + y);
			EntitySet& set = playerRealm->chunks[chunk];
        	updateSet.insert(set.begin(), set.end());
		}
	}

	sensorSystem->update(updateSet, ticks, realmManager);

	//aiMoveSystem->update(ticks, realmManager);
	aiWanderSystem->score(ticks);
	aiFleeSystem->score(ticks);
	aiChaseSystem->score(ticks);
	aiMeleeSystem->score(ticks);

	aiSystem->update(ticks);
	//aiMoveSystem->move(ticks);
	aiWanderSystem->update(ticks, realmManager);
	aiFleeSystem->update(ticks, realmManager);
	aiChaseSystem->update(ticks, realmManager);
	aiMeleeSystem->update(ticks, realmManager);

	creatureActionSystem->update(ticks, realmManager, forageSystem, updateSet);

	projectileSystem->update(ticks, dt);
	creatureMovementSystem->update(dt, realmManager);

	EntityMap collisions;
	collisionSystem->update(collisions, updateSet);

	EntityMap hits;
	hitboxSystem->update(hits, updateSet);

	damageSystem->update(hits, ticks);

	chunkSystem->update(realmManager);

	itemPickupSystem->update(collisions, realmManager);

	updateCamera(player);
	healthSystem->update(ticks, updateSet);
	maturitySystem->update(ticks, updateSet);
	lootSystem->update(ticks, playerRealm);

	creatureAnimationSystem->update(ticks);

	inventoryDeathSystem->update(ticks, realmManager);
	deathSystem->update(realmManager, particleSystem);

	creatureParticleSystem->update();
	particleEmitSystem->update(particleSystem, ticks);
	particleSystem.update(dt);
}

void World::updateCamera(Entity target) {
	if (!target || !ecs.hasComponent<PositionComponent>(target)) return;
	PositionComponent& positionComponent = ecs.getComponent<PositionComponent>(target);
	camera.position = positionComponent.position;
	camera.realmId = positionComponent.realmId;
}

void World::draw() {
	drawQueue.clear();
	drawTiles();

	if (state) {
		pair gridPosition = camera.screenPosition(vec::round(camera.worldPosition(Window::instance->mousePosition)));
		TextureManager::drawRect(gridPosition, pair(camera.zoom * BIT, camera.zoom * BIT), {0, 0, 255, 255});
	}

	EntitySet drawSet;
	pair cameraChunk = vec::round(camera.position / CHUNK_SIZE);
	Realm* cameraRealm = realmManager.getRealm(camera.realmId);
	const uchar renderDistance = 1;
	for (int x = -renderDistance; x <= renderDistance; x++) {
		for (int y = -renderDistance; y <= renderDistance; y++) {
			pair chunk(cameraChunk.x + x, cameraChunk.y + y);
			EntitySet& set = cameraRealm->chunks[chunk];
			drawSet.insert(set.begin(), set.end());
		}
	}

	entityDrawSystem->update(camera, drawQueue, state, player, ticks, drawSet);
	handRenderSystem->update(camera, drawQueue, ticks);

	auto lambda = [](auto& l, auto& r) {
		float ly = l.position.y + l.z;
		float ry = r.position.y + r.z;
		return ly < ry || (ly == ry && l.position.x < r.position.x);
	};

	std::sort(drawQueue.begin(), drawQueue.end(), lambda);
	for (auto& p : drawQueue) p.spriteStack.draw(p.position, p.scale, p.style, ticks);

	if (colliderDraw) {
		colliderDrawSystem->update(camera, ticks, drawSet);
		hitboxDrawSystem->update(camera, ticks, drawSet);
	}

	particleSystem.draw(camera);
	lightSystem->update(camera, time, ticks, drawSet);
	playerRealm->environment->draw(ticks);

	guiManager.draw();

	if (player) {
		vec playerPos = ecs.getComponent<PositionComponent>(player).position;
		minimap.draw(playerPos);
	}
	state = false;
}

void World::drawTiles() {
	pair range = vec::ceil(vec(Window::instance->size) / (2 * BIT * camera.zoom));
	pair start = vec::round(camera.position);

	int x1 = std::max(0, start.x - range.x);
	int x2 = std::min(playerRealm->map->size.x - 1, start.x + range.x);
	int y1 = std::max(0, start.y - range.y);
	int y2 = std::min(playerRealm->map->size.y - 1, start.y + range.y);

	for (int x = x1; x <= x2; x++) {
		for (int y = y1; y <= y2; y++) {
			if (!playerRealm->map->tiles[x][y]) continue;
			pair screenPosition = camera.screenPosition(vec(x, y));
			// for (auto& layer : playerRealm->map->tiles[x][y]->sprites) {
			// 	
			// 	layer.second.draw(screenPosition, camera.zoom, TextureStyle(), ticks);
			// }
			playerRealm->map->tiles[x][y]->sprites.draw(screenPosition, camera.zoom, TextureStyle(), ticks);
		}
	}
}

std::unique_ptr<GuiElement> World::makeInventory() {
	if (!player) return nullptr;
	InventoryComponent& inventoryComponent = ecs.getComponent<InventoryComponent>(player);
	PlayerComponent& playerComponent = ecs.getComponent<PlayerComponent>(player);
	Sprite sprite = Sprite(SpriteSheet::INVENTORY, {0, 0}, {10, 10});
	std::unique_ptr<Widget> gui = std::make_unique<Widget>(pair(0, 0), pair(150, 150), sprite);
	gui->addGuiElement(std::make_unique<InventoryGui>(pair(0, -60), &playerComponent.hotbar, 20, &inventoryComponent.inventory));
	gui->addGuiElement(std::make_unique<InventoryGui>(pair(0, 20), &inventoryComponent.inventory, 20, &playerComponent.hotbar));
	return gui;
}

std::unique_ptr<GuiElement> World::makeMenu() {
	if (!player) return nullptr;
	InventoryComponent& inventoryComponent = ecs.getComponent<InventoryComponent>(player);
	PlayerComponent& playerComponent = ecs.getComponent<PlayerComponent>(player);
	Sprite sprite = Sprite(SpriteSheet::MENU, {0, 0}, {10, 10});

	std::unique_ptr<TabWidget> gui = std::make_unique<TabWidget>(pair(0, 0), pair(150, 150));
	std::unique_ptr<Widget> tab1 = std::make_unique<Widget>(pair(0, 0), pair(150, 150), sprite);
	std::unique_ptr<Widget> tab2 = std::make_unique<Widget>(pair(0, 0), pair(150, 150), sprite);
	std::unique_ptr<Widget> tab3 = std::make_unique<Widget>(pair(0, 0), pair(150, 150), sprite);
	tab1->addGuiElement(std::make_unique<InventoryGui>(pair(-40, 10), &playerComponent.equipment, 20, &inventoryComponent.inventory));
	tab2->addGuiElement(std::make_unique<CraftingGui>(pair(0, 10), &inventoryComponent.inventory));
	tab3->addGuiElement(std::make_unique<BuildGui>(pair(0, 10), &inventoryComponent.inventory));

	gui->addTab(std::move(tab1));
	gui->addTab(std::move(tab2));
	gui->addTab(std::move(tab3));
	return gui;
}

void World::respawn() {
	Entity player = EntityFactory::createPlayer(spawnRealm, spawn);
	LOG("Player Respawned");
}

std::unique_ptr<GuiElement> World::makeDeathScreen() {
	Sprite sprite = Sprite(SpriteSheet::DEATH_SCREEN, {0, 0}, {10, 10});
	std::unique_ptr<Widget> widget = std::make_unique<Widget>(pair(0, 0), pair(150, 150), sprite);
	widget->addGuiElement(std::make_unique<Button>(pair(0, 0), pair(70, 50), std::bind(&World::respawn, this), Sprite(), Sprite(), "Respawn"));
	return widget;
}

bool World::handleEvent(InputEvent event, uint dt) {
	if (guiManager.handleEvent(event)) return true;
	if (minimap.handleEvent(event)) return true;
	if (!player) return false;
	
	PlayerComponent& playerComponent = ecs.getComponent<PlayerComponent>(player);
	ItemContainer& activeItemContainer = playerComponent.hotbar.itemContainers[playerComponent.activeSlot][0];

	if (event.id == InputEventId::INVENTORY) {
		guiManager.open(makeInventory(), makeMenu());
	} else if (event.id == InputEventId::THROW) {
		PositionComponent& positionComponent = ecs.getComponent<PositionComponent>(player);
		CreatureStateComponent& creatureStateComponent = ecs.getComponent<CreatureStateComponent>(player);
		vec position = positionComponent.position + Direction::unit[creatureStateComponent.facing];
		ecs.addComponent<PositionComponent>({position, positionComponent.realmId}, activeItemContainer.item);
		pair chunk = vec::round(position / CHUNK_SIZE);
		playerRealm->linkChunk(activeItemContainer.item, chunk);
		activeItemContainer.clear();
	} else if (event.id == InputEventId::SELECT_1) {
		playerComponent.activeSlot = 0;
	} else if (event.id == InputEventId::SELECT_2) {
		playerComponent.activeSlot = 1;
	} else if (event.id == InputEventId::SELECT_3) {
		playerComponent.activeSlot = 2;
	} else if (event.id == InputEventId::SELECT_4) {
		playerComponent.activeSlot = 3;
	} else if (event.id == InputEventId::SELECT_5) {
		playerComponent.activeSlot = 4;
	} else if (event.id == InputEventId::SELECT_6) {
		playerComponent.activeSlot = 5;
	} else if (event.id == InputEventId::SELECT_7) {
		playerComponent.activeSlot = 6;
	} else if (event.id == InputEventId::ROTATE_LEFT) {
		playerComponent.activeSlot = (playerComponent.activeSlot + 6) % 7;
	} else if (event.id == InputEventId::ROTATE_RIGHT) {
		playerComponent.activeSlot = (playerComponent.activeSlot + 1) % 7;
	} else if (event.id == InputEventId::PRIMARY) {
		vec position = camera.worldPosition(event.mousePosition);

		CreatureStateComponent& creatureStateComponent = ecs.getComponent<CreatureStateComponent>(player);
		if (creatureStateComponent.actionState == ActionState::IDLE) {
			creatureStateComponent.actionState = ActionState::ATTACK;
			creatureStateComponent.actionPosition = position;
			creatureStateComponent.actionStart = ticks;
			creatureStateComponent.actionEnd = ticks + 500;
		}

	} else if (event.id == InputEventId::SECONDARY) {
		vec position = camera.worldPosition(event.mousePosition);
		pair gridPos = vec::round(position);

		if (playerRealm->gridMap.find(gridPos) != playerRealm->gridMap.end()) {
			Entity entity = playerRealm->gridMap[gridPos];
			if (entity && ecs.hasComponent<PortalComponent>(entity)) {
				PortalComponent& portalComponent = ecs.getComponent<PortalComponent>(entity);
				PositionComponent& positionComponent = ecs.getComponent<PositionComponent>(player);
				playerRealm->unlinkChunk(player, positionComponent.chunk);
				positionComponent.position = portalComponent.position;
				pair chunk = vec::round(positionComponent.position / CHUNK_SIZE);
				positionComponent.chunk = chunk;
				positionComponent.realmId = portalComponent.realmId;
				Realm* realm = realmManager.getRealm(portalComponent.realmId);
				realm->linkChunk(player, chunk);
				return true;
			}
		} else {
			if (hasItemKind(activeItemContainer.item, ItemKind::HOE)) {
				TileId:: value tileId = playerRealm->map->getTileId(gridPos);
				if (tileId == TileId::DIRT || tileId == TileId::GRASS) {
					playerRealm->map->tiles[gridPos.x][gridPos.y]->tileId = TileId::SOIL;
					playerRealm->map->updateStyle(gridPos, true);
					return true;
				}
			} else {
				ItemComponent& itemComponent = ecs.getComponent<ItemComponent>(activeItemContainer.item);
				if (itemComponent.itemId == ItemId::PARSNIP_SEEDS) {
					EntityFactory::createCrop(CropId::PARSNIP, playerRealm, gridPos);
					itemComponent.count -= 1;
					if (itemComponent.count == 0) {
						ecs.addComponent<DeathComponent>({}, activeItemContainer.item);
						activeItemContainer.item = 0;
					}
					return true;
				}
			}
		}
		
		std::unique_ptr<GuiElement> gui = interactionSystem->update(position, updateSet);
		if (gui) guiManager.open(makeInventory(), std::move(gui));
	} else if (event.id == InputEventId::STATE) {
		state = true;
		if (ecs.hasComponent<LauncherComponent>(activeItemContainer.item)) {
			LauncherComponent& launcherComponent = ecs.getComponent<LauncherComponent>(activeItemContainer.item);
			if (inputState[InputStateId::SECONDARY]) {
				launcherComponent.charge +=  dt * launcherComponent.maxForce / launcherComponent.chargeTime;
				launcherComponent.charge = std::min(launcherComponent.charge, launcherComponent.maxForce);
			} else {
				if (launcherComponent.charge > launcherComponent.minForce) {
					vec position = camera.worldPosition(event.mousePosition);
					vec playerPosition = ecs.getComponent<PositionComponent>(player).position;
					vec direction = vec::normalise(position - playerPosition);
					EntityFactory::createProjectile(playerRealm, playerPosition, launcherComponent.charge * direction);
				}
				launcherComponent.charge = 0;
			}
		}
	}

	return false;
}

void World::serialise(std::fstream& stream) {
	serialise_object(stream, seed);
	serialise_object(stream, ticks);
	realmManager.serialise(stream);
	ecs.serialise(stream);
}