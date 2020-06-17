//
//  Console.cpp
//  Game
//
//  Created by Valentin Imbach on 26.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#include "Console.hpp"
#include "TextManager.hpp"
#include "TextureManager.hpp"
#include "Window.hpp"
#include "Game.hpp"

#include "EntityFactory.hpp"

void Console::render() {
    if (!active) { return; }
    TextureManager::drawTexture(TextureManager::getTexture("grey.png"), 20, Window::size.Y-500, 400, 480);
    for (int i = 0; i < memory; i++) {
        if (i >= history.size()) { break; }
        if (i == memIt) {
            TextManager::drawText("- "+history[history.size()-1-i], {30,Window::size.Y-60-30*(i+1)}, false, {220,220,220});
        } else {
            TextManager::drawText("$ "+history[history.size()-1-i], {30,Window::size.Y-60-30*(i+1)}, false, {160,160,160});
        }
    }
    TextManager::drawText("$ "+text, {30,Window::size.Y-60});
}

bool Console::handleEvent(SDL_Event event) {
    if (event.key.repeat) { return false; }
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.scancode == SDL_SCANCODE_TAB) {
            active = !active;
            return true;
        }
    }
    if (active) {
        if (event.type == SDL_TEXTINPUT) {
            text += event.text.text;
            memIt = -1;
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE) {
            if (text.length() > 0) {
                text = text.substr(0,text.length()-1);
            }
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
            active = false;
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
            history.push_back(text);
            execute(text);
            memIt = -1;
            text.clear();
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_UP) {
            if (history.size() > memIt+1) {
                memIt += 1;
                text = history[history.size()-memIt-1];
            }
        }
        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
            if (memIt > 0) {
                memIt -= 1;
                text = history[history.size()-memIt-1];
            } else {
                memIt = -1;
                text.clear();
            }
        }
        return true;
    }
    return false;
}

bool is_number(const std::string &s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}


bool Console::execute(std::string s) {
    EntityManager* manager = &(Game::world -> entityLayer.entityManager);
    Entity* player = Game::world -> entityLayer.player;
    LOG("Executed command",s);
    if (s == "quit") {
        Window::running = false;
    } else if (s == "clear") {
        history.clear();
    } else if (s == "refresh") {
        TextureManager::refresh();
    }
    if (Game::world != nullptr) {
        if (s == "kill") {
            player -> getComponent<HealthComponent>() -> health = 0;
        }
        if (s == "god") {
            player -> getComponent<PlayerInputComponent>() -> god = !(player -> getComponent<PlayerInputComponent>() -> god);
        }
        
        if (s.substr(0,5) == "place") {
            int n = std::stoi(s.substr(6));
            if (0 <= n) {
                EntityFactory::createEntity(manager, n, (player -> getComponent<PositionComponent>() -> position - pair<float>(0,1.5)).rounded());
            }
        }
    }
    return true;
}
