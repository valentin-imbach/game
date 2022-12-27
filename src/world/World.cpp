
#include "World.hpp"
#include "Components.hpp"
#include "ECS_types.hpp"
#include "Events.hpp"
#include "GuiElement.hpp"
#include "Item.hpp"
#include "Sprite.hpp"
#include "Tile.hpp"
#include "Window.hpp"
#include "EntityFactory.hpp"
#include "utils.hpp"

#include "ItemTemplates.hpp"

World::World(std::string name) : name(name) {
	rosterComponents();
	rosterSystems();

	ItemPropertyTemplate::setTemplates();
	ItemKindTemplate::setTemplates();
	ItemTemplate::setTemplates();

	guiManager.ecs = &ecs;
	guiManager.world = this;
	EntityFactory::ecs = &ecs;
	EntityFactory::gridMap = &gridMap;

	player = EntityFactory::createPlayer({8, 8});

	Entity item = EntityFactory::createItem(ItemId::APPLE, 20);
	Entity rest = ecs.getComponent<InventoryComponent>(player).inventory.add(item);

	guiManager.add(std::make_unique<HotbarGui>(player));
	guiManager.add(std::make_unique<HealthBarGui>(player));

	camera = EntityFactory::createCamera({0, 0}, 4);

	EntityFactory::createAnimal(AnimalId::COW, {6, 6});

	Entity axe = ecs.createEntity();
	SpriteStack axeSprites;
	axeSprites.addSprite(Sprite(SpriteSheet::ITEMS, {2, 0}, {1, 1}));
	ecs.addComponent<SpriteComponent>({axeSprites, 0, 0.5f}, axe);
	ecs.addComponent<ItemComponent>({ItemId::NONE, 1}, axe);
	ecs.addComponent<ToolComponent>({ToolId::AXE}, axe);
	Collider axeCollider = {{0, 0}, {0.4f, 0.4f}};
	ecs.addComponent<ColliderComponent>({axeCollider}, axe);
	ecs.addComponent<PositionComponent>({{7, 5}}, axe);

	Entity pick = ecs.createEntity();
	SpriteStack pickSprites;
	pickSprites.addSprite(Sprite(SpriteSheet::ITEMS, {1, 0}, {1, 1}));
	ecs.addComponent<SpriteComponent>({pickSprites, 0, 0.5f}, pick);
	ecs.addComponent<ItemComponent>({ItemId::NONE, 1}, pick);
	ecs.addComponent<ToolComponent>({ToolId::PICK_AXE}, pick);
	Collider pickCollider = {{0, 0}, {0.4f, 0.4f}};
	ecs.addComponent<ColliderComponent>({pickCollider}, pick);
	ecs.addComponent<PositionComponent>({{8, 5}}, pick);

	Entity sword = ecs.createEntity();
	SpriteStack swordSprites;
	swordSprites.addSprite(Sprite(SpriteSheet::ITEMS, {0, 0}, {1, 1}));
	ecs.addComponent<SpriteComponent>({swordSprites, 0, 0.5f}, sword);
	ecs.addComponent<ItemComponent>({ItemId::NONE, 1}, sword);
	ecs.addComponent<DamageComponent>({1}, sword);
	Collider swordCollider = {{0, 0}, {0.4f, 0.4f}};
	ecs.addComponent<ColliderComponent>({swordCollider}, sword);
	ecs.addComponent<PositionComponent>({{9, 5}}, sword);

	generate();
}

void World::generate() {
	uint seed = 456;
	for (int x = 0; x < MAP_WIDTH; x++) {
		for (int y = 0; y < MAP_HEIGHT; y++) {
			pair position = {x, y};
			TileId::value tileId = map.getTileId(position);
			if (tileId == TileId::WATER) continue;
			if (tileId == TileId::GRASS) {
				if (bernoulli(seed++, 0.1f)) {
					EntityFactory::createResource(ResourceId::TREE, position);
					continue;
				}
			}
			if (bernoulli(seed++, 0.05f)) {
				EntityFactory::createResource(ResourceId::ROCK, position);
			}
		}
	}
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
}

