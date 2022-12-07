
#include "World.hpp"
#include "Components.hpp"
#include "Systems/CameraSystem.hpp"

World::World(std::string name) : name(name) {
	rosterComponents();
	rosterSystems();

	player = ecs.createEntity();
	ecs.addComponent<PositionComponent>({{0, 0}}, player);
	ecs.addComponent<CreatureStateComponent>({CreatureState::IDLE, Direction::EAST}, player);
	ecs.addComponent<DirectionComponent>({Direction::EAST}, player);
	ecs.addComponent<MovementComponent>({2}, player);
	ecs.addComponent<ControllerComponent>({}, player);
	SpriteStack playerSprites;
	playerSprites.addSprite({SpriteSheet::PLAYER, {0, 0}, {1, 2}, 1, 100});
	ecs.addComponent<SpriteComponent>({playerSprites, 1}, player);
	Collider playerCollider = {{-0.5f, -0.5f}, {1, 1}};
	ecs.addComponent<ColliderComponent>({playerCollider}, player);

	camera = ecs.createEntity();
	ecs.addComponent<CameraComponent>({4, player}, camera);
	ecs.addComponent<PositionComponent>({{0, 0}}, camera);

	Entity tree = ecs.createEntity();
	ecs.addComponent<PositionComponent>({{7, 4}}, tree);
	gridEntities[{7,4}] = tree;
	SpriteStack treeSprites;
	treeSprites.addSprite({SpriteSheet::RESOURCES, {0, 0}, {1, 3}});
	ecs.addComponent<SpriteComponent>({treeSprites, 2}, tree);

	Entity rock = ecs.createEntity();
	ecs.addComponent<PositionComponent>({{4, 5}}, rock);
	gridEntities[{4,5}] = rock;
	SpriteStack rockSprites;
	rockSprites.addSprite({SpriteSheet::RESOURCES, {0, 3}, {1, 2}});
	ecs.addComponent<SpriteComponent>({rockSprites, 1}, rock);
	Collider rockCollider = {{-0.5f, -0.5f}, {1, 1}};
	ecs.addComponent<ColliderComponent>({rockCollider}, rock);
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
}

void World::rosterSystems() {
	spriteSystem = ecs.rosterSystem<SpriteSystem>(SystemId::SPRITE, {ComponentId::SPRITE, ComponentId::POSITION});
	creatureMovementSystem = ecs.rosterSystem<CreatureMovementSystem>(SystemId::CREATURE_MOVEMENT, {ComponentId::MOVEMENT, ComponentId::CREATURE_STATE, ComponentId::POSITION, ComponentId::COLLIDER});
	controllerSystem = ecs.rosterSystem<ControllerSystem>(SystemId::CONTROLLER, {ComponentId::CONTROLLER, ComponentId::CREATURE_STATE, ComponentId::DIRECTION});
	cameraSystem = ecs.rosterSystem<CameraSystem>(SystemId::CAMERA, {ComponentId::CAMERA, ComponentId::POSITION});
	creatureAnimationSystem = ecs.rosterSystem<CreatureAnimationSystem>(SystemId::CREATURE_ANIMATION, {ComponentId::CREATURE_STATE, ComponentId::SPRITE, ComponentId::DIRECTION});
	collisionSystem = ecs.rosterSystem<CollisionSystem>(SystemId::COLLISION, {ComponentId::COLLIDER, ComponentId::POSITION});
}

void World::update(uint dt) {
	controllerSystem->update();
	collisionSystem->update();

	creatureMovementSystem->update(dt);
	cameraSystem->update();

	creatureAnimationSystem->update();
	std::vector<std::pair<float, DrawCall>> drawQueue;
	spriteSystem->update(camera, drawQueue);

	drawMap();
	drawEntities(drawQueue);
}

void World::drawMap() {
	vec cameraPosition = ecs.getComponent<PositionComponent>(camera).position;
	int zoom = ecs.getComponent<CameraComponent>(camera).zoom;
	pair screenSize = Window::instance->size;
	int border = BIT * zoom / 2;

	pair range = ceil(vec(screenSize) / (2 * BIT * zoom));
	pair start = round(cameraPosition);

	int x1 = std::max(0, start.x - range.x);
	int x2 = std::min(MAP_WIDTH - 1, start.x + range.x);
	int y1 = std::max(0, start.y - range.y);
	int y2 = std::min(MAP_HEIGHT - 1, start.y + range.y);

	for (int x = x1; x <= x2; x++) {
		for (int y = y1; y <= y2; y++) {
			SpriteStack spriteStack = map.tiles[x][y]->spriteStack;
			vec tilePosition = {x, y};
			pair screenPosition = round(BIT * zoom * (tilePosition - cameraPosition)) + screenSize / 2;
			spriteStack.draw(screenPosition, zoom, true);
		}
	}
}

void World::drawEntities(DrawQueue& drawQueue) {
	std::sort(drawQueue.begin(), drawQueue.end(), [](auto& l, auto& r) { return l.first < r.first; });
	for (auto& p : drawQueue) {
		p.second.spriteStack.draw(p.second.position, p.second.scale, p.second.centered);
	}
}