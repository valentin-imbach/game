
#pragma once
#include "Console.hpp"
#include "DebugScreen.hpp"
#include "GuiElement.hpp"
#include "SoundManager.hpp"

#define SAMPLE_SIZE 50

ENUM(GameState,
LOADING,
MENU,
RUNNING,
PAUSED)

class Game {
public:
	Game();

	TrackMix trackMix;

	void update();
	void draw();
	void handleEvents();
	void limitFrameRate(int fps);

	GameState::value gameState;
	Console console;
	DebugScreen debugScreen;
	std::unique_ptr<World> world;

private:
	uint lastFrameTicks;
	uint dt = 0;
	std::queue<uint> sample;
	uint sampleSum;
	int framesPerSecond;

	void buildMenu();
	
	std::unique_ptr<Widget> mainMenu;
	std::unique_ptr<Widget> pauseMenu;

	void createButton();
	void createWorld(std::string name, uint seed);
	void removeWorld(std::string name);
	void loadWorld(std::string name);
	void saveWorld();

};