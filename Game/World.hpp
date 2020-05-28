//
//  World.hpp
//  Game
//
//  Created by Valentin Imbach on 28.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "tools.h"
#include "LayerSystem/Layers.h"
#include "Gui/PauseMenu.hpp"
#include "Console.hpp"

class World {
private:
    LayerManager layerManager;
public:
    World();
    void update();
    void render();
    bool handleEvent(SDL_Event event);
    
    GuiManager guiManager;
    
    EntityLayer* entityLayer;
    DebugLayer* debugLayer;
    
    Console* console;
};
