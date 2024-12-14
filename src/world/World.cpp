
#include "World.hpp"
#include "systems.hpp"

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

#include "Game.hpp"

#include "StructureTemplates.hpp"
#include "AnimalTemplates.hpp"

#include "ClusterTemplates.hpp"

#include <thread>

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
	GroundTemplate::setTemplates();
	WallTemplate::setTemplates();
	AnimalTemplate::setTemplates();
	StructureTemplate::setTemplates();
	Cluster::setTemplates();

	guiManager.world = this;
	EntityFactory::world = this;
}

World::World(std::string name, uint seed, WorldParameters params) : name(name), seed(seed), ticks(0), particleSystem(1000), realmManager(10) {
	init();

	spawnRealm = realmManager.addRealm(this, noise::UInt(seed + 1));
	spawnRealm->generate(params);

	spawn = pair(5,5);
	pair spawnChunk = spawnRealm->chunkManager.getChunk(spawn);

	spawnRealm->chunkManager.generateChunk(spawnChunk, ChunkStage::LOADED, spawnRealm->environment.get());
	player = EntityFactory::createPlayer(spawnRealm->realmId, spawn);

	guiManager.add(std::make_unique<HotbarGui>(player));
	guiManager.add(std::make_unique<HealthBarGui>(player));

	Realm* house = realmManager.addRealm(this, noise::UInt(seed + 2));
	house->generateHouse();

	EntityFactory::createPortal(spawnRealm->realmId, spawn, house->realmId, pair(8,8));

	Realm* otherRealm = realmManager.addRealm(this, noise::UInt(seed + 2));
	otherRealm->generateFlat();

	Realm* dungeon = realmManager.addRealm(this, noise::UInt(seed + 3));
	dungeon->generateDungeon2();

	// if (debug) {
	// 	Realm* house = realmManager.addRealm(this, noise::UInt(seed + 2));
	// 	house->generate(RealmType::HOUSE);

	// 	Realm* cave = realmManager.addRealm(this, noise::UInt(seed + 3));
	// 	cave->generate(RealmType::CAVE);

	// 	Realm* dungeon = realmManager.addRealm(this, noise::UInt(seed + rand()));
	// 	dungeon->generate(RealmType::DUNGEON);

		//Entity dam = EntityFactory::createDamageArea(spawnRealm, spawn + pair(1,1), Shape(1.0f), ticks, 0);

		// for (int x = 0; x < 6; x++) {
		// 	spawnRealm->map->tiles[spawn.x + x][spawn.y]->wallId = GroundId::MUD_WALL;
		// }

		// for (int x = 1; x < 5; x++) {
		// 	spawnRealm->map->tiles[spawn.x + x][spawn.y + 1]->wallId = GroundId::MUD_WALL;
		// }

		// spawnRealm->map->updateStyle();

		// EntityFactory::createPortal(spawnRealm->realmId, spawn + pair(3, 1), dungeon->realmId, dungeon->spawn);

		// EntityFactory::createStructure(StructureId::TENT, spawnRealm->realmId, spawn + pair(0, 3));
		
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
	// }
}

World::World(std::fstream& stream) : particleSystem(1000), realmManager(10) {
	init();

	deserialise_object(stream, seed);
	deserialise_object(stream, ticks);
	deserialise_object(stream, player);
	realmManager.deserialise(this, stream);
	// ecs.deserialise(stream);

	guiManager.add(std::make_unique<HotbarGui>(player));
	guiManager.add(std::make_unique<HealthBarGui>(player));

	gridSystem->rebuild(realmManager);
	positionSystem->rebuild(realmManager);
}

