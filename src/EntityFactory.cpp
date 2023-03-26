
#include "EntityFactory.hpp"
#include "ECS.hpp"
#include "ECS_types.hpp"
#include "Item.hpp"
#include "Components.hpp"
#include "World.hpp"

World* EntityFactory::world = nullptr;
uint EntityFactory::seed = 1729;

Entity EntityFactory::createPlayer(vec position) {
	Entity player = world->ecs.createEntity();
	world->ecs.addComponent<PositionComponent>({{8, 8}}, player);
	world->ecs.addComponent<CreatureStateComponent>({CreatureState::IDLE, Direction::EAST}, player);
	world->ecs.addComponent<DirectionComponent>({Direction::EAST}, player);
	world->ecs.addComponent<MovementComponent>({2}, player);
	world->ecs.addComponent<ControllerComponent>({}, player);
	SpriteStack spriteStack;
	spriteStack.addSprite({SpriteSheet::PLAYER, {0, 0}, {1, 2}, 1, 100}, pair(0, -1));
	world->ecs.addComponent<SpriteComponent>({spriteStack}, player);
	Collider collider = {{0, 0}, {0.6f, 0.6f}};
	world->ecs.addComponent<ColliderComponent>({collider}, player);
	world->ecs.addComponent<InventoryComponent>({Inventory({7, 5})}, player);
	world->ecs.addComponent<HealthComponent>({20, 20}, player);
	Inventory equipment({3,4});
	equipment.itemContainers[0][0].itemKind = ItemKind::CLOTHING_HEAD;
	equipment.itemContainers[0][1].itemKind = ItemKind::CLOTHING_BODY;
	equipment.itemContainers[0][2].itemKind = ItemKind::CLOTHING_LEGS;
	equipment.itemContainers[0][3].itemKind = ItemKind::CLOTHING_FEET;
	equipment.itemContainers[1][0].itemKind = ItemKind::ARMOR_HEAD;
	equipment.itemContainers[1][1].itemKind = ItemKind::ARMOR_BODY;
	equipment.itemContainers[1][2].itemKind = ItemKind::ARMOR_LEGS;
	equipment.itemContainers[1][3].itemKind = ItemKind::ARMOR_FEET;
	for (int y = 0; y < 4; y++) equipment.itemContainers[2][y].itemKind = ItemKind::ACCESSORY;
	world->ecs.addComponent<PlayerComponent>({Inventory({7, 1}), equipment, 0}, player);
	return player;
}

Entity EntityFactory::createCamera(vec position, uint8_t zoom) {
	Entity camera = world->ecs.createEntity();
	world->ecs.addComponent<CameraComponent>({zoom}, camera);
	world->ecs.addComponent<PositionComponent>({position}, camera);
	return camera;
}

bool EntityFactory::free(pair position, pair size) {
	for (int x = 0; x < size.x; x++) {
		for (int y = 0; y < size.y; y++) {
			pair offset(x,y);
			if (world->gridMap.find(position + offset) != world->gridMap.end()) return false;
		}
	}
	return true;
}

Entity EntityFactory::createResource(ResourceId::value resourceId, pair position) {
	if (!resourceId || !ResourceTemplate::templates[resourceId]) return 0;
	ResourceTemplate* resourceTemplate = ResourceTemplate::templates[resourceId].get();
	if (!free(position, resourceTemplate->size)) return 0;

	Entity resource = world->ecs.createEntity();
	if (!resource) return 0;

	world->ecs.addComponent<PositionComponent>({position}, resource);
	world->ecs.addComponent<GridComponent>({position, resourceTemplate->size, resourceTemplate->solid, false}, resource);

	SpriteStack spriteStack;
	uint variation = rand_uint(seed++, resourceTemplate->variations);
	pair spritePosition(resourceTemplate->anker.x + variation * resourceTemplate->size.x, resourceTemplate->anker.y - resourceTemplate->height);
	pair spriteSize(resourceTemplate->size.x, resourceTemplate->size.y + resourceTemplate->height);
	spriteStack.addSprite({SpriteSheet::RESOURCES, spritePosition, spriteSize}, pair(0, -resourceTemplate->height));

	world->ecs.addComponent<SpriteComponent>({spriteStack}, resource);
	world->ecs.addComponent<ResourceComponent>({resourceTemplate->toolId}, resource);
	world->ecs.addComponent<LootComponent>({resourceTemplate->lootTable}, resource);
	world->ecs.addComponent<HealthComponent>({resourceTemplate->health, resourceTemplate->health}, resource);
	
	return resource;
}

Entity EntityFactory::createAnimal(AnimalId::value animalId, vec position) {
	Entity animal = world->ecs.createEntity();
	world->ecs.addComponent<PositionComponent>({position}, animal);
	world->ecs.addComponent<CreatureStateComponent>({CreatureState::IDLE, Direction::EAST}, animal);
	world->ecs.addComponent<DirectionComponent>({Direction::EAST}, animal);
	world->ecs.addComponent<MovementComponent>({0.5f}, animal);
	SpriteStack spriteStack;
	spriteStack.addSprite({SpriteSheet::COW, {0, 0}, {1, 2}, 1, 100}, pair(0, -1));
	world->ecs.addComponent<SpriteComponent>({spriteStack}, animal);
	Collider collider = {{0, 0}, {0.6f, 0.6f}};
	world->ecs.addComponent<ColliderComponent>({collider}, animal);
	world->ecs.addComponent<AnimalAiComponent>({0, 0}, animal);
	//world->ecs.addComponent<LootComponent>({ItemId::APPLE, {1,3}, 1.0f}, animal);
	world->ecs.addComponent<HealthComponent>({10, 10}, animal);
	world->ecs.addComponent<ForceComponent>({{0, 0}}, animal);
	return animal;
}

Entity EntityFactory::createItem(ItemId::value itemId, uint8_t count) {
	Entity item = world->ecs.createEntity();
	Collider collider = {{0, 0}, {0.4f, 0.4f}};
	world->ecs.addComponent<ColliderComponent>({collider}, item);
	SpriteStack spriteStack;
	spriteStack.addSprite({SpriteSheet::ITEMS, {(itemId - 1) % 6, (itemId - 1) / 6}, {1, 1}});
	world->ecs.addComponent<SpriteComponent>({spriteStack, 0.5f, {ShaderId::BOUNCE, 0}}, item);
	ItemComponent itemComponent = {itemId, count};
	// if (itemId && ItemTemplate::templates[itemId]) {
	// 	ItemTemplate* temp = ItemTemplate::templates[itemId].get();
	// }
	world->ecs.addComponent<ItemComponent>(itemComponent, item);
	return item;
}

Entity EntityFactory::createItem(ItemId::value itemId, uint8_t count, vec position) {
	Entity item = createItem(itemId, count);
	world->ecs.addComponent<PositionComponent>({position}, item);
	return item;
}