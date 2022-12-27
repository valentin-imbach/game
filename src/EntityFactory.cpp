
#include "EntityFactory.hpp"
#include "ECS.hpp"
#include "ECS_types.hpp"
#include "Item.hpp"
#include "Components.hpp"

ECS* EntityFactory::ecs = nullptr;
GridMap* EntityFactory::gridMap = nullptr;

Entity EntityFactory::createPlayer(vec position) {
	Entity player = ecs->createEntity();
	ecs->addComponent<PositionComponent>({{8, 8}}, player);
	ecs->addComponent<CreatureStateComponent>({CreatureState::IDLE, Direction::EAST}, player);
	ecs->addComponent<DirectionComponent>({Direction::EAST}, player);
	ecs->addComponent<MovementComponent>({2}, player);
	ecs->addComponent<ControllerComponent>({}, player);
	SpriteStack spriteStack;
	spriteStack.addSprite({SpriteSheet::PLAYER, {0, 0}, {1, 2}, 1, 100});
	ecs->addComponent<SpriteComponent>({spriteStack, 1}, player);
	Collider collider = {{0, 0}, {0.6f, 0.6f}};
	ecs->addComponent<ColliderComponent>({collider}, player);
	ecs->addComponent<InventoryComponent>({Inventory({7, 6})}, player);
	ecs->addComponent<HealthComponent>({20, 20}, player);
	ecs->addComponent<PlayerComponent>({0}, player);
	return player;
}

Entity EntityFactory::createCamera(vec position, uint8_t zoom) {
	Entity camera = ecs->createEntity();
	ecs->addComponent<CameraComponent>({zoom}, camera);
	ecs->addComponent<PositionComponent>({position}, camera);
	return camera;
}

Entity EntityFactory::createResource(ResourceId::value resourceId, pair position) {
	Entity resource = ecs->createEntity();
	ecs->addComponent<PositionComponent>({position}, resource);
	if (resource) (*gridMap)[position] = resource;
	SpriteStack spriteStack;
	ToolId::value toolId;
	uint8_t height;
	ItemId::value lootId;
	pair lootRange;
	switch (resourceId) {
		case ResourceId::TREE:
			spriteStack.addSprite({SpriteSheet::RESOURCES, {0, 0}, {1, 3}});
			toolId = ToolId::AXE;
			height = 2;
			lootId = ItemId::ASHWOOD_PLANK;
			lootRange = {2, 4};
			break;
		case ResourceId::ROCK:
			spriteStack.addSprite({SpriteSheet::RESOURCES, {0, 3}, {1, 2}});
			toolId = ToolId::PICK_AXE;
			height = 1;
			lootId = ItemId::BASALT_COBBLE;
			lootRange = {2, 4};
			break;
		default:
			return 0;
	}

	ecs->addComponent<SpriteComponent>({spriteStack, height}, resource);
	ecs->addComponent<ResourceComponent>({toolId}, resource);
	ecs->addComponent<LootComponent>({lootId, lootRange, 1.0f}, resource);
	ecs->addComponent<HealthComponent>({5, 5}, resource);
	return resource;
}

Entity EntityFactory::createAnimal(AnimalId::value animalId, vec position) {
	Entity animal = ecs->createEntity();
	ecs->addComponent<PositionComponent>({position}, animal);
	ecs->addComponent<CreatureStateComponent>({CreatureState::IDLE, Direction::EAST}, animal);
	ecs->addComponent<DirectionComponent>({Direction::EAST}, animal);
	ecs->addComponent<MovementComponent>({0.5f}, animal);
	SpriteStack spriteStack;
	spriteStack.addSprite({SpriteSheet::COW, {0, 0}, {1, 2}, 1, 100});
	ecs->addComponent<SpriteComponent>({spriteStack, 1}, animal);
	Collider collider = {{0, 0}, {0.6f, 0.6f}};
	ecs->addComponent<ColliderComponent>({collider}, animal);
	ecs->addComponent<AnimalAiComponent>({0, 0}, animal);
	//ecs->addComponent<LootComponent>({ItemId::LEATHER, {1,3}, 1.0f}, animal);
	ecs->addComponent<HealthComponent>({10, 10}, animal);
	ecs->addComponent<ForceComponent>({{0, 0}}, animal);
	return animal;
}

Entity EntityFactory::createItem(ItemId::value itemId, uint8_t count) {
	Entity item = ecs->createEntity();
	Collider collider = {{0, 0}, {0.4f, 0.4f}};
	ecs->addComponent<ColliderComponent>({collider}, item);
	SpriteStack spriteStack;
	spriteStack.addSprite({SpriteSheet::ITEMS, {(itemId - 1) % 6, (itemId - 1) / 6}, {1, 1}});
	ecs->addComponent<SpriteComponent>({spriteStack, 0, 0.5f}, item);
	ItemComponent itemComponent = {itemId, count};
	if (itemId) {
		ItemTemplate* temp = ItemTemplate::templates[itemId].get();
	}
	ecs->addComponent<ItemComponent>(itemComponent, item);
	return item;
}

Entity EntityFactory::createItem(ItemId::value itemId, uint8_t count, vec position) {
	Entity item = createItem(itemId, count);
	ecs->addComponent<PositionComponent>({position}, item);
	return item;
}