void World::rosterComponents() {
	ecs.rosterComponent<PositionComponent>(ComponentId::POSITION);
	ecs.rosterComponent<SpriteComponent>(ComponentId::SPRITE);
	ecs.rosterComponent<ActionComponent>(ComponentId::ACTION);
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
	ecs.rosterComponent<AiComponent>(ComponentId::AI);
	ecs.rosterComponent<AiWanderComponent>(ComponentId::AI_WANDER);
	ecs.rosterComponent<AiFleeComponent>(ComponentId::AI_FLEE);
	ecs.rosterComponent<AiChaseComponent>(ComponentId::AI_CHASE);
	ecs.rosterComponent<AiMeleeComponent>(ComponentId::AI_MELEE);
	ecs.rosterComponent<AiPostComponent>(ComponentId::AI_POST);
	ecs.rosterComponent<PortalComponent>(ComponentId::PORTAL);
	ecs.rosterComponent<MaturityComponent>(ComponentId::MATURITY);
	ecs.rosterComponent<HitboxComponent>(ComponentId::HITBOX);
	ecs.rosterComponent<DamageComponent>(ComponentId::DAMAGE);
	ecs.rosterComponent<TagComponent>(ComponentId::TAG);
	ecs.rosterComponent<CreatureAnimationComponent>(ComponentId::CREATURE_ANIMATION);
	ecs.rosterComponent<DurabilityComponent>(ComponentId::DURABILITY);
	ecs.rosterComponent<EffectComponent>(ComponentId::EFFECT);
	ecs.rosterComponent<ExplosiveComponent>(ComponentId::EXPLOSIVE);
	ecs.rosterComponent<ItemModComponent>(ComponentId::ITEM_MOD);
	ecs.rosterComponent<HandComponent>(ComponentId::HAND);
	ecs.rosterComponent<ProcessingComponent>(ComponentId::PROCESSING);
	ecs.rosterComponent<FuelComponent>(ComponentId::FUEL);
	ecs.rosterComponent<TankComponent>(ComponentId::TANK);
	ecs.rosterComponent<SpawnerComponent>(ComponentId::SPAWNER);
	ecs.rosterComponent<AiLureComponent>(ComponentId::AI_LURE);
	ecs.rosterComponent<WindUpComponent>(ComponentId::WIND_UP);
	ecs.rosterComponent<FacingComponent>(ComponentId::FACING);

	LOG("Components rostered");
}

void World::rosterSystems() {
	entityDrawSystem = ecs.rosterSystem<EntityDrawSystem>(SystemId::ENTITY_DRAW,
		{ComponentId::SPRITE, ComponentId::POSITION});
	creatureMovementSystem = ecs.rosterSystem<CreatureMovementSystem>(SystemId::CREATURE_MOVEMENT,
		{ComponentId::MOVEMENT, ComponentId::POSITION, ComponentId::COLLIDER});
	controllerSystem = ecs.rosterSystem<ControllerSystem>(SystemId::CONTROLLER,
		{ComponentId::CONTROLLER, ComponentId::MOVEMENT, ComponentId::DIRECTION});
	creatureAnimationSystem = ecs.rosterSystem<CreatureAnimationSystem>(SystemId::CREATURE_ANIMATION,
		{ComponentId::MOVEMENT, ComponentId::SPRITE, ComponentId::DIRECTION, ComponentId::CREATURE_ANIMATION});
	collisionSystem = ecs.rosterSystem<CollisionSystem>(SystemId::COLLISION,
		{ComponentId::COLLIDER, ComponentId::POSITION});
	itemPickupSystem = ecs.rosterSystem<ItemPickupSystem>(SystemId::ITEM_PICKUP,
		{ComponentId::COLLIDER, ComponentId::INVENTORY});
	forageSystem = ecs.rosterSystem<ForageSystem>(SystemId::FORAGE,
		{ComponentId::RESOURCE, ComponentId::GRID, ComponentId::HEALTH});
	healthSystem = ecs.rosterSystem<HealthSystem>(SystemId::HEALTH,
		{ComponentId::HEALTH});
	lootSystem = ecs.rosterSystem<LootSystem>(SystemId::LOOT,
		{ComponentId::LOOT, ComponentId::DEATH, ComponentId::POSITION});
	playerSystem = ecs.rosterSystem<PlayerSystem>(SystemId::PLAYER,
		{ComponentId::PLAYER, ComponentId::HAND});
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
		{ComponentId::PARTICLE, ComponentId::MOVEMENT});
	handRenderSystem = ecs.rosterSystem<HandRenderSystem>(SystemId::HAND_RENDER,
		{ComponentId::POSITION, ComponentId::ACTION, ComponentId::MOVEMENT, ComponentId::HAND, ComponentId::FACING});
	// chunkSystem = ecs.rosterSystem<ChunkSystem>(SystemId::CHUNK,
	// 	{ComponentId::POSITION, ComponentId::CHUNK});
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
	// aiMoveSystem = ecs.rosterSystem<AiMoveSystem>(SystemId::AI_MOVE,
	// 	{ComponentId::AI, ComponentId::AI_MOVE});
	aiFleeSystem = ecs.rosterSystem<AiFleeSystem>(SystemId::AI_FLEE,
		{ComponentId::AI, ComponentId::SENSOR, ComponentId::AI_FLEE, ComponentId::HEALTH});
	aiChaseSystem = ecs.rosterSystem<AiChaseSystem>(SystemId::AI_CHASE,
		{ComponentId::AI, ComponentId::SENSOR, ComponentId::AI_CHASE, ComponentId::POSITION, ComponentId::MOVEMENT, ComponentId::DIRECTION, ComponentId::FACING});
	aiMeleeSystem = ecs.rosterSystem<AiMeleeSystem>(SystemId::AI_MELEE,
		{ComponentId::AI, ComponentId::SENSOR, ComponentId::AI_MELEE});
	aiPostSystem = ecs.rosterSystem<AiPostSystem>(SystemId::AI_POST,
		{ComponentId::AI, ComponentId::AI_POST});
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
		{ComponentId::ACTION});
	effectSystem = ecs.rosterSystem<EffectSystem>(SystemId::EFFECT,
		{ComponentId::EFFECT});
	explosiveSystem = ecs.rosterSystem<ExplosiveSystem>(SystemId::EXPLOSIVE,
		{ComponentId::EXPLOSIVE, ComponentId::POSITION});
	itemModSystem = ecs.rosterSystem<ItemModSystem>(SystemId::ITEM_MOD,
		{ComponentId::PLAYER, ComponentId::MOVEMENT});
	shovingSystem = ecs.rosterSystem<ShovingSystem>(SystemId::SHOVING,
		{ComponentId::POSITION, ComponentId::COLLIDER, ComponentId::FORCE});
	processingSystem = ecs.rosterSystem<ProcessingSystem>(SystemId::PROCESSING,
		{ComponentId::PROCESSING, ComponentId::SPRITE});
	fuelSystem = ecs.rosterSystem<FuelSystem>(SystemId::FUEL,
		{ComponentId::FUEL});
	spawnerSystem = ecs.rosterSystem<SpawnerSystem>(SystemId::SPAWNER, 
		{ComponentId::SPAWNER, ComponentId::POSITION});
	aiLureSystem = ecs.rosterSystem<AiLureSystem>(SystemId::AI_LURE, 
		{ComponentId::AI, ComponentId::AI_LURE});

	LOG("Systems rostered")
}

