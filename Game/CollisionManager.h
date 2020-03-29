//
//  CollisionManager.h
//  Game
//
//  Created by Valentin Imbach on 28.03.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once

struct Collider {
    
    float x = 0, y = 0, w = 1, h = 1;
    bool active = true;
    
    Collider() {}
    Collider(float x_, float y_, float w_, float h_) {
        x = x_;
        y = y_;
        w = w_;
        h = h_;
        active = true;
    }
};

struct CollisionManager {
    static bool AABB(Collider A, Collider B) {
        if (!A.active || !B.active) { return false; }
        return (A.x < B.x + B.w && B.x < A.x + A.w && A.y < B.y + B.h && B.y < A.y + A.h);
    }
};

