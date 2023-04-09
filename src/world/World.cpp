
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
	BiomeTemplate::setTemplates();

	guiManager.world = this;
	EntityFactory::world = this;
}

World::World(std::string name)
	: name(name), ticks(0) {
	init();
	realm = std::make_unique<Realm>(pair(100, 100), 1729);

	Entity player = EntityFactory::createPlayer({8, 8});

	Entity item = EntityFactory::createItem(ItemId::APPLE, 20);
	Entity rest = ecs.getComponent<InventoryComponent>(player).inventory.add(item);

	guiManager.add(std::make_unique<HotbarGui>(player));
	guiManager.add(std::make_unique<HealthBarGui>(player));

	EntityFactory::createCamera({0, 0}, 4);

	EntityFactory::createAnimal(AnimalId::COW, {6, 6});

	Entity axe = ecs.createEntity();
	SpriteStack axeSprites;
	axeSprites.addSprite(Sprite(SpriteSheet::ITEMS, {2, 0}, {1, 1}));
	ecs.addComponent<SpriteComponent>({axeSprites, 0.5f, {ShaderId::BOUNCE, ticks}}, axe);
	ecs.addComponent<ItemComponent>({ItemId::NONE, 1}, axe);
	ecs.addComponent<ToolComponent>({ToolId::AXE}, axe);
	Collider axeCollider({0, 0}, {0.4f, 0.4f});
	ecs.addComponent<ColliderComponent>({axeCollider}, axe);
	ecs.addComponent<PositionComponent>({{6, 5}}, axe);
	ecs.addComponent<NameComponent>({Textblock("Axe")}, axe);

	Entity pick = ecs.createEntity();
	SpriteStack pickSprites;
	pickSprites.addSprite(Sprite(SpriteSheet::ITEMS, {1, 0}, {1, 1}));
	ecs.addComponent<SpriteComponent>({pickSprites, 0.5f, {ShaderId::BOUNCE, ticks}}, pick);
	ecs.addComponent<ItemComponent>({ItemId::NONE, 1}, pick);
	ecs.addComponent<ToolComponent>({ToolId::PICK_AXE}, pick);
	Collider pickCollider({0, 0}, {0.4f, 0.4f});
	ecs.addComponent<ColliderComponent>({pickCollider}, pick);
	ecs.addComponent<PositionComponent>({{7, 5}}, pick);
	ecs.addComponent<NameComponent>({Textblock("Pick Axe")}, pick);

	Entity sword = ecs.createEntity();
	SpriteStack swordSprites;
	swordSprites.addSprite(Sprite(SpriteSheet::ITEMS, {0, 0}, {1, 1}));
	ecs.addComponent<SpriteComponent>({swordSprites, 0.5f, {ShaderId::BOUNCE, ticks}}, sword);
	ecs.addComponent<ItemComponent>({ItemId::NONE, 1}, sword);
	ecs.addComponent<DamageComponent>({1}, sword);
	Collider swordCollider({0, 0}, {0.4f, 0.4f});
	ecs.addComponent<ColliderComponent>({swordCollider}, sword);
	ecs.addComponent<PositionComponent>({{8, 5}}, sword);
	ecs.addComponent<NameComponent>({Textblock("Sword")}, sword);

	EntityFactory::createStation(StationId::CHEST, {10, 9});

	Entity monster = ecs.createEntity();
	ecs.addComponent<PositionComponent>({{5, 8}}, monster);
	ecs.addComponent<CreatureStateComponent>({CreatureState::IDLE, Direction::EAST}, monster);
	ecs.addComponent<DirectionComponent>({Direction::EAST}, monster);
	ecs.addComponent<MovementComponent>({1}, monster);
	SpriteStack monsterSprites;
	monsterSprites.addSprite({SpriteSheet::MONSTER, {0, 0}, {1, 2}, 1, 100}, pair(0, -1));
	ecs.addComponent<SpriteComponent>({monsterSprites}, monster);
	Collider collider({0, 0}, {0.6f, 0.6f});
	ecs.addComponent<ColliderComponent>({collider}, monster);
	ecs.addComponent<HealthComponent>({20, 20}, monster);
	ecs.addComponent<MonsterAiComponent>({}, monster);
	ecs.addComponent<ParticleComponent>({ParticleSystem::DIRT}, monster);

	Entity tree = ecs.createEntity();
	ecs.addComponent<PositionComponent>({pair(8, 3)}, tree);
	ecs.addComponent<GridComponent>({pair(8, 3), pair(1, 1), true}, tree);
	SpriteStack treeSprites;
	treeSprites.addSprite({SpriteSheet::RESOURCES, pair(5, 7), pair(1, 2)}, pair(0, -1));
	treeSprites.addSprite({SpriteSheet::RESOURCES, pair(6, 6), pair(3, 2)}, pair(-1, -2));
	ecs.addComponent<SpriteComponent>({treeSprites}, tree);
	ecs.addComponent<ResourceComponent>({ToolId::AXE}, tree);
	ecs.addComponent<HealthComponent>({5, 5}, tree);

	Entity tree2 = ecs.createEntity();
	ecs.addComponent<PositionComponent>({pair(9, 3)}, tree2);
	ecs.addComponent<GridComponent>({pair(9, 3), pair(1, 1), true}, tree2);
	SpriteStack treeSprites2;
	treeSprites2.addSprite({SpriteSheet::RESOURCES, pair(5, 7), pair(1, 2)}, pair(0, -1));
	treeSprites2.addSprite({SpriteSheet::RESOURCES, pair(6, 6), pair(3, 2)}, pair(-1, -2));
	ecs.addComponent<SpriteComponent>({treeSprites2}, tree2);
	ecs.addComponent<ResourceComponent>({ToolId::AXE}, tree2);
	ecs.addComponent<HealthComponent>({5, 5}, tree2);

	Entity fire = ecs.createEntity();
	ecs.addComponent<PositionComponent>({pair(11, 3)}, fire);
	ecs.addComponent<GridComponent>({pair(11, 3), pair(1, 1), true}, fire);
	SpriteStack fireSprites;
	fireSprites.addSprite({SpriteSheet::FIRE, pair(0, 0), pair(1, 1), 4, 200});
	ecs.addComponent<SpriteComponent>({fireSprites}, fire);
	ecs.addComponent<ParticleComponent>({ParticleSystem::SMOKE}, fire);

	Entity circle = ecs.createEntity();
	ecs.addComponent<PositionComponent>({{10, 10}}, circle);
	Collider circleCollider({0, 0}, 0.5f);
	ecs.addComponent<ColliderComponent>({circleCollider}, circle);

	realm->generate();

	gridSystem->rebuild(realm->gridMap, realm->solidMap);
}

