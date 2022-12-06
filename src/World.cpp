
#include "World.hpp"
#include "Components.hpp"
#include "Systems/CameraSystem.hpp"

World::World(std::string name) : name(name) {
	rosterComponents();
	rosterSystems();

	player = ecs.createEntity();
	ecs.addComponent<PositionComponent>({{0, 0}}, player);
	ecs.addComponent<CreatureStateComponent>({CreatureState::IDLE}, player);
	ecs.addComponent<DirectionComponent>({Direction::EAST}, player);
	ecs.addComponent<MovementComponent>({1}, player);
	ecs.addComponent<ControllerComponent>({}, player);
	ecs.addComponent<SpriteComponent>({SpriteSheet::HOLE}, player);

	camera = ecs.createEntity();
	ecs.addComponent<CameraComponent>({4, player}, camera);
	ecs.addComponent<PositionComponent>({{0, 0}}, camera);
}

void World::rosterComponents() {
	ecs.rosterComponent<PositionComponent>(ComponentId::POSITION);
	ecs.rosterComponent<SpriteComponent>(ComponentId::SPRITE);
	ecs.rosterComponent<CameraComponent>(ComponentId::CAMERA);
	ecs.rosterComponent<CreatureStateComponent>(ComponentId::CREATURE_STATE);
	ecs.rosterComponent<ControllerComponent>(ComponentId::CONTROLLER);
	ecs.rosterComponent<DirectionComponent>(ComponentId::DIRECTION);
	ecs.rosterComponent<MovementComponent>(ComponentId::MOVEMENT);
}

void World::rosterSystems() {
	spriteSystem = ecs.rosterSystem<SpriteSystem>(SystemId::SPRITE, {ComponentId::SPRITE, ComponentId::POSITION});
	creatureMovementSystem = ecs.rosterSystem<CreatureMovementSystem>(SystemId::CREATURE_MOVEMENT, {ComponentId::MOVEMENT, ComponentId::CREATURE_STATE, ComponentId::POSITION});
	controllerSystem = ecs.rosterSystem<ControllerSystem>(SystemId::CONTROLLER, {ComponentId::CONTROLLER, ComponentId::CREATURE_STATE, ComponentId::DIRECTION});
	cameraSystem = ecs.rosterSystem<CameraSystem>(SystemId::CAMERA, {ComponentId::CAMERA, ComponentId::POSITION});
}

void World::update(uint dt) {
	renderMap();
	controllerSystem -> update();
	creatureMovementSystem -> update(dt);
	cameraSystem -> update();
	spriteSystem->update(camera);
}

void World::renderMap() {
	for (int x = 0; x < MAP_WIDTH; x++) {
		for (int y = 0; y < MAP_HEIGHT; y++) {
			SpriteSheet spriteSheet = Tile::spriteSheets[size_t(map.tiles[x][y].tileId)];
			pair style = map.tiles[x][y].style;

			vec cameraPosition = ecs.getComponent<PositionComponent>(camera).position;
			int zoom = ecs.getComponent<CameraComponent>(camera).zoom;
			vec tilePosition = {x, y};

			pair screenPosition = round(BIT * zoom * (tilePosition - cameraPosition)) + (Window::instance->size) / 2;
			TextureManager::drawTexture(spriteSheet, BIT * style, {BIT, BIT}, screenPosition, zoom, true);
		}
	}
}