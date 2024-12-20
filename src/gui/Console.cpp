
#include "Console.hpp"
#include "Game.hpp"

Console::Console(Game* game) : game(game) {
	setCommands();
}

void Console::update(uint ticks) {
	this->ticks = ticks;
}

void Console::draw() {
	if (!active) return;
	vec windowSize = Window::instance->size;
	TextureManager::drawRect({20, windowSize.y / 2 + 10}, {300, windowSize.y / 2 - 30}, {0, 0, 0, 100}, false, true);
	for (uint i = 0; i < history.size(); i++) {
		pair position(40, windowSize.y - 60 - (history.size() - i) * 30);
		TextManager::drawText(history[i], position);
	}
	std::string indexText = "-";
	pair position(25, windowSize.y - 60 - (history.size() - index) * 30);
	TextManager::drawText(indexText, position);
	TextManager::drawText(input, pair(40, windowSize.y - 60));
}

bool Console::handleEvent(InputEvent event) {
	if (event.id == InputEventId::CONSOLE) {
		active = !active;
		return true;
	}
	if (!active) return false;
	if (event.id == InputEventId::TEXT) {
		if (input.size() < 22) {
			input += event.text;
			index = history.size();
		}
	} else if (event.id == InputEventId::BACKSPACE) {
		if (input.length() > 0) {
			input.pop_back();
			index = history.size();
			return true;
		}
	} else if (event.id == InputEventId::ESCAPE) {
		active = false;
		return true;
	} else if (event.id == InputEventId::RETURN) {
		if (execute(input)) {
			history.push_back(input);
			input.clear();
			index = history.size();
			return true;
		}
		return false;
	} else if (event.id == InputEventId::UP) {
		index = std::max(index, 1u) - 1;
		if (index < history.size()) {
			input = history[index];
			return true;
		}
	} else if (event.id == InputEventId::DOWN) {
		index = std::min(index + 1, uint(history.size()));
		if (index == history.size()) {
			input.clear();
			return true;
		} else if (index < history.size()) {
			input = history[index];
			return true;
		}
	}
	return true;
}

bool Console::execute(std::string input) {
	std::vector<std::string> inputs = string::split(input);

	if (inputs.empty()) return false;
	std::string com = inputs.front();

	auto it = commands.find(com);
	if (it == commands.end()) {
		WARNING("Command", com, "not found");
		return false;
	}

	world = game->world ? game->world.get() : nullptr;
	ecs = game->world ? &game->world->ecs : nullptr;
	player = game->world ? game->world->player : 0;
	camera = game->world ? &game->world->camera : nullptr;
	std::vector<std::string> args(inputs.begin() + 1, inputs.end());
	(*it).second->execute(args);
	return true;
}