World::World(std::fstream& stream) {
	deserialise_object(stream, ticks);

	init();
	ecs.deserialise(stream);

	realm = std::make_unique<Realm>(pair(100, 100), 1729);
	player = playerSystem->getPlayer();

	guiManager.add(std::make_unique<HotbarGui>(player));
	guiManager.add(std::make_unique<HealthBarGui>(player));

	gridSystem->rebuild(realm->gridMap, realm->solidMap);
}

void World::rosterComponents() {
	ecs.rosterComponent<PositionComponent>(ComponentId::POSITION);
	ecs.rosterComponent<SpriteComponent>(ComponentId::SPRITE);
	ecs.rosterComponent<CameraComponent>(ComponentId::CAMERA);
	ecs.rosterComponent<CreatureStateComponent>(ComponentId::CREATURE_STATE);
	ecs.rosterComponent<ControllerComponent>(ComponentId::CONTROLLER);
	ecs.rosterComponent<DirectionComponent>(ComponentId::DIRECTION);
	ecs.rosterComponent<MovementComponent>(ComponentId::MOVEMENT);
	ecs.rosterComponent<ColliderComponent>(ComponentId::COLLIDER);
	ecs.rosterComponent<ItemComponent>(ComponentId::ITEM);
	ecs.rosterComponent<AnimalAiComponent>(ComponentId::ANIMAL_AI);
	ecs.rosterComponent<InventoryComponent>(ComponentId::INVENTORY);
	ecs.rosterComponent<HealthComponent>(ComponentId::HEALTH);
	ecs.rosterComponent<PlayerComponent>(ComponentId::PLAYER);
	ecs.rosterComponent<ResourceComponent>(ComponentId::RESOURCE);
	ecs.rosterComponent<LootComponent>(ComponentId::LOOT);
	ecs.rosterComponent<ToolComponent>(ComponentId::TOOL);
	ecs.rosterComponent<DamageComponent>(ComponentId::DAMAGE);
	ecs.rosterComponent<ForceComponent>(ComponentId::FORCE);
	ecs.rosterComponent<GridComponent>(ComponentId::GRID);
	ecs.rosterComponent<StationComponent>(ComponentId::INTERACTION);
	ecs.rosterComponent<NameComponent>(ComponentId::NAME);
	ecs.rosterComponent<MonsterAiComponent>(ComponentId::MONSTER_AI);
	ecs.rosterComponent<GatherComponent>(ComponentId::GATHER);
	ecs.rosterComponent<DeathComponent>(ComponentId::DEATH);
	ecs.rosterComponent<ParticleComponent>(ComponentId::PARTICLE);

	LOG("Components rostered");
}

