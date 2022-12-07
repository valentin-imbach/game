
#pragma once
#include "World.hpp"

#define SAMPLE_SIZE 50

class Game {
public:
	Game();

	void update();
	void handleEvents();
	void limitFrameRate(int fps);

	bool running;

	uint lastFrameTicks;
	uint dt;

	std::queue<uint> sample;
	uint sampleSum;
	int framesPerSecond;

	std::unique_ptr<World> world;
};