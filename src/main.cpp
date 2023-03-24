
#include "Game.hpp"
#include "Window.hpp"

int main() {
	Window window("Title", {1024, 640});
	Game game;

	while (game.gameState) {
		game.handleEvents();
		game.update();
		game.draw();
		game.limitFrameRate(60);
	}

	return 0;
}