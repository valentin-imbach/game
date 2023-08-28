
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

	guiManager.world = this;
	EntityFactory::world = this;
}

World::World(std::string name, uint seed) : name(name), seed(seed), ticks(0), particleSystem(1000), realmManager(10) {
	init();

	Realm* realm = realmManager.addRealm(this, noise::UInt(seed + 1));
	Realm* house = realmManager.addRealm(this, noise::UInt(seed + 2), RealmType::HOUSE);
	Realm* cave = realmManager.addRealm(this, noise::UInt(seed + 3), RealmType::CAVE);

	pair spawn = realm->findFree(pair(50,50));
	Entity player = EntityFactory::createPlayer(realm, spawn);
	
	
	//EntityFactory::createAnimal(CreatureId::COW, realm, realm->findFree(pair(52,52)));

	// Entity portal = EntityFactory::createResource(ResourceId::BASALT_ROCK, realm, spawn);
	// ecs.addComponent<PortalComponent>({otherRealm->realmId, pair(2, 2)}, portal);


	// Entity tree = ecs.createEntity();
	// pair chunk = vec::round(spawn / CHUNK_SIZE);
	// ecs.addComponent<PositionComponent>({spawn, realm->realmId, chunk}, tree);
	// realm->linkChunk(tree, chunk);
	// ecs.addComponent<GridComponent>({spawn, realm->realmId, pair(1, 1), true, true}, tree);
	// realm->linkGrid(tree, spawn, pair(1, 1), true, true);
	// SpriteStack spriteStack;
	// spriteStack.addSprite({SpriteSheet::RESOURCES_NEW, pair(0, 0), pair(1, 2)}, pair(0, -1));
	// spriteStack.addSprite({SpriteSheet::RESOURCES_NEW, pair(0, 2), pair(3, 3)}, pair(-1, -3));
	// ecs.addComponent<SpriteComponent>({spriteStack}, tree);
	// ecs.addComponent<MaturityComponent>({ticks, 10000, 5}, tree);

	//LOG(ecs.getComponent<PositionComponent>(player).chunk);

	EntityFactory::createMonster(CreatureId::MONSTER, realm, realm->findFree(pair(55,55)));

	guiManager.add(std::make_unique<HotbarGui>(player));
	guiManager.add(std::make_unique<HealthBarGui>(player));

	Entity axe = ecs.createEntity();
	SpriteStack axeSprites;
	axeSprites.addSprite(Sprite(SpriteSheet::ITEMS, pair(2, 0)));
	ecs.addComponent<SpriteComponent>({axeSprites, 0.5f}, axe);
	ecs.getComponent<SpriteComponent>(axe).effects[SpriteEffectId::BOUNCE] = {true, 0};
	ecs.addComponent<ItemComponent>({ItemId::NONE, 1, true}, axe);
	ItemKindComponent axeKindComponent = {};
	axeKindComponent.itemKinds[ItemKind::AXE] = true;
	axeKindComponent.itemProperties[ItemProperty::EFFICIENCY] = 3;
	axeKindComponent.itemProperties[ItemProperty::LEVEL] = 2;
	ecs.addComponent<ItemKindComponent>(axeKindComponent, axe);
	Shape axeCollider({0, 0}, {0.4f, 0.4f});
	ecs.addComponent<ColliderComponent>({axeCollider}, axe);
	ecs.addComponent<NameComponent>({Textblock("Axe")}, axe);
	Entity rest1 = ecs.getComponent<InventoryComponent>(player).inventory.add(axe);

	Entity pick = ecs.createEntity();
	SpriteStack pickSprites;
	pickSprites.addSprite(Sprite(SpriteSheet::ITEMS, pair(1, 0)));
	ecs.addComponent<SpriteComponent>({pickSprites, 0.5f}, pick);
	ecs.getComponent<SpriteComponent>(pick).effects[SpriteEffectId::BOUNCE] = {true, 0};
	ecs.addComponent<ItemComponent>({ItemId::NONE, 1, true}, pick);
	ItemKindComponent pickKindComponent = {};
	pickKindComponent.itemKinds[ItemKind::PICK_AXE] = true;
	pickKindComponent.itemProperties[ItemProperty::EFFICIENCY] = 4;
	pickKindComponent.itemProperties[ItemProperty::LEVEL] = 3;
	ecs.addComponent<ItemKindComponent>(pickKindComponent, pick);
	Shape pickCollider(vec(0,0), vec(0.4f, 0.4f));
	ecs.addComponent<ColliderComponent>({pickCollider}, pick);
	ecs.addComponent<NameComponent>({Textblock("Pick Axe")}, pick);
	Entity rest2 = ecs.getComponent<InventoryComponent>(player).inventory.add(pick);

	Entity sword = ecs.createEntity();
	SpriteStack swordSprites;
	swordSprites.addSprite(Sprite(SpriteSheet::ITEMS, pair(0, 0)));
	ecs.addComponent<SpriteComponent>({swordSprites, 0.5f}, sword);
	ecs.getComponent<SpriteComponent>(sword).effects[SpriteEffectId::BOUNCE] = {true, 0};
	ecs.addComponent<ItemComponent>({ItemId::NONE, 1, true}, sword);
	ecs.addComponent<DamageComponent>({1}, sword);
	Shape swordCollider(vec(0, 0), vec(0.4f, 0.4f));
	ecs.addComponent<ColliderComponent>({swordCollider}, sword);
	ecs.addComponent<NameComponent>({Textblock("Sword")}, sword);
	Entity rest3 = ecs.getComponent<InventoryComponent>(player).inventory.add(sword);

	Entity bow = ecs.createEntity();
	SpriteStack bowSprites;
	bowSprites.addSprite(Sprite(SpriteSheet::ITEMS, pair(4, 0)));
	ecs.addComponent<SpriteComponent>({bowSprites, 0.5f}, bow);
	ecs.getComponent<SpriteComponent>(bow).effects[SpriteEffectId::BOUNCE] = {true, 0};
	ecs.addComponent<ItemComponent>({ItemId::NONE, 1, true}, bow);
	ItemKindComponent bowKindComponent = {};
	bowKindComponent.itemKinds[ItemKind::BOW] = true;
	bowKindComponent.itemProperties[ItemProperty::DAMAGE] = 3;
	ecs.addComponent<ItemKindComponent>(bowKindComponent, bow);
	Shape bowCollider(vec(0, 0), vec(0.4f, 0.4f));
	ecs.addComponent<ColliderComponent>({bowCollider}, bow);
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
	ecs.rosterComponent<DamageComponent>(ComponentId::DAMAGE);
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
		{ComponentId::CREATURE_STATE, ComponentId::SPRITE, ComponentId::DIRECTION});
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
	damageSystem = ecs.rosterSystem<DamageSystem>(SystemId::DAMAGE,
		{ComponentId::POSITION, ComponentId::HITBOX, ComponentId::HEALTH});
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

	LOG("Systems rostered")
}

