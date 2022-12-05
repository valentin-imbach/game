
#include "Window.hpp"
#include "Game.hpp"

int main() {
    Window window("Title", {1024, 640});
    Game game;

    while (game.running) {
        game.handleEvents();
        game.update();
    }

    return 0;
}