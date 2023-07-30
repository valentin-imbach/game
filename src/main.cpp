
#include "Game.hpp"
#include "Window.hpp"

int main() {
	std::srand(time(NULL));
	Window window("Title", {1024, 640}, false);
	Game game;

	const uint targetFPS = 60;
	const uint targetUPS = 60;
    const uint targetFrameTime = 1000 / targetFPS;
    const uint targetUpdateTime = 1000 / targetUPS;

	uint previousTicks = SDL_GetTicks();
	uint lastRender = previousTicks;

	const uchar sampleSize = 50;
	std::array<pair, sampleSize> samples = {};
	uchar index = 0;
	pair sampleSum(0, 0);

	while (game.gameState) {
		uint currentTicks = SDL_GetTicks();
        uint dt = currentTicks - previousTicks;
        previousTicks = currentTicks;

		if (dt < targetUpdateTime) {
            SDL_Delay(targetUpdateTime - dt);
			dt = targetUpdateTime;
        }

		game.pollEvents(dt);
		game.update(dt);
		
		bool render = false;
		if (currentTicks - lastRender > targetFrameTime) {
			game.draw();
			lastRender = currentTicks;
			render = true;
		}

		pair stats(dt, render);
		sampleSum -= samples[index];
		samples[index] = stats;
		sampleSum += stats;
		index = (index + 1) % sampleSize;

		game.updatesPerSecond = (sampleSize * 1000.0f) / sampleSum.x;
		game.framesPerSecond = (sampleSum.y * 1000.0f) / sampleSum.x;
	}

	window.cleanup();
	SDL_Quit();
	return 0;
}