void World::update(uint dt) {
	ticks += dt;
	time.update(dt);
	player = playerSystem->getPlayer();

	if (player) {
		PositionComponent& positionComponent = ecs.getComponent<PositionComponent>(player);
		playerChunk = positionComponent.chunk;
		playerRealm = realmManager.getRealm(positionComponent.realmId);
	}

	minimap.update(playerRealm);

	std::unordered_map<Entity, std::vector<Entity>> collisions;

	guiManager.update();
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

	sensorSystem->update(player, ticks, realmManager);

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

	projectileSystem->update(ticks, dt);
	creatureMovementSystem->update(dt, realmManager);
	collisionSystem->update(collisions, updateSet);

	chunkSystem->update(realmManager);

	itemPickupSystem->update(collisions, realmManager);

	updateCamera(player);
	healthSystem->update(ticks, updateSet);
	maturitySystem->update(ticks, updateSet);
	lootSystem->update(ticks, playerRealm);

	creatureAnimationSystem->update(ticks);

	inventoryDeathSystem->update(ticks);
	deathSystem->update(realmManager);

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
			for (auto& layer : playerRealm->map->tiles[x][y]->sprites) {
				pair screenPosition = camera.screenPosition(vec(x, y));
				layer.second.draw(screenPosition, camera.zoom);
			}
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
		if (ticks - playerComponent.lastAction > 500) {
			if (damageSystem->update(player, position, activeItemContainer.item, ticks, updateSet)) {
				playerComponent.lastAction = ticks;
				return true;
			} else if (forageSystem->update(position, activeItemContainer.item, ticks, updateSet)) {
				playerComponent.lastAction = ticks;
				return true;
			}
		}
	} else if (event.id == InputEventId::SECONDARY) {
		vec position = camera.worldPosition(event.mousePosition);
		Entity entity = playerRealm->gridMap[vec::round(position)];
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