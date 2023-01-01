
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
	ecs->addComponent<InventoryComponent>({Inventory({7, 5})}, player);
	ecs->addComponent<HealthComponent>({20, 20}, player);
	ecs->addComponent<PlayerComponent>({Inventory({7, 1}), 0}, player);
	return player;
}

Entity EntityFactory::createCamera(vec position, uint8_t zoom) {
	Entity camera = ecs->createEntity();
	ecs->addComponent<CameraComponent>({zoom}, camera);
	ecs->addComponent<PositionComponent>({position}, camera);
	return camera;
}

bool EntityFactory::free(pair position, pair size) {
	for (int x = 0; x < size.x; x++) {
		for (int y = 0; y < size.y; y++) {
			pair offset(x,y);
			if ((*gridMap)[position + offset]) return false;
		}
	}
	return true;
}

Entity EntityFactory::createResource(ResourceId::value resourceId, pair position) {
	if (!resourceId || !ResourceTemplate::templates[resourceId]) return 0;
	ResourceTemplate* resourceTemplate = ResourceTemplate::templates[resourceId].get();
	if (!free(position, resourceTemplate->size)) return 0;

	Entity resource = ecs->createEntity();
	if (!resource) return 0;

	ecs->addComponent<PositionComponent>({position}, resource);
	ecs->addComponent<GridComponent>({position, resourceTemplate->size, true}, resource);

	SpriteStack spriteStack;
	pair spritePosition(resourceTemplate->position.x, resourceTemplate->position.y - resourceTemplate->height);
	pair spriteSize(resourceTemplate->size.x, resourceTemplate->size.y + resourceTemplate->height);
	spriteStack.addSprite({SpriteSheet::RESOURCES, spritePosition, spriteSize});

	ecs->addComponent<SpriteComponent>({spriteStack, resourceTemplate->height}, resource);
	ecs->addComponent<ResourceComponent>({resourceTemplate->toolId}, resource);
	ecs->addComponent<LootComponent>({resourceTemplate->lootTable}, resource);
	ecs->addComponent<HealthComponent>({5, 5}, resource);

	for (int x = 0; x < resourceTemplate->size.x; x++) {
		for (int y = 0; y < resourceTemplate->size.y; y++) {
			pair offset(x,y);
			(*gridMap)[position + offset] = resource;
		}
	}
	
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
	ecs->addComponent<SpriteComponent>({spriteStack, 0, 0.5f, {ShaderId::BOUNCE, SDL_GetTicks()}}, item);
	ItemComponent itemComponent = {itemId, count};
	// if (itemId && ItemTemplate::templates[itemId]) {
	// 	ItemTemplate* temp = ItemTemplate::templates[itemId].get();
	// }
	ecs->addComponent<ItemComponent>(itemComponent, item);
	return item;
}

Entity EntityFactory::createItem(ItemId::value itemId, uint8_t count, vec position) {
	Entity item = createItem(itemId, count);
	ecs->addComponent<PositionComponent>({position}, item);
	return item;
}