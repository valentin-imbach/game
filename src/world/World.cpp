
#include "World.hpp"
#include <memory>
#include "Components.hpp"
#include "ECS_types.hpp"
#include "Events.hpp"
#include "ForageSystem.hpp"
#include "GuiElement.hpp"
#include "HealthSystem.hpp"
#include "Item.hpp"
#include "Sprite.hpp"
#include "Window.hpp"
#include "EntityFactory.hpp"

World::World(std::string name) : name(name) {
	rosterComponents();
	rosterSystems();

	guiManager.ecs = &ecs;
	EntityFactory::ecs = &ecs;
	EntityFactory::gridMap = &gridMap;

	player = EntityFactory::createPlayer({8, 8});
	Item rest = ecs.getComponent<InventoryComponent>(player).inventory.add(Item(ItemId::APPLE, 20));
	guiManager.add(std::make_unique<HotbarGui>(player));
	guiManager.add(std::make_unique<HealthBarGui>(player));

	camera = EntityFactory::createCamera({0, 0}, 4, player);

	EntityFactory::createResource(ResourceId::TREE, {7, 4});
	EntityFactory::createResource(ResourceId::ROCK, {4, 5});

	EntityFactory::createAnimal(AnimalId::COW, {6, 6});

	Entity axe = ecs.createEntity();
	SpriteStack axeSprites;
	axeSprites.addSprite(Sprite(SpriteSheet::ITEMS, {2, 0}, {1, 1}));
	ecs.addComponent<SpriteComponent>({axeSprites, 0, 0.5f}, axe);
	ecs.addComponent<ItemComponent>({Item(axe)}, axe);
	ecs.addComponent<ToolComponent>({ToolId::AXE}, axe);
	EntityFactory::createItemEntity(Item(axe), {7, 5});
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
}

void World::update(uint dt) {
	handleEvents();
	guiManager.update();

	if (guiManager.active()) inputState = 0;
	controllerSystem->update(inputState);

	animalAiSystem->update();

	creatureMovementSystem->update(dt, gridMap, map);
	collisionSystem->update();

	itemPickupSystem->update();
	lootSystem->update();
	healthSystem->update();

	creatureAnimationSystem->update();
	cameraSystem->update();

	tileDrawSystem->update(map);
	entityDrawSystem->update(camera);

	guiManager.draw();

	for (Entity entity : ecs.dead) {
		if (ecs.hasComponent<ResourceComponent>(entity)) {
			pair pos = round(ecs.getComponent<PositionComponent>(entity).position);
			gridMap.erase(gridMap.find(pos));
		}
	}
	ecs.update();
}

void World::handleEvents() {
	for (InputEvent event : inputEvents) {
		if (guiManager.handleEvent(event)) continue;
		if (event.id == InputEventId::INVENTORY) {
			std::unique_ptr<Widget> inventoryGui = std::make_unique<Widget>(pair(0, 0), pair(150, 150), Sprite(SpriteSheet::INVENTORY, {0, 0}, {10, 10}));
			Inventory& playerInventory = ecs.getComponent<InventoryComponent>(player).inventory;
			int spacing = 20;

			for (int x = 0; x < playerInventory.size.x; x++) {
				for (int y = 1; y < playerInventory.size.y; y++) {
					ItemContainer& container = playerInventory.itemContainers[x][y];
					pair position = {spacing * x - spacing * (playerInventory.size.x - 1) / 2, spacing * (y - 2)};
					inventoryGui->addGuiElement(std::make_unique<ItemSlot>(position, container));
				}
				ItemContainer& container = playerInventory.itemContainers[x][0];
				pair position = {spacing * x - spacing * (playerInventory.size.x - 1) / 2, -3 * spacing};
				inventoryGui->addGuiElement(std::make_unique<ItemSlot>(position, container));
			}

			guiManager.open(std::move(inventoryGui));
		}

		PlayerComponent& playerComponent = ecs.getComponent<PlayerComponent>(player);
		if (event.id == InputEventId::SELECT_1) playerComponent.activeSlot = 0;
		if (event.id == InputEventId::SELECT_2) playerComponent.activeSlot = 1;
		if (event.id == InputEventId::SELECT_3) playerComponent.activeSlot = 2;
		if (event.id == InputEventId::SELECT_4) playerComponent.activeSlot = 3;
		if (event.id == InputEventId::SELECT_5) playerComponent.activeSlot = 4;
		if (event.id == InputEventId::SELECT_6) playerComponent.activeSlot = 5;
		if (event.id == InputEventId::SELECT_7) playerComponent.activeSlot = 6;

		if (event.id == InputEventId::PRIMARY) {
			vec cameraPosition = ecs.getComponent<PositionComponent>(camera).position;
			uint zoom = ecs.getComponent<CameraComponent>(camera).zoom;
			pair position = round(cameraPosition + vec(event.mousePosition - Window::instance->size / 2) / (zoom * BIT));
			uint activeSlot = ecs.getComponent<PlayerComponent>(player).activeSlot;
			Inventory& inventory = ecs.getComponent<InventoryComponent>(player).inventory;
			forageSystem->update(position, inventory.itemContainers[activeSlot][0].item);
		}
	}
}