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

void Console::render() {
    if (!active) return;
    TextureManager::drawTexture(TextureManager::getTexture("grey.png"), 20, Window::size.Y-500, 400, 480);
    for (int i = 0; i < memory; i++) {
        if (i >= history.size()) break;
        if (i == memIt) {
            std::string s = "- "+history[history.size()-1-i];
            TextManager::drawText(s, {30,Window::size.Y-60-30*(i+1)}, false, {220,220,220});
        } else {
            std::string s = "$ "+history[history.size()-1-i];
            TextManager::drawText(s, {30,Window::size.Y-60-30*(i+1)}, false, {160,160,160});
        }
    }
    std::string s = "$ "+text;
    TextManager::drawText(s, {30,Window::size.Y-60});
}

void Console::write(std::string text) {
    history.push_back(text);
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
        } else if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_BACKSPACE) {
            if (text.length() > 0) text = text.substr(0,text.length()-1);
        } else if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
            active = false;
        } else if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_RETURN) {
            history.push_back(text);
            execute(text);
            memIt = -1;
            text.clear();
        } else if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_UP) {
            if (history.size() > memIt+1) {
                memIt += 1;
                text = history[history.size()-memIt-1];
            }
        } else if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_DOWN) {
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

bool Console::onMessage(const Message& message) {
    if (message.type == MessageType::PRINT) {
        const PrintMessage &msg = static_cast<const PrintMessage&>(message);
        write(msg.text);
        return true;
    }
    if (message.type == MessageType::ITEM_PICKUP) {
        const PickupMessage &msg = static_cast<const PickupMessage&>(message);
        std::string s = "Picked up "+std::to_string(msg.item -> count)+" item(s) of type " + std::to_string((int)(msg.item -> itemID));
        write(s);
        return true;
    }
    
    return false;
}

bool Console::execute(std::string s) {
    v(std::string) split = splitString(s);
    if (split[0] == "quit") MessageManager::notify(QuitMessage());
    else if (split[0] == "clear") history.clear();
    else if (split[0] == "refresh") MessageManager::notify(RefreshMessage());
    else if (split[0] == "kill") MessageManager::notify(KillPlayerMessage());
    else if (split[0] == "god") MessageManager::notify(ToggleGodMessage());
    else if (split[0] == "tp") {
        if (split.size() < 3) return false;
        int x = std::stoi(split[1]);
        int y = std::stoi(split[2]);
        MessageManager::notify(TeleportMessage({x,y}));
    } else if (split[0] == "place") {
        if (split.size() < 2) return false;
        int n = std::stoi(split[1]);
        if (n < 0) return false;
        MessageManager::notify(PlaceMessage(n));
    } else if (split[0] == "give") {
        if (split.size() < 2) return false;
        int t = std::stoi(split[1]);
        int n = 0;
        if (split.size() > 2) n = std::stoi(split[2]);
        if (n < 0 || t < 0) return false;
        MessageManager::notify(GiveMessage(new ItemStack((ItemID)t, n)));
    } else return false;
    return true;
}
