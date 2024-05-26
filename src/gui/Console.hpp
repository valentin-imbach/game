
#pragma once
#include "utils.hpp"
#include "Events.hpp"

// #include "Command.hpp"

class Game;

class Console {
public:
	Console(Game* game);

	void draw();
	bool handleEvent(InputEvent event);
	bool active = false;

private:
	Game* game;
	std::string input;
	std::vector<std::string> history;
	uint index = 0;

	// std::unordered_map<std::string, Command> commands;

	bool execute(std::string input);
};