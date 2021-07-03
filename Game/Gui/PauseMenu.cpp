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
    gui -> addGuiElement(new TextElement({0,30}, "Game Paused", true, Alignment::NORTH));
    
    SDL_Texture* left = TextureManager::getTexture("left.png");
    SDL_Texture* left2 = TextureManager::getTexture("left2.png");
    SDL_Texture* right = TextureManager::getTexture("right.png");
    SDL_Texture* right2 = TextureManager::getTexture("right2.png");
    
    gui -> addGuiElement(new TextElement({-250,-120}, "Camera Zoom:"));
    gui -> addGuiElement(new DisplayElement({0,-105}, &Camera::ZOOM));
    gui -> addGuiElement(new Button({-50,-105}, {28,36}, &PauseMenu::zoomLeft, left, left2));
    gui -> addGuiElement(new Button({50,-105}, {28,36}, &PauseMenu::zoomRight, right, right2));
    
    gui -> addGuiElement(new TextElement({-250,-60}, "Max FPS:"));
    gui -> addGuiElement(new DisplayElement({0,-45}, &Window::MAX_FPS));
    gui -> addGuiElement(new Button({-50,-45}, {28,36}, &PauseMenu::fpsLeft, left, left2));
    gui -> addGuiElement(new Button({50,-45}, {28,36}, &PauseMenu::fpsRight, right, right2));
    
    gui -> addGuiElement(new TextElement({-250,0}, "Music Volume:"));
    gui -> addGuiElement(new DisplayElement({0,15}, &SoundManager::volume));
    gui -> addGuiElement(new Button({-50,15}, {28,36}, &PauseMenu::musicLeft, left, left2));
    gui -> addGuiElement(new Button({50,15}, {28,36}, &PauseMenu::musicRight, right, right2));
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

void PauseMenu::musicLeft() { SoundManager::setVolume(SoundManager::volume - 8); }
void PauseMenu::musicRight() { SoundManager::setVolume(SoundManager::volume + 8); }