void World::rosterSystems() {
	entityDrawSystem = ecs.rosterSystem<EntityDrawSystem>(SystemId::ENTITY_DRAW,
		{ComponentId::SPRITE, ComponentId::POSITION});
	creatureMovementSystem = ecs.rosterSystem<CreatureMovementSystem>(SystemId::CREATURE_MOVEMENT,
		{ComponentId::MOVEMENT, ComponentId::CREATURE_STATE, ComponentId::POSITION, ComponentId::COLLIDER});
	controllerSystem = ecs.rosterSystem<ControllerSystem>(SystemId::CONTROLLER,
		{ComponentId::CONTROLLER, ComponentId::CREATURE_STATE, ComponentId::DIRECTION});
	cameraSystem = ecs.rosterSystem<CameraSystem>(SystemId::CAMERA,
		{ComponentId::CAMERA, ComponentId::POSITION});
	creatureAnimationSystem = ecs.rosterSystem<CreatureAnimationSystem>(SystemId::CREATURE_ANIMATION,
		{ComponentId::CREATURE_STATE, ComponentId::SPRITE, ComponentId::DIRECTION});
	collisionSystem = ecs.rosterSystem<CollisionSystem>(SystemId::COLLISION,
		{ComponentId::COLLIDER, ComponentId::POSITION});
	itemPickupSystem = ecs.rosterSystem<ItemPickupSystem>(SystemId::ITEM_PICKUP,
		{ComponentId::PLAYER, ComponentId::INVENTORY});
	tileDrawSystem = ecs.rosterSystem<TileDrawSystem>(SystemId::TILE,
		{ComponentId::CAMERA, ComponentId::POSITION});
	animalAiSystem = ecs.rosterSystem<AnimalAiSystem>(SystemId::ANIMAL_AI,
		{ComponentId::CREATURE_STATE, ComponentId::ANIMAL_AI, ComponentId::DIRECTION});
	forageSystem = ecs.rosterSystem<ForageSystem>(SystemId::FORAGE,
		{ComponentId::RESOURCE, ComponentId::POSITION, ComponentId::HEALTH});
	healthSystem = ecs.rosterSystem<HealthSystem>(SystemId::HEALTH,
		{ComponentId::HEALTH});
	lootSystem = ecs.rosterSystem<LootSystem>(SystemId::LOOT,
		{ComponentId::LOOT, ComponentId::DEATH, ComponentId::POSITION});
	damageSystem = ecs.rosterSystem<DamageSystem>(SystemId::DAMAGE,
		{ComponentId::POSITION, ComponentId::COLLIDER, ComponentId::HEALTH});
	playerSystem = ecs.rosterSystem<PlayerSystem>(SystemId::PLAYER,
		{ComponentId::PLAYER});
	colliderDrawSystem = ecs.rosterSystem<ColliderDrawSystem>(SystemId::COLLIDER_DRAW,
		{ComponentId::COLLIDER, ComponentId::POSITION});
	gridSystem = ecs.rosterSystem<GridSystem>(SystemId::GRID,
		{ComponentId::GRID});
	interactionSystem = ecs.rosterSystem<InteractionSystem>(SystemId::INTERACTION,
		{ComponentId::INTERACTION});
	monsterAiSystem = ecs.rosterSystem<MonsterAiSystem>(SystemId::MONSTER_AI,
		{ComponentId::CREATURE_STATE, ComponentId::MONSTER_AI, ComponentId::DIRECTION});
	gatherSystem = ecs.rosterSystem<GatherSystem>(SystemId::GATHER,
		{ComponentId::GATHER, ComponentId::POSITION, ComponentId::LOOT});
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
	gridDeathSystem = ecs.rosterSystem<GridDeathSystem>(SystemId::GRID_DEATH,
		{ComponentId::GRID, ComponentId::DEATH});
	chunkSystem = ecs.rosterSystem<ChunkSystem>(SystemId::CHUNK,
		{ComponentId::POSITION, ComponentId::MOVEMENT});

	LOG("Systems rostered")
}

