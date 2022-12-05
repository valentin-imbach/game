
#include "Game.hpp"
#include "Window.hpp"

int main() {
	Window window("Title", {1024, 640});
	Game game;

	while(game.running) {
	    game.handleEvents();
		game.update();
		game.limitFrameRate(30);
	}

	return 0;
}