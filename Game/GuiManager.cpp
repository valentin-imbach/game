//
//  GuiManager.cpp
//  Game
//
//  Created by Valentin Imbach on 21.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#include "GuiManager.hpp"

GuiComponent* GuiManager::activeGui = nullptr;

void GuiManager::update() {
    if (activeGui != nullptr) {
        activeGui -> update();
        activeGui -> render();
    }
}
