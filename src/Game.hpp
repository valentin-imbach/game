
#pragma once
#include <memory>

#include "World.hpp"

class Game {
public:
	Game();

	void update();
	void handleEvents();
	void limitFrameRate(int fps);

	bool running;
	uint lastFrameTicks;
	int framesPerSecond;
	uint lastSecondTicks;
	int frameCounter;

	std::unique_ptr<World> world;
};