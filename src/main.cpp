
#include "Game.hpp"
#include "Window.hpp"

#define MAX_FPS 120

int main() {
	std::srand(time(NULL));
	Window window("Title", {1024, 640}, false);
	Game game;

	while (game.gameState) {
		game.handleEvents();
		game.update();
		game.draw();
		game.limitFrameRate(MAX_FPS);
	}

	return 0;
}