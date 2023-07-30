
#include "GuiManager.hpp"
#include "GuiElement.hpp"
#include "World.hpp"

uint GuiManager::scale = 3;
bool GuiManager::box = true;

void GuiManager::update() {
	for (auto& guiElement : guiElements) guiElement->reposition();
	for (auto& guiElement : guiElements) guiElement->update(this);

	if (buildMode) {
		pair gridPosition = vec::round(world->camera.worldPosition(Window::instance->mousePosition));
		world->ecs.getComponent<PositionComponent>(buildMode).position = gridPosition;
		world->ecs.getComponent<GridComponent>(buildMode).anker = gridPosition;

		SpriteComponent& spriteComponent = world->ecs.getComponent<SpriteComponent>(buildMode);
		if (world->playerRealm->free(gridPosition)) {
			spriteComponent.effects[SpriteEffectId::RED] = {false, 0};
		} else {
			spriteComponent.effects[SpriteEffectId::RED] = {true, 0};
		}
	}

	if (!primary) return;
	primary->reposition();
	primary->update(this);
	if (!secondary) return;
	secondary->reposition();
	secondary->update(this);
}

void GuiManager::draw() {
	for (auto& guiElement : guiElements) guiElement->draw();
	if (!primary) return;
	if (secondary) secondary->draw();
	primary->draw();
	mouseItemContainer.draw(mousePosition, GuiManager::scale);
}

bool GuiManager::handleEvent(InputEvent event) {
	if (buildMode && event.id == InputEventId::PRIMARY) {
		PositionComponent& positionComponent = world->ecs.getComponent<PositionComponent>(buildMode);
		GridComponent& gridComponent = world->ecs.getComponent<GridComponent>(buildMode);
		pair pos = vec::round(positionComponent.position);
		if (world->playerRealm->free(pos)) {
			world->ecs.getComponent<SpriteComponent>(buildMode).z = 0;
			world->ecs.removeComponent<ChunkComponent>(buildMode);
			world->playerRealm->linkGrid(buildMode, gridComponent.anker, gridComponent.size, gridComponent.solid, gridComponent.opaque);
			buildMode = 0;
		}
		return true;
	}

	if (event.id == InputEventId::INVENTORY) {
		if (primary) {
			close();
			return true;
		}
	}

	if (primary && primary->handleEvent(event)) return true;
	if (secondary && secondary->handleEvent(event)) return true;
	for (auto& guiElement : guiElements) {
		if (guiElement->handleEvent(event)) return true;
	}
	if (primary) return true;
	return false;
}

void GuiManager::open(std::unique_ptr<GuiElement> a, std::unique_ptr<GuiElement> b) {
	primary = std::move(a);
	secondary = std::move(b);
	if (!primary) return;
	if (buildMode) {
		world->ecs.destroyEntity(buildMode);
		buildMode = 0;
	}
	primary->position.y = 0;
	primary->alignment = Direction::NONE;
	if (secondary) {
		secondary->position.y = 0;
		secondary->alignment = Direction::NONE;

		primary->position.x = -80;
		secondary->position.x = 80;
	} else {
		primary->position.x = 0;
	}
}

void GuiManager::close() {
	primary = nullptr;
	secondary = nullptr;
}

bool GuiManager::active() {
	return bool(primary);
}

void GuiManager::add(std::unique_ptr<GuiElement> guiElement) {
	guiElements.push_back(std::move(guiElement));
}