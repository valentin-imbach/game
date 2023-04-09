
#include "Console.hpp"
#include "Game.hpp"
#include "pathfinding.hpp"

Console::Console(Game* game)
	: game(game) {}

void Console::draw() {
	if (!active) return;
	pair windowSize = Window::instance->size;
	TextureManager::drawRect({20, windowSize.y / 2 + 10}, {300, windowSize.y / 2 - 30}, {0, 0, 0, 100}, false, true);
	for (int i = 0; i < history.size(); i++) {
		pair position(40, windowSize.y - 60 - (history.size() - i) * 30);
		TextManager::drawText(history[i], position);
	}
	std::string indexText = "-";
	pair position(25, windowSize.y - 60 - (history.size() - index) * 30);
	TextManager::drawText(indexText, position);
	TextManager::drawText(input, {40, windowSize.y - 60});
}

bool Console::handleEvent(SDL_Event event) {
	if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_TAB) {
		active = !active;
		return true;
	}
	if (!active) return false;
	if (event.type == SDL_TEXTINPUT) {
		if (input.size() < 22) {
			input += event.text.text;
			index = history.size();
		}
	} else if (event.type == SDL_KEYDOWN) {
		switch (event.key.keysym.scancode) {
		case SDL_SCANCODE_BACKSPACE:
			if (input.length() > 0) {
				input.pop_back();
				index = history.size();
			}
			break;
		case SDL_SCANCODE_ESCAPE:
			active = false;
			break;
		case SDL_SCANCODE_RETURN:
			if (execute(input)) {
				history.push_back(input);
				input.clear();
				index = history.size();
			}
			break;
		case SDL_SCANCODE_UP:
			index = std::max(index, 1UL) - 1;
			if (index < history.size()) input = history[index];
			break;
		case SDL_SCANCODE_DOWN:
			index = std::min(index + 1, history.size());
			if (index == history.size()) input.clear();
			if (index < history.size()) input = history[index];
			break;
		default:
			return true;
		}
	}
	return true;
}

bool Console::execute(std::string input) {
	if (!game->world) return false;
	ECS& ecs = game->world->ecs;
	Entity player = game->world->player;
	Entity camera = game->world->camera;
	std::vector<std::string> inputs = string_split(input);

	if (inputs[0] == "clear") {
		history.clear();
	} else if (inputs[0] == "refresh") {
		Sprite::loadSpriteSheets();
		ItemPropertyTemplate::setTemplates();
		ItemKindTemplate::setTemplates();
		ItemTemplate::setTemplates();
		ResourceTemplate::setTemplates();
	} else if (inputs[0] == "kill") {
		if (!player) return false;
		ecs.addComponent<DeathComponent>({}, player);
	} else if (inputs[0] == "god") {
		// MessageManager::notify(ToggleGodMessage());
	} else if (inputs[0] == "tp") {
		if (inputs.size() < 3) return false;
		if (!player) return false;
		int x = std::stoi(inputs[1]);
		int y = std::stoi(inputs[2]);
		ecs.getComponent<PositionComponent>(player).position = vec(x, y);
	} else if (inputs[0] == "place") {
		if (inputs.size() < 2) return false;
		ResourceId::value resourceId = ResourceId::from_string(inputs[1]);
		if (!player || !resourceId) return false;
		pair position = round(ecs.getComponent<PositionComponent>(player).position);
		Entity resource = EntityFactory::createResource(resourceId, position);
		game->world->link(resource);
	} else if (inputs[0] == "cows") {
		if (inputs.size() < 2 || !player) return false;
		int n = std::stoi(inputs[1]);
		vec position = ecs.getComponent<PositionComponent>(player).position;
		for (int i = 0; i < n; i++) EntityFactory::createAnimal(AnimalId::COW, position);
	} else if (inputs[0] == "give") {
		if (inputs.size() < 2 || !player) return false;
		ItemId::value itemId = ItemId::from_string(inputs[1]);
		if (!itemId) return false;
		uint count = 1;
		if (inputs.size() > 2) {
			// if (!isUInt(inputs[2])) return false;
			count = std::stoi(inputs[2]);
		}
		while (count > 0) {
			int batch = std::min(count, MAX_STACK);
			count -= batch;
			Entity item = EntityFactory::createItem(itemId, batch);
			Entity rest = ecs.getComponent<InventoryComponent>(player).inventory.add(item);
			if (rest) ecs.destroyEntity(rest);
		}
	} else if (inputs[0] == "empty") {
		if (!player) return false;
		ecs.getComponent<InventoryComponent>(player).inventory.clear(true);
	} else if (inputs[0] == "tile") {
		if (inputs.size() != 2 || !game->world || !player) return false;
		TileId::value tileId = TileId::from_string(inputs[1]);
		if (!tileId) return false;
		pair position = round(ecs.getComponent<PositionComponent>(player).position);
		game->world->realm->map->tiles[position.x][position.y]->tileId = tileId;
		game->world->realm->map->updateStyle(position, true);
	} else if (inputs[0] == "weather") {
		// if (split.size() != 2) return false;
		// WeatherType w = WeatherTypeFromString(split[1]);
		// MessageManager::notify(SetWeatherMessage(w));
	} else if (inputs[0] == "zoom") {
		if (inputs.size() != 2 || !camera) return false;
		int zoom = std::stoi(inputs[1]);
		ecs.getComponent<CameraComponent>(camera).zoom = zoom;
	} else if (inputs[0] == "gui") {
		if (inputs.size() != 2) return false;
		uint scale = std::stoi(inputs[1]);
		GuiManager::scale = scale;
	} else if (inputs[0] == "tools") {
		// vup(Item) tools = LootTable::tools();
		// for (unsigned int i = 0; i < tools.size(); i++) MessageManager::notify(GiveMessage(std::move(tools[i])));
	} else if (inputs[0] == "save") {
		std::string path = "../saves/" + inputs[1] + ".binary";
		std::fstream file = std::fstream(path, std::ios::out | std::ios::binary);
		if (!file) {
			ERROR("No file");
			return true;
		}
		game->world->serialise(file);
		file.close();
		LOG("World saved");
	} else if (inputs[0] == "load") {
		std::string path = "../saves/" + inputs[1] + ".binary";
		std::fstream file = std::fstream(path, std::ios::in | std::ios::binary);
		if (!file) {
			ERROR("No file");
			return true;
		}
		game->world = std::make_unique<World>(file);
		file.close();
		LOG("World loaded");
	} else if (inputs[0] == "test") {
		if (!player) return false;
		vec position = ecs.getComponent<PositionComponent>(player).position;
		for (int i = 0; i < 10; i++) game->world->particleSystem.emit(ParticleSystem::DIRT, position);
	} else if (inputs[0] == "state") {
		game->gameState = GameState::from_string(inputs[1]);
	} else {
		return false;
	}

	return true;
}