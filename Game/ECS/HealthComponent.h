//
//  HealthComponent.h
//  Game
//
//  Created by Valentin Imbach on 19.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once

class HealthComponent : public Component {
public:
    int maxHealth;
    int health;
    HealthComponent(Entity* entity, int max) {
        maxHealth = max;
        health = max;
    }
};
