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

class World : public Serializable {
public:
    World(std::string n);
    void update();
    void render();
    bool handleEvent(SDL_Event event);
    
    void populate();
    
    void serialize(std::fstream& stream) override;
    void deserialize(std::fstream& stream) override;
    
    std::string name;
    
    MapLayer mapLayer;
    GuiLayer guiLayer;
    EntityLayer entityLayer;
    DebugLayer debugLayer;
    CameraLayer cameraLayer;
private:
    LayerManager layerManager;
};
