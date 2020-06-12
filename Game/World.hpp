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

class World : public Serializable {
private:
    LayerManager layerManager;
public:
    World(std::string n);
    void update();
    void render();
    bool handleEvent(SDL_Event event);
    
    void serialize(std::fstream& stream) override;
    void deserialize(std::fstream& stream) override;
    
    std::string name;
    
    GuiLayer guiLayer;
    EntityLayer entityLayer;
    DebugLayer debugLayer;
};
