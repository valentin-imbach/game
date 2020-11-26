//
//  CommandSystem.hpp
//  Game
//
//  Created by Valentin Imbach on 26/11/2020.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "Components.h"

struct Command {
    virtual void execute(Entity* target) = 0;
    virtual ~Command() {}
};

struct BreakCommand : public Command {
    void execute(Entity* target) override {
        if (target -> hasComponent<ResourceComponent>()) {
            target -> getComponent<ResourceComponent>() -> onBreak();
        }
    }
};
