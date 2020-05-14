//
//  Game.hpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-20.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#pragma once
#include "LayerSystem/Layers.h"
#include "Gui/GuiManager.hpp"

enum GameState {
    RUNNING,
    PAUSED,
    LOADING,
    MENU
};

class Game {
public:
    static void Init();
    static void render();
    static void update();
    static void handleEvents();
    static GameState state;
    static EntityLayer* entityLayer;
    static DebugLayer* debugLayer;
    static GuiManager guiManager;
};