void World::update(uint dt) {

	// inspect = player;

	ticks += tickSpeed * dt;
	time.update(tickSpeed * dt);

	if (player && ecs.hasComponent<DeathComponent>(player)) {
		player = 0;
		guiManager.add(makeDeathScreen());
		LOG("Death");
	}

	if (player) {
		PositionComponent& positionComponent = ecs.getComponent<PositionComponent>(player);
		playerRealm = realmManager.getRealm(positionComponent.realmId);
		playerPosition = positionComponent.position;

		playerChunk = playerRealm->chunkManager.getChunk(playerPosition);

		pair c = playerRealm->chunkManager.getChunk(playerPosition);

		// ChunkManager& cm = playerRealm->chunkManager;
		// std::thread thread(&ChunkManager::generateChunk, cm, c, ChunkStage::LOADED, playerRealm->environment.get());
		playerRealm->chunkManager.generateChunk(c, ChunkStage::LOADED, playerRealm->environment.get());
	}

	playerRealm->minimap.update(playerPosition, playerRealm);

	guiManager.update(dt);
	controllerSystem->update(inputState, camera, state, ticks);
	playerSystem->update(camera);

	updateSet.clear();
	const uchar updateDistance = 1;
	for (int x = -updateDistance; x <= updateDistance; x++) {
		for (int y = -updateDistance; y <= updateDistance; y++) {
			pair chunk = playerChunk + pair(x, y);

			auto it =  playerRealm->chunkManager.chunks.find(chunk);
			if (it != playerRealm->chunkManager.chunks.end()) {
				EntitySet& set = (*it).second.entities;
				updateSet.insert(set.begin(), set.end());
			}
		}
	}

	spawnerSystem->update(ticks, updateSet, particleSystem);

	sensorSystem->update(updateSet, ticks, realmManager);

	//aiMoveSystem->update(ticks, realmManager);
	aiWanderSystem->score(ticks);
	aiFleeSystem->score(ticks);
	aiChaseSystem->score(ticks);
	aiMeleeSystem->score(ticks);
	aiPostSystem->score(ticks);
	aiLureSystem->score(ticks);

	aiSystem->update(ticks);
	//aiMoveSystem->move(ticks);
	aiWanderSystem->update(ticks, realmManager);
	aiFleeSystem->update(ticks, realmManager);
	aiChaseSystem->update(ticks, realmManager);
	aiMeleeSystem->update(ticks, realmManager);
	aiPostSystem->update(ticks, realmManager);
	aiLureSystem->update(ticks, realmManager);

	creatureActionSystem->update(ticks, forageSystem, updateSet);

	projectileSystem->update(ticks, dt);
	explosiveSystem->update(ticks, camera);

	effectSystem->update(ticks);
	itemModSystem->update();

	creatureMovementSystem->update(dt, realmManager);

	playerRealm->chunkManager.reballance(this);

	EntityMap collisions;
	collisionSystem->update(collisions, updateSet);
	shovingSystem->update(collisions);

	EntityMap hits;
	hitboxSystem->update(hits, updateSet);

	damageSystem->update(hits, ticks);

	fuelSystem->update(ticks, dt);
	processingSystem->update(ticks, dt);

	// chunkSystem->update(realmManager);

	itemPickupSystem->update(collisions, realmManager);

	updateCamera(player);
	camera.update(ticks);
	healthSystem->update(ticks, updateSet);
	maturitySystem->update(ticks, updateSet);
	lootSystem->update(ticks);

	creatureAnimationSystem->update(ticks);

	inventoryDeathSystem->update(ticks, realmManager);
	deathSystem->update(realmManager, particleSystem);

	creatureParticleSystem->update(ticks);
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
	playerRealm->chunkManager.draw(camera, ticks);
	// drawTiles()

	if (state) {
		vec gridPosition = camera.screenPosition(vec::round(camera.worldPosition(Window::instance->mousePosition)));
		TextureManager::drawRect(gridPosition, pair(camera.zoom * BIT, camera.zoom * BIT), {0, 0, 255, 255});
	}

	EntitySet drawSet;
	
	Realm* cameraRealm = realmManager.getRealm(camera.realmId);
	pair cameraChunk = cameraRealm->chunkManager.getChunk(camera.position);
	const uchar renderDistance = 1;
	for (int x = -renderDistance; x <= renderDistance; x++) {
		for (int y = -renderDistance; y <= renderDistance; y++) {
			pair chunk = cameraChunk + pair(x, y);
			auto it =  playerRealm->chunkManager.chunks.find(chunk);
			if (it != playerRealm->chunkManager.chunks.end()) {
				EntitySet& set = (*it).second.entities;
				drawSet.insert(set.begin(), set.end());
			}
		}
	}

	// LOG(drawSet.size());

	entityDrawSystem->update(camera, drawQueue, state, player, ticks, drawSet);
	handRenderSystem->update(camera, drawQueue, ticks);

	auto lambda = [](auto& l, auto& r) {
		float ly = l.position.y + l.z;
		float ry = r.position.y + r.z;
		return ly < ry || (ly == ry && l.position.x < r.position.x);
	};

	std::sort(drawQueue.begin(), drawQueue.end(), lambda);

	hover = 0;
	pair mpos = vec::round(Window::instance->mousePosition);
	for (auto& p : drawQueue) {
		if (p.spriteStack.ison(mpos, p.position, p.scale, p.style, ticks) && p.entity) hover = p.entity;
		p.spriteStack.draw(p.position, p.scale, p.style, ticks);
	}

	if (Game::settings.showCollider) colliderDrawSystem->update(camera, ticks, drawSet);
	if (Game::settings.showHitbox) hitboxDrawSystem->update(camera, ticks, drawSet);

	particleSystem.draw(camera);
	lightSystem->update(camera, time, ticks, drawSet);
	playerRealm->environment->draw(camera, ticks);

	guiManager.draw();

	if (player) {
		vec playerPos = ecs.getComponent<PositionComponent>(player).position;
		playerRealm->minimap.draw();
	}
	state = false;
}

