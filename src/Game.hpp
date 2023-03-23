
#pragma once
#include "Console.hpp"
#include "DebugScreen.hpp"

#define SAMPLE_SIZE 50

ENUM(GameState,
LOADING,
MENU,
RUNNING,
PAUSED)

class Game {
public:
	Game();

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
};