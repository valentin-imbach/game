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
    gui = new Widget(Window::size/2, {600,400}, TextureManager::getTexture("grey.png"));
    gui -> addGuiElement(new Button({300,150},{200,50},&MainMenu::buttonPressed,TextureManager::getTexture("grey.png")));
    gui -> addGuiElement(new TextField({300,250},{200,50},TextureManager::getTexture("grey.png")));
}

bool MainMenu::handleEvent(SDL_Event event) {
    return gui -> handleEvent(event);
}

void MainMenu::render() {
    gui -> render();
}
