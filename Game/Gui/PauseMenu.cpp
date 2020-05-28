//
//  PauseMenu.cpp
//  Game
//
//  Created by Valentin Imbach on 26.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#include "PauseMenu.hpp"
#include "../Camera.hpp"

PauseMenu::PauseMenu() {
    gui = new Widget(Window::size/2, {600,400}, TextureManager::loadTexture("pauseMenu.png"));
    gui -> addGuiElement(new TextElement({300,30}, "Game Paused", true));
    
    gui -> addGuiElement(new TextElement({50,80}, "Camera Zoom:"));
    gui -> addGuiElement(new DisplayElement({300,80}, &Camera::ZOOM));
    gui -> addGuiElement(new Button<PauseMenu>({250,95}, {36,36}, this, &PauseMenu::zoomLeft, TextureManager::loadTexture("left.png")));
    gui -> addGuiElement(new Button<PauseMenu>({350,95}, {36,36}, this, &PauseMenu::zoomRight, TextureManager::loadTexture("right.png")));
    
    gui -> addGuiElement(new TextElement({50,140}, "Max FPS:"));
    gui -> addGuiElement(new DisplayElement({300,140}, &Window::MAX_FPS));
    gui -> addGuiElement(new Button<PauseMenu>({250,155}, {36,36}, this, &PauseMenu::fpsLeft, TextureManager::loadTexture("left.png")));
    gui -> addGuiElement(new Button<PauseMenu>({350,155}, {36,36}, this, &PauseMenu::fpsRight, TextureManager::loadTexture("right.png")));
}

bool PauseMenu::handleEvent(SDL_Event event) {
    return gui -> handleEvent(event);
}

void PauseMenu::render() {
    gui -> render();
}

void PauseMenu::zoomLeft() { Camera::ZOOM -= 8; }
void PauseMenu::zoomRight() { Camera::ZOOM += 8; }

void PauseMenu::fpsLeft() { Window::MAX_FPS -= 5; }
void PauseMenu::fpsRight() { Window::MAX_FPS += 5; }