std::unique_ptr<GuiElement> World::makeInventory(InventorySlice link) {
	if (!player) return nullptr;
	InventoryComponent& inventoryComponent = ecs.getComponent<InventoryComponent>(player);
	PlayerComponent& playerComponent = ecs.getComponent<PlayerComponent>(player);
	Sprite sprite = Sprite(SpriteSheet::INVENTORY, {0, 0}, {10, 10});
	std::unique_ptr<Widget> gui = std::make_unique<Widget>(pair(0, 0), pair(150, 150), sprite);

	InventorySlice hotbar = InventorySlice(&inventoryComponent.inventory, pair(0, 1));
	InventorySlice main = InventorySlice(&inventoryComponent.inventory, pair(1, 7));
	gui->addGuiElement(std::make_unique<InventoryGui>(pair(0, -60), hotbar, 20, link.inventory ? link : main));
	gui->addGuiElement(std::make_unique<InventoryGui>(pair(0, 20), main, 20, link.inventory ? link : hotbar));
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
	Entity player = EntityFactory::createPlayer(spawnRealm->realmId, spawn);
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
	if (playerRealm->minimap.handleEvent(event)) return true;
	if (!player) return false;
	
	PlayerComponent& playerComponent = ecs.getComponent<PlayerComponent>(player);
	InventoryComponent& inventoryComponent = ecs.getComponent<InventoryComponent>(player);
	ItemContainer& activeItemContainer = inventoryComponent.inventory.itemContainers[playerComponent.activeSlot][0];

	if (event.id == InputEventId::INVENTORY) {
		guiManager.open(makeInventory(), makeMenu());
	} else if (event.id == InputEventId::THROW) {
		if (!activeItemContainer.item) return true;
		PositionComponent& positionComponent = ecs.getComponent<PositionComponent>(player);
		FacingComponent& facingComponent = ecs.getComponent<FacingComponent>(player);
		vec position = positionComponent.position + facingComponent.facing;
		ecs.addComponent<PositionComponent>({position, positionComponent.realmId}, activeItemContainer.item);
		pair chunk = vec::round(position / CHUNK_SIZE);
		playerRealm->attach(activeItemContainer.item, chunk);
		activeItemContainer.clear();
	} else if (12 <= event.id && event.id <= 18) {
		playerComponent.activeSlot = event.id - 12;
	} else if (event.id == InputEventId::ROTATE_LEFT) {
		playerComponent.activeSlot = (playerComponent.activeSlot + 6) % 7;
	} else if (event.id == InputEventId::ROTATE_RIGHT) {
		playerComponent.activeSlot = (playerComponent.activeSlot + 1) % 7;
	} else if (event.id == InputEventId::PRIMARY) {
		vec position = camera.worldPosition(event.mousePosition);
		ActionComponent& actionComponent = ecs.getComponent<ActionComponent>(player);

		if (activeItemContainer.item) {
			if (ecs.hasComponent<LauncherComponent>(activeItemContainer.item)) {
				LauncherComponent& launcherComponent = ecs.getComponent<LauncherComponent>(activeItemContainer.item);
				vec position = camera.worldPosition(event.mousePosition);
				vec playerPosition = ecs.getComponent<PositionComponent>(player).position;
				vec direction = vec::normalise(position - playerPosition);
				EntityFactory::createProjectile(ProjectileId::FIRE_BALL, playerRealm->realmId, playerPosition, launcherComponent.force * direction, player);
				return true;
			}
		}

		if (actionComponent.actionState == ActionState::IDLE) {
			actionComponent.actionState = ActionState::ATTACK;
			actionComponent.position = position;
			actionComponent.start = ticks;
			actionComponent.trigger = ticks + 150;
			actionComponent.end = ticks + 300;
			actionComponent.item = activeItemContainer.item;
		}

	} else if (event.id == InputEventId::SECONDARY) {
		vec position = camera.worldPosition(event.mousePosition);
		pair gridPos = vec::round(position);

		Entity gridEntity = playerRealm->chunkManager.gridEntity(gridPos);
		GroundId::value groundId = playerRealm->chunkManager.getGround(gridPos);

		if (gridEntity && ecs.hasComponent<PortalComponent>(gridEntity)) {
			PortalComponent& portalComponent = ecs.getComponent<PortalComponent>(gridEntity);
			PositionComponent& positionComponent = ecs.getComponent<PositionComponent>(player);
			playerRealm->detach(player);
			positionComponent.position = portalComponent.position;
			positionComponent.realmId = portalComponent.realmId;
			Realm* realm = realmManager.getRealm(portalComponent.realmId);
			pair chunk = realm->chunkManager.getChunk(positionComponent.position);
			realm->attach(player, chunk);
			return true;
		}
		
		if (activeItemContainer.item) {
			ItemComponent& itemComponent = ecs.getComponent<ItemComponent>(activeItemContainer.item);
			if (!ecs.hasComponent<ItemKindComponent>(activeItemContainer.item)) return false;
			ItemKindComponent& itemKindComponent = ecs.getComponent<ItemKindComponent>(activeItemContainer.item);

			if (hasItemKind(activeItemContainer.item, ItemKind::HOE)) {
				if (groundId == GroundId::DIRT || groundId == GroundId::GRASS && gridEntity) {
					playerRealm->chunkManager.setGround(gridPos, GroundId::SOIL);
					return true;
				}
			}  

			if (itemComponent.itemId == ItemId::PARSNIP_SEEDS && groundId == GroundId::SOIL && !gridEntity) {
				EntityFactory::createCrop(CropId::PARSNIP, playerRealm->realmId, gridPos);
				itemComponent.count -= 1;
				if (itemComponent.count == 0) {
					ecs.addComponent<DeathComponent>({}, activeItemContainer.item);
					activeItemContainer.item = 0;
				}
				return true;
			}

			if (itemKindComponent.itemKinds[ItemKind::TANK] && groundId == GroundId::WATER) {
				TankComponent& tankComponent = ecs.getComponent<TankComponent>(activeItemContainer.item);
				SpriteComponent& spriteComponent = ecs.getComponent<SpriteComponent>(activeItemContainer.item);

				tankComponent.content = tankComponent.capacity;
				tankComponent.liquid = Liquid::WATER;

				spriteComponent.spriteStack = tankComponent.fullSprite;
			}
		}
		
		InventorySlice link;
		std::unique_ptr<GuiElement> gui = interactionSystem->update(position, updateSet, ticks, &inventoryComponent.inventory, &link);
		if (gui) guiManager.open(makeInventory(link), std::move(gui));
	} else if (event.id == InputEventId::STATE) {
		state = true;

		ActionComponent& actionComponent = ecs.getComponent<ActionComponent>(player);

		if (actionComponent.actionState == ActionState::IDLE && inputState[InputStateId::SECONDARY]) {
			Entity item = activeItemContainer.item;
			if (ecs.hasComponent<WindUpComponent>(item)) {
				actionComponent.actionState = ActionState::WIND_UP;
				actionComponent.start = ticks;
				actionComponent.trigger = 0;
				actionComponent.end = 0;
				actionComponent.item = item;
			}
		}

		if (actionComponent.actionState == ActionState::WIND_UP && !inputState[InputStateId::SECONDARY]) {
			actionComponent.position = camera.worldPosition(event.mousePosition);
			actionComponent.trigger = ticks;
		}
	

		// if (ecs.hasComponent<LauncherComponent>(activeItemContainer.item)) {
		// 	LauncherComponent& launcherComponent = ecs.getComponent<LauncherComponent>(activeItemContainer.item);
		// 	if (inputState[InputStateId::SECONDARY]) {
		// 		launcherComponent.charge +=  dt * launcherComponent.maxForce / launcherComponent.chargeTime;
		// 		launcherComponent.charge = std::min(launcherComponent.charge, launcherComponent.maxForce);
		// 	} else {
		// 		if (launcherComponent.charge > launcherComponent.minForce) {
		// 			vec position = camera.worldPosition(event.mousePosition);
		// 			vec playerPosition = ecs.getComponent<PositionComponent>(player).position;
		// 			vec direction = vec::normalise(position - playerPosition);
		// 			EntityFactory::createProjectile(playerRealm->realmId, playerPosition, launcherComponent.charge * direction);
		// 		}
		// 		launcherComponent.charge = 0;
		// 	}
		// }
	} else if (event.id == InputEventId::INSPECT) {
		if (hover) {
			inspect = hover;
			return true;
		}

		// pair mPos = Window::instance->mousePosition;
		// vec pos = camera.worldPosition(mPos);
		// pair tile = vec::round(pos);

		// if (playerRealm->gridMap.find(tile) != playerRealm->gridMap.end()) {
		// 	inspect = playerRealm->gridMap[tile];
		// }
	}

	return false;
}

void World::serialise(std::fstream& stream) {
	serialise_object(stream, seed);
	serialise_object(stream, ticks);
	serialise_object(stream, player);
	realmManager.serialise(stream);
	// ecs.serialise(stream);
}

void World::serialise2(std::filesystem::path path) {
	std::filesystem::create_directory(path / name);
	std::fstream file = std::fstream(path / name / "world.binary", std::ios::out | std::ios::binary);
	if (!file) ERROR("Failed to create save files for world", name);

	serialise_object(file, seed);
	serialise_object(file, ticks);
	serialise_object(file, player);

	realmManager.serialise2(path / name);
	file.close();
	
}