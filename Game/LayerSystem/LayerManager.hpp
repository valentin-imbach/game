//
//  LayerManager.hpp
//  Game
//
//  Created by Valentin Imbach on 23.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "tools.h"
#include "../serializer.h"

struct Layer : public Serializable {
    static int entityCount;
    bool active = true;
    virtual void update() {};
    virtual void render() {};
    virtual bool handleEvent(SDL_Event event) { return false; };
};

class LayerManager : public Serializable {
private:
    v(Layer*) layerStack;
public:
    LayerManager();
    Layer* addLayer(Layer* l);
    void update();
    void render();
    bool handleEvent(SDL_Event event);
    
    void serialize(std::fstream& stream) override;
    void deserialize(std::fstream& stream) override;
};

