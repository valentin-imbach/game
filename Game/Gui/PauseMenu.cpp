//
//  PauseMenu.cpp
//  Game
//
//  Created by Valentin Imbach on 26.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#include "PauseMenu.hpp"
#include "../Camera.hpp"
#include "../SoundManager.hpp"

PauseMenu::PauseMenu() {
    gui = new Widget(Window::size/2, {600,400}, TextureManager::getTexture("grey.png"));
    gui -> addGuiElement(new TextElement({300,30}, "Game Paused", true));
    
    SDL_Texture* left = TextureManager::getTexture("left.png");
    SDL_Texture* left2 = TextureManager::getTexture("left2.png");
    SDL_Texture* right = TextureManager::getTexture("right.png");
    SDL_Texture* right2 = TextureManager::getTexture("right2.png");
    
    gui -> addGuiElement(new TextElement({50,80}, "Camera Zoom:"));
    gui -> addGuiElement(new DisplayElement({300,80}, &Camera::ZOOM));
    gui -> addGuiElement(new Button({250,95}, {28,36}, &PauseMenu::zoomLeft, left, left2));
    gui -> addGuiElement(new Button({350,95}, {28,36}, &PauseMenu::zoomRight, right, right2));
    
    gui -> addGuiElement(new TextElement({50,140}, "Max FPS:"));
    gui -> addGuiElement(new DisplayElement({300,140}, &Window::MAX_FPS));
    gui -> addGuiElement(new Button({250,155}, {28,36}, &PauseMenu::fpsLeft, left, left2));
    gui -> addGuiElement(new Button({350,155}, {28,36}, &PauseMenu::fpsRight, right, right2));
    
    gui -> addGuiElement(new TextElement({50,200}, "Music Volume:"));
    gui -> addGuiElement(new DisplayElement({300,200}, &SoundManager::volume));
    gui -> addGuiElement(new Button({250,215}, {28,36}, &PauseMenu::musicLeft, left, left2));
    gui -> addGuiElement(new Button({350,215}, {28,36}, &PauseMenu::musicRight, right, right2));
}

bool PauseMenu::handleEvent(SDL_Event event) {
    return gui -> handleEvent(event);
}

void PauseMenu::render() {
    gui -> recursiveRender();
}

void PauseMenu::zoomLeft() { Camera::ZOOM -= 8; }
void PauseMenu::zoomRight() { Camera::ZOOM += 8; }

void PauseMenu::fpsLeft() { Window::MAX_FPS -= 5; }
void PauseMenu::fpsRight() { Window::MAX_FPS += 5; }

void PauseMenu::musicLeft() { SoundManager::setVolume(SoundManager::volume - 8); }
void PauseMenu::musicRight() { SoundManager::setVolume(SoundManager::volume + 8); }
