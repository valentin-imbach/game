//
//  HealthComponent.h
//  Game
//
//  Created by Valentin Imbach on 19.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "Components.h"

class HealthComponent : public Component {
public:
    static ComponentType componentType;
    int maxHealth;
    int health;

    HealthComponent(int max = 20) {
        maxHealth = max;
        health = max;
    }
    
    SERIALIZE(maxHealth,health);
    
    Component* create() override {
        return new HealthComponent();
    }
    
};
