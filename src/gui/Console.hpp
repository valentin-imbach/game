
#pragma once
#include "utils.hpp"
#include "Events.hpp"
#include "Command.hpp"
#include "ECS_types.hpp"

class Game;
class World;
class ECS;
class Camera;

class Console {
public:
	Console(Game* game);

	void draw();
	bool handleEvent(InputEvent event);
	bool active = false;

private:
	Game* game;
	World* world;
	ECS* ecs;
	Camera* camera;
	Entity player;

	std::string input;
	std::vector<std::string> history;
	uint index = 0;
	void setCommands();

	std::unordered_map<std::string, std::unique_ptr<ICommand>> commands;

	bool execute(std::string input);
};