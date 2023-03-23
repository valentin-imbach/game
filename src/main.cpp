
#include "Game.hpp"
#include "Window.hpp"

int main() {
	Window window("Title", {1024, 640});
	Game game;

	while (game.gameState) {
		game.handleEvents();
		if (game.gameState == GameState::RUNNING) game.update();
		if (game.gameState == GameState::PAUSED || game.gameState == GameState::RUNNING) game.draw();
		game.limitFrameRate(60);
	}

	return 0;
}