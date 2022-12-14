
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
	Collider collider = {{-0.3f, -0.3f}, {0.6f, 0.6f}};
	ecs->addComponent<ColliderComponent>({collider}, player);
	ecs->addComponent<InventoryComponent>({Inventory({7, 6})}, player);
	ecs->addComponent<HealthComponent>({20, 20}, player);
	ecs->addComponent<PlayerComponent>({0}, player);
	return player;
}

Entity EntityFactory::createCamera(vec position, uint8_t zoom, Entity target) {
	Entity camera = ecs->createEntity();
	ecs->addComponent<CameraComponent>({zoom, target}, camera);
	ecs->addComponent<PositionComponent>({position}, camera);
	return camera;
}

Entity EntityFactory::createResource(ResourceId resourceId, pair position) {
	Entity resource = ecs->createEntity();
	ecs->addComponent<PositionComponent>({position}, resource);
	if (resource) (*gridMap)[position] = resource;
	SpriteStack spriteStack;
	ToolId toolId;
	uint8_t height;
	Item loot;
	switch (resourceId) {
		case ResourceId::TREE:
			spriteStack.addSprite({SpriteSheet::RESOURCES, {0, 0}, {1, 3}});
			toolId = ToolId::AXE;
			height = 2;
			loot = Item(ItemId::ASHWOOD_PLANK, 3);
			break;
		case ResourceId::ROCK:
			spriteStack.addSprite({SpriteSheet::RESOURCES, {0, 3}, {1, 2}});
			toolId = ToolId::PICK_AXE;
			height = 1;
			loot = Item(ItemId::BASALT_COBBLE, 3);
			break;
		default:
			return 0;
	}
	
	ecs->addComponent<SpriteComponent>({spriteStack, height}, resource);
	ecs->addComponent<ResourceComponent>({toolId}, resource);
	ecs->addComponent<LootComponent>({loot}, resource);
	ecs->addComponent<HealthComponent>({5, 5}, resource);
	return resource;
}

Entity EntityFactory::createAnimal(AnimalId animalId, vec position) {
	Entity animal = ecs->createEntity();
	ecs->addComponent<PositionComponent>({position}, animal);
	ecs->addComponent<CreatureStateComponent>({CreatureState::IDLE, Direction::EAST}, animal);
	ecs->addComponent<DirectionComponent>({Direction::EAST}, animal);
	ecs->addComponent<MovementComponent>({0.5f}, animal);
	SpriteStack spriteStack;
	spriteStack.addSprite({SpriteSheet::COW, {0, 0}, {1, 2}, 1, 100});
	ecs->addComponent<SpriteComponent>({spriteStack, 1}, animal);
	Collider collider = {{-0.3f, -0.3f}, {0.6f, 0.6f}};
	ecs->addComponent<ColliderComponent>({collider}, animal);
	ecs->addComponent<AnimalAiComponent>({}, animal);
	ecs->addComponent<HealthComponent>({10, 10}, animal);
	return animal;
}

Entity EntityFactory::createItemEntity(Item item, vec position) {
	Collider collider = {{-0.2f, -0.2f}, {0.4f, 0.4f}};
	if (item.entity) {
		ecs->addComponent<PositionComponent>({position}, item.entity);
		ecs->addComponent<ColliderComponent>({collider}, item.entity);
		return item.entity;
	}
	Entity entity = ecs -> createEntity();
	ecs->addComponent<PositionComponent>({position}, entity);
	int index = int(item.itemId) - 1;
	SpriteStack spriteStack;
	spriteStack.addSprite({SpriteSheet::ITEMS, {index % 6, index / 6}, {1, 1}});
	ecs->addComponent<SpriteComponent>({spriteStack, 0, 0.5f}, entity);
	ecs->addComponent<ColliderComponent>({collider}, entity);
	ecs->addComponent<ItemComponent>({item}, entity);
	return entity;
}