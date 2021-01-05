//
//  CollisionManager.h
//  Game
//
//  Created by Valentin Imbach on 28.03.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once

struct Collider {
    pair<float> position;
    pair<float> size = {1,1};
    bool active = true;
};

struct CollisionManager {
    static bool AABB(Collider A, Collider B) {
        if (!A.active || !B.active) return false;
        return (A.position.X < B.position.X + B.size.X && B.position.X < A.position.X + A.size.X && A.position.Y < B.position.Y + B.size.Y && B.position.Y < A.position.Y + A.size.Y);
    }
};

