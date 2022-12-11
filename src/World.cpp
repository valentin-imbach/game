
#include "World.hpp"
#include <memory>
#include "Components.hpp"
#include "ECS_types.hpp"
#include "Events.hpp"
#include "GuiElement.hpp"
#include "Item.hpp"
#include "Sprite.hpp"

World* World::world = nullptr;

World::World(std::string name) : name(name) {
	world = this;
	rosterComponents();
	rosterSystems();

	player = ecs.createEntity();
	ecs.addComponent<PositionComponent>({{8, 8}}, player);
	ecs.addComponent<CreatureStateComponent>({CreatureState::IDLE, Direction::EAST}, player);
	ecs.addComponent<DirectionComponent>({Direction::EAST}, player);
	ecs.addComponent<MovementComponent>({2}, player);
	ecs.addComponent<ControllerComponent>({}, player);
	SpriteStack playerSprites;
	playerSprites.addSprite({SpriteSheet::PLAYER, {0, 0}, {1, 2}, 1, 100});
	ecs.addComponent<SpriteComponent>({playerSprites, 1}, player);
	Collider playerCollider = {{-0.3f, -0.3f}, {0.6f, 0.6f}};
	ecs.addComponent<ColliderComponent>({playerCollider}, player);
	ecs.addComponent<InventoryComponent>({Inventory(7,4)}, player);

	camera = ecs.createEntity();
	ecs.addComponent<CameraComponent>({4, player}, camera);
	ecs.addComponent<PositionComponent>({{0, 0}}, camera);

	Entity tree = ecs.createEntity();
	ecs.addComponent<PositionComponent>({{7, 4}}, tree);
	gridMap[{7, 4}] = tree;
	SpriteStack treeSprites;
	treeSprites.addSprite({SpriteSheet::RESOURCES, {0, 0}, {1, 3}});
	ecs.addComponent<SpriteComponent>({treeSprites, 2}, tree);

	Entity rock = ecs.createEntity();
	ecs.addComponent<PositionComponent>({{4, 5}}, rock);
	gridMap[{4, 5}] = rock;
	SpriteStack rockSprites;
	rockSprites.addSprite({SpriteSheet::RESOURCES, {0, 3}, {1, 2}});
	ecs.addComponent<SpriteComponent>({rockSprites, 1}, rock);
	
	Entity cow = ecs.createEntity();
	ecs.addComponent<PositionComponent>({{3, 3}}, cow);
	ecs.addComponent<CreatureStateComponent>({CreatureState::IDLE, Direction::EAST}, cow);
	ecs.addComponent<DirectionComponent>({Direction::EAST}, cow);
	ecs.addComponent<MovementComponent>({0.5f}, cow);
	SpriteStack cowSprites;
	cowSprites.addSprite({SpriteSheet::COW, {0, 0}, {1, 2}, 1, 100});
	ecs.addComponent<SpriteComponent>({cowSprites, 1}, cow);
	Collider cowCollider = {{-0.3f, -0.3f}, {0.6f, 0.6f}};
	ecs.addComponent<ColliderComponent>({cowCollider}, cow);
	ecs.addComponent<AnimalAiComponent>({}, cow);

	Entity sword = ecs.createEntity();
	ecs.addComponent<PositionComponent>({{3, 5}}, sword);
	SpriteStack swordSprites;
	swordSprites.addSprite({SpriteSheet::ITEMS, {0, 0}, {1, 1}});
	ecs.addComponent<SpriteComponent>({swordSprites, 0, 0.5f}, sword);
	Collider swordCollider = {{-0.2f, -0.2f}, {0.4f, 0.4f}};
	ecs.addComponent<ColliderComponent>({swordCollider}, sword);
	ecs.addComponent<ItemComponent>({Item(sword)}, sword);
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
}

void World::update(uint dt) {
	handleEvents();
	guiManager.update();
	
	if (guiManager.active()) inputStates = 0;
	controllerSystem->update(inputStates);

	animalAiSystem->update();

	creatureMovementSystem->update(dt, gridMap, map);
	collisionSystem->update();

	itemPickupSystem->update();

	creatureAnimationSystem->update();
	cameraSystem->update();

	tileDrawSystem->update(map);
	entityDrawSystem->update(camera);

	guiManager.draw();
}

void World::handleEvents() {
	for (InputEvent event : inputEvents) {
		if (guiManager.handleEvent(event)) continue;
		if (event == InputEvent::INVENTORY) {
			std::unique_ptr<Widget> inventory = std::make_unique<Widget>(pair(0,0), Sprite(SpriteSheet::INVENTORY, {0,0}, {10, 10}));
			guiManager.open(std::move(inventory));
		}
	}
}

World::~World() {
	world = nullptr;
}