void World::rosterSystems() {
	entityDrawSystem = ecs.rosterSystem<EntityDrawSystem>(SystemId::ENTITY_DRAW, {ComponentId::SPRITE, ComponentId::POSITION});
	creatureMovementSystem = ecs.rosterSystem<CreatureMovementSystem>(SystemId::CREATURE_MOVEMENT, {ComponentId::MOVEMENT, ComponentId::CREATURE_STATE, ComponentId::POSITION, ComponentId::COLLIDER});
	controllerSystem = ecs.rosterSystem<ControllerSystem>(SystemId::CONTROLLER, {ComponentId::CONTROLLER, ComponentId::CREATURE_STATE, ComponentId::DIRECTION});
	cameraSystem = ecs.rosterSystem<CameraSystem>(SystemId::CAMERA, {ComponentId::CAMERA, ComponentId::POSITION});
	creatureAnimationSystem = ecs.rosterSystem<CreatureAnimationSystem>(SystemId::CREATURE_ANIMATION, {ComponentId::CREATURE_STATE, ComponentId::SPRITE, ComponentId::DIRECTION});
	collisionSystem = ecs.rosterSystem<CollisionSystem>(SystemId::COLLISION, {ComponentId::COLLIDER, ComponentId::POSITION});
	itemPickupSystem = ecs.rosterSystem<ItemPickupSystem>(SystemId::ITEM_PICKUP, {ComponentId::COLLIDER, ComponentId::INVENTORY});
	tileDrawSystem = ecs.rosterSystem<TileDrawSystem>(SystemId::TILE, {ComponentId::CAMERA, ComponentId::POSITION});
	animalAiSystem = ecs.rosterSystem<AnimalAiSystem>(SystemId::ANIMAL_AI, {ComponentId::CREATURE_STATE, ComponentId::ANIMAL_AI, ComponentId::DIRECTION});
	forageSystem = ecs.rosterSystem<ForageSystem>(SystemId::FORAGE, {ComponentId::RESOURCE, ComponentId::POSITION, ComponentId::HEALTH});
	healthSystem = ecs.rosterSystem<HealthSystem>(SystemId::HEALTH, {ComponentId::HEALTH});
	lootSystem = ecs.rosterSystem<LootSystem>(SystemId::LOOT, {ComponentId::LOOT, ComponentId::HEALTH, ComponentId::POSITION});
	damageSystem = ecs.rosterSystem<DamageSystem>(SystemId::DAMAGE, {ComponentId::POSITION, ComponentId::COLLIDER, ComponentId::HEALTH});
	playerSystem = ecs.rosterSystem<PlayerSystem>(SystemId::PLAYER, {ComponentId::PLAYER});
	colldierDrawSystem = ecs.rosterSystem<ColliderDrawSystem>(SystemId::COLLIDER_DRAW, {ComponentId::COLLIDER, ComponentId::POSITION});
}

void World::update(uint dt) {
	player = playerSystem->getPlayer();
	camera = cameraSystem->getCamera();

	handleEvents();
	guiManager.update();

	controllerSystem->update(inputState, !guiManager.active());

	animalAiSystem->update();

	creatureMovementSystem->update(dt, gridMap, map);
	collisionSystem->update();

	itemPickupSystem->update();
	lootSystem->update();

	cameraSystem->update(player);
	healthSystem->update();

	creatureAnimationSystem->update();

	tileDrawSystem->update(map);
	entityDrawSystem->update(camera);
	colldierDrawSystem->update(camera);

	guiManager.draw();

	for (Entity entity : ecs.dead) {
		if (ecs.hasComponent<ResourceComponent>(entity)) {
			pair pos = round(ecs.getComponent<PositionComponent>(entity).position);
			gridMap.erase(gridMap.find(pos));
		}
	}
	ecs.update();

	player = playerSystem->getPlayer();
	camera = cameraSystem->getCamera();
}

void World::handleEvents() {
	if (!player) return;
	PlayerComponent& playerComponent = ecs.getComponent<PlayerComponent>(player);
	InventoryComponent& inventoryComponent = ecs.getComponent<InventoryComponent>(player);
	CreatureStateComponent& creatureStateComponent = ecs.getComponent<CreatureStateComponent>(player);
	PositionComponent& positionComponent = ecs.getComponent<PositionComponent>(player);
	ItemContainer& activeItemContainer = inventoryComponent.inventory.itemContainers[playerComponent.activeSlot][0];

	for (InputEvent event : inputEvents) {
		if (guiManager.handleEvent(event)) continue;
		if (event.id == InputEventId::INVENTORY) {
			Sprite sprite = Sprite(SpriteSheet::INVENTORY, {0, 0}, {10, 10});
			std::unique_ptr<Widget> inventoryGui = std::make_unique<Widget>(pair(0, 0), pair(150, 150), sprite);
			
			int spacing = 20;
			Inventory& inventory = inventoryComponent.inventory;

			for (int x = 0; x < inventory.size.x; x++) {
				pair position = {spacing * x - spacing * (inventory.size.x - 1) / 2, -3 * spacing};
				inventoryGui->addGuiElement(std::make_unique<ItemSlot>(position, inventory.itemContainers[x][0]));
			}

			for (int y = 1; y < inventory.size.y; y++) {
				for (int x = 0; x < inventory.size.x; x++) {
					pair position = {spacing * x - spacing * (inventory.size.x - 1) / 2, spacing * (y - 2)};
					inventoryGui->addGuiElement(std::make_unique<ItemSlot>(position, inventory.itemContainers[x][y]));
				}
			}
			guiManager.open(std::move(inventoryGui));

		} else if (event.id == InputEventId::THROW) {
			vec position = positionComponent.position + unitVectors[creatureStateComponent.facing - 1];
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

		if (!camera) continue;
		if (event.id == InputEventId::PRIMARY) {
			vec cameraPosition = ecs.getComponent<PositionComponent>(camera).position;
			uint zoom = ecs.getComponent<CameraComponent>(camera).zoom;
			vec position = cameraPosition + vec(event.mousePosition - Window::instance->size / 2) / (zoom * BIT);
			forageSystem->update(position, activeItemContainer.item);
			damageSystem->update(player, position, activeItemContainer.item);
		}
	}
}