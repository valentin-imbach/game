//
//  GuiManager.hpp
//  Game
//
//  Created by Valentin Imbach on 21.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "Components.h"

struct GuiManager {
    static GuiComponent *activeGui;
    static void update();
    static void handleEvents();
};
