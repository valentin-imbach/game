//
//  MainMenu.cpp
//  Game
//
//  Created by Valentin Imbach on 28.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#include "MainMenu.hpp"
#include "Game.hpp"

void MainMenu::buttonPressed() {
    Game::createWorld();
}

MainMenu::MainMenu() {
    gui = new Widget({0,0}, {600,400}, TextureManager::getTexture("mm.png"));
    gui -> addGuiElement(new Button({0,0},{200,50}, &MainMenu::buttonPressed, TextureManager::getTexture("start.png")));
    gui -> addGuiElement(new TextField({0,100},{200,50}, TextureManager::getTexture("name.png")));
}

bool MainMenu::handleEvent(SDL_Event event) {
    return gui -> handleEvent(event);
}

void MainMenu::render() {
    gui -> update();
    gui -> render();
}
