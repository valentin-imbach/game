
#pragma once
#include "Console.hpp"
#include "DebugScreen.hpp"
#include "GuiElement.hpp"
#include "SoundManager.hpp"

ENUM(GameState,
LOADING,
MENU,
RUNNING,
PAUSED)

class Game {
public:
	Game();
	~Game();

	TrackMix trackMix;

	void update(uint dt);
	void draw();
	void pollEvents(uint dt);

	GameState::value gameState;
	Console console;
	DebugScreen debugScreen;
	std::unique_ptr<World> world;

	int framesPerSecond;
	int updatesPerSecond;

private:
	bool handleEvent(InputEvent event, uint dt);
	TextField* nameField = nullptr;
	TextField* seedField = nullptr;
	void buildMenu();
	
	std::unique_ptr<Widget> mainMenu;
	std::unique_ptr<Widget> pauseMenu;

	void createButton();
	void createWorld(std::string name, uint seed);
	void removeWorld(std::string name);
	void loadWorld(std::string name);
	void saveWorld();

};