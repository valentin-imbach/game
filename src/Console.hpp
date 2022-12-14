
#pragma once
#include <vector>
#include "Components.hpp"
#include "EntityFactory.hpp"
#include "Events.hpp"
#include "SDL2/SDL_events.h"
#include "TextManager.hpp"
#include "Window.hpp"
#include "utils.hpp"
#include "TextureManager.hpp"
#include "World.hpp"

class Console {
public:
	void update(World* w) {
		world = w;
	}

	void draw() {
		if (!active) return;
		pair windowSize = Window::instance->size;
		TextureManager::drawRect({20, windowSize.y / 2}, {300, windowSize.y / 2 - 20}, {0, 0, 0, 100}, false, true);
		for (int i = 0; i < history.size(); i++) {
			TextManager::drawText(history[i], {40, windowSize.y - 60 - (history.size() - i) * 30});
		}
		std::string indexText = "-";
		TextManager::drawText(indexText, {25, windowSize.y - 60 - (history.size() - index) * 30});
		TextManager::drawText(input, {40, windowSize.y - 60});
	}

	bool handleEvent(SDL_Event event) {
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

	bool active = false;

private:
	std::string input;
	std::vector<std::string> history;
	size_t index = 0;
	World* world = nullptr;

	bool execute(std::string input) {
		ECS& ecs = world->ecs;
		Entity player = world->player;
		std::vector<std::string> inputs = string_split(input);

		if (inputs[0] == "clear") {
			history.clear();
		} else if (inputs[0] == "refresh") {
			//TextureManager::Init();
		} else if (inputs[0] == "kill") {
			if (!player) return false;
			ecs.getComponent<HealthComponent>(player).health = 0;
		} else if (inputs[0] == "god") {
			//MessageManager::notify(ToggleGodMessage());
		} else if (inputs[0] == "tp") {
			if (inputs.size() < 3) return false;
			if (!player) return false;
			int x = std::stoi(inputs[1]);
			int y = std::stoi(inputs[2]);
			ecs.getComponent<PositionComponent>(player).position = {x, y};
		} else if (inputs[0] == "place") {
			// if (split.size() < 2) return false;
			// int n = std::stoi(split[1]);
			// if (n < 0) return false;
			// MessageManager::notify(PlaceMessage(n, playerRealm, playerPosition.rounded()));
		} else if (inputs[0] == "cows") {
			if (inputs.size() < 2) return false;
			if (!player) return false;
			int n = std::stoi(inputs[1]);
			vec position = ecs.getComponent<PositionComponent>(player).position;
			for (int i = 0; i < n; i++) EntityFactory::createAnimal(AnimalId::COW, position);
		} else if (inputs[0] == "give") {
			// if (inputs.size() == 1) return false;
			// ItemId itemID = ItemIdFromString(split[1]);
			// if (itemID == ItemId::NONE) return false;
			// int num = 1;
			// if (split.size() > 2) {
			// 	if (!isUInt(split[2])) return false;
			// 	num = std::stoi(split[2]);
			// }
			// MessageManager::notify(GiveMessage(std::make_unique<Item>(itemID, num)));
		} else if (inputs[0] == "tile") {
			// if (inputs.size() != 2) return false;
			// TileId t = TileIdFromString(split[1]);
			// MessageManager::notify(TileMessage(t, playerRealm, playerPosition.rounded()));
		} else if (inputs[0] == "weather") {
			// if (split.size() != 2) return false;
			// WeatherType w = WeatherTypeFromString(split[1]);
			// MessageManager::notify(SetWeatherMessage(w));
		} else if (inputs[0] == "tools") {
			// vup(Item) tools = LootTable::tools();
			// for (unsigned int i = 0; i < tools.size(); i++) MessageManager::notify(GiveMessage(std::move(tools[i])));
		} else {
			return false;
		}

		return true;
	}
};