void World::update(uint dt) {
	ticks += dt;
	player = playerSystem->getPlayer();
	camera = cameraSystem->getCamera();

	std::unordered_map<Entity, std::vector<Entity>> collisions;

	guiManager.update();

	controllerSystem->update(inputState, !guiManager.active(), ticks);

	animalAiSystem->update(ticks);
	monsterAiSystem->update(player, realm->solidMap, ticks);

	creatureMovementSystem->update(dt, realm->solidMap, realm->map.get());
	collisionSystem->update(collisions);

	chunkSystem->update(chunks);

	itemPickupSystem->update(collisions);

	cameraSystem->update(player);
	healthSystem->update(); //TODO SLOW

	lootSystem->update(ticks);

	creatureAnimationSystem->update(ticks);

	gridDeathSystem->update(realm->gridMap, realm->solidMap);
	inventoryDeathSystem->update(ticks);
	deathSystem->update();

	creatureParticleSystem->update();
	particleEmitSystem->update(particleSystem, ticks);
	particleSystem.update(dt);
}

void World::draw() {
	
	tileDrawSystem->update(realm->map.get(), ticks);

	if (camera) {
		vec cameraPosition = ecs.getComponent<PositionComponent>(camera).position;
		float cameraZoom = ecs.getComponent<CameraComponent>(camera).zoom;
		pair position = round(cameraPosition + vec(Window::instance->mousePosition - Window::instance->size / 2) / (cameraZoom * BIT));
		pair screenPosition = round(BIT * cameraZoom * (position - cameraPosition)) + (Window::instance->size) / 2;

		TextureManager::drawRect(screenPosition, {cameraZoom * BIT, cameraZoom * BIT}, {0, 0, 255, 255});

		entityDrawSystem->update(camera, ticks, chunks[pair(0,0)]);  //TODO SLOW
		// handRenderSystem->update(camera, ticks);
		colliderDrawSystem->update(camera, ticks);
		particleSystem.draw(cameraPosition, cameraZoom);
	}

	guiManager.draw();
}

void World::link(Entity entity) {
	if (ecs.hasComponent<GridComponent>(entity)) gridSystem->link(realm->gridMap, realm->solidMap, entity);
	if (ecs.hasComponent<PositionComponent>(entity)) chunkSystem->assign(entity, chunks);
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
	tab3->addGuiElement(std::make_unique<BuildGui>(pair(0, 10)));

	gui->addTab(std::move(tab1));
	gui->addTab(std::move(tab2));
	gui->addTab(std::move(tab3));
	return gui;
}

bool World::handleEvent(InputEvent event) {
	if (!player) return false;
	if (guiManager.handleEvent(event)) return true;
	PlayerComponent& playerComponent = ecs.getComponent<PlayerComponent>(player);
	CreatureStateComponent& creatureStateComponent = ecs.getComponent<CreatureStateComponent>(player);
	PositionComponent& positionComponent = ecs.getComponent<PositionComponent>(player);
	ItemContainer& activeItemContainer = playerComponent.hotbar.itemContainers[playerComponent.activeSlot][0];

	
	if (event.id == InputEventId::INVENTORY) {
		guiManager.open(makeInventory(), makeMenu());
	} else if (event.id == InputEventId::THROW) {
		vec position = positionComponent.position + unitVectors[creatureStateComponent.facing];
		ecs.addComponent<PositionComponent>({position}, activeItemContainer.item);
		activeItemContainer.clear();
	}

	if (event.id == InputEventId::SELECT_1) playerComponent.activeSlot = 0;
	if (event.id == InputEventId::SELECT_2) playerComponent.activeSlot = 1;
	if (event.id == InputEventId::SELECT_3) playerComponent.activeSlot = 2;
	if (event.id == InputEventId::SELECT_4) playerComponent.activeSlot = 3;
	if (event.id == InputEventId::SELECT_5) playerComponent.activeSlot = 4;
	if (event.id == InputEventId::SELECT_6) playerComponent.activeSlot = 5;
	if (event.id == InputEventId::SELECT_7) playerComponent.activeSlot = 6;

	if (!camera) return false;
	vec cameraPosition = ecs.getComponent<PositionComponent>(camera).position;
	uint zoom = ecs.getComponent<CameraComponent>(camera).zoom;
	vec position = cameraPosition + vec(event.mousePosition - Window::instance->size / 2) / (zoom * BIT);

	if (event.id == InputEventId::PRIMARY) {
		forageSystem->update(position, activeItemContainer.item, ticks);
		gatherSystem->update(player, position, ticks);
		damageSystem->update(player, position, activeItemContainer.item);
	} else if (event.id == InputEventId::SECONDARY) {
		std::unique_ptr<GuiElement> gui = interactionSystem->update(position);
		if (gui) guiManager.open(makeInventory(), std::move(gui));
	}

	return false;
}

void World::serialise(std::fstream& stream) {
	serialise_object(stream, ticks);
	ecs.serialise(stream);
}