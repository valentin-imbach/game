//
//  GuiComponent.h
//  Game
//
//  Created by Valentin Imbach on 21.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "Components.h"

class GuiComponent : public Component {
public:
    virtual void guiUpdate() {};
    virtual void guiRender() = 0;
    virtual bool guiHandleEvent(SDL_Event event) = 0;
    pair<int> position;
    pair<int> size;
};
