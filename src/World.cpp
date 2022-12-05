
#include "World.hpp"
#include "Components.hpp"

World::World(std::string name) : name(name) {
	rosterComponents();
	rosterSystems();

	camera = ecs.createEntity();
	ecs.addComponent<CameraComponent>({4}, camera);
	ecs.addComponent<PositionComponent>({{0, 0}}, camera);

	player = ecs.createEntity();
	ecs.addComponent<PositionComponent>({{0, 0}}, player);
	ecs.addComponent<SpriteComponent>({SpriteSheet::HOLE}, player);
}

void World::rosterComponents() {
	ecs.rosterComponent<PositionComponent>(ComponentId::POSITION);
	ecs.rosterComponent<SpriteComponent>(ComponentId::SPRITE);
	ecs.rosterComponent<CameraComponent>(ComponentId::CAMERA);
}

void World::rosterSystems() {
	spriteSystem = ecs.rosterSystem<SpriteSystem>(SystemId::SPRITE, {ComponentId::SPRITE, ComponentId::POSITION});
	gravitySystem = ecs.rosterSystem<GravitySystem>(SystemId::GRAVITY, {ComponentId::POSITION});
}

void World::update() {
	// gravitySystem -> update();
	renderMap();
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