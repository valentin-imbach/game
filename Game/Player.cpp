//
//  Player.cpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-25.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#include "Player.hpp"

Player::Player(float x, float y, int d) {
    pos = {x,y};
    dir = d;
    Camera::pos = {pos.X*ZOOM,pos.Y*ZOOM};
    walkTicks = 0;
    speed = 1.3/16;
    inv = Inventory(9,4);
    inv.slots[0][0] = new ItemStack(0,1);
    inv.slots[1][0] = new ItemStack(1,1);
    inv.slots[2][0] = new ItemStack(2,1);
    inv.slots[3][0] = new ItemStack(3,1);
    inv.slots[4][0] = new ItemStack(4,1);
};

void Player::render() {
    int frame = ((walkTicks+3)/4) % 6;
    Camera::placeTexture(TextureManager::character, dir, 2*frame, 1, 2, {pos.X,pos.Y-1});
}

void Player::renderHotbar(int scale) {
    int x = Window::size.X/2-3*scale*16;
    int y = Window::size.Y-100;
    TextureManager::drawTexture(TextureManager::hotbar, 0, 0, 103, 18, x-scale, y-scale, scale*103, scale*18);
    inv.render(x, y, scale*17, scale);
}

bool Player::checkCollision(pout disp, Map* world) {
    float r = 0.3;
    float x = pos.X+disp.X;
    float y = pos.Y+disp.Y;
    for (int i = floor(x); i <= floor(x)+1; i++) {
        for (int j = floor(y); j <= floor(y)+1; j++) {
            if (abs(i-x) < r+0.5 && abs(j-y) < r+0.5) {
                if (world -> hasSolid(i,j)) {
                    return true;
                }
            }
        }
    }
    return false;
}

void Player::update(Map* world) {
    if (Camera::mode == 1) {
        return;
    }
    pout disp = {0,0};
    if (Window::keys[SDL_SCANCODE_W]) {
        dir = 2;
        disp.Y -= speed;
    } else if (Window::keys[SDL_SCANCODE_S]) {
        dir = 6;
        disp.Y += speed;
    }
    
    if (Window::keys[SDL_SCANCODE_A]) {
        if (dir == 2) {
            dir = 3;
        } else if (dir == 6) {
            dir = 5;
        } else {
            dir = 4;
        }
        disp.X -= speed;
    } else if (Window::keys[SDL_SCANCODE_D]) {
        if (dir == 2) {
            dir = 1;
        } else if (dir == 6) {
            dir = 7;
        } else {
            dir = 0;
        }
        disp.X += speed;
    }
    
    if (disp.X != 0 && disp.Y != 0) {
        disp.X /= 1.4;
        disp.Y /= 1.4;
    }
    
    bool walking = false;
    
    if (disp.X != 0 && !checkCollision({disp.X,0},world)) {
        pos.X += disp.X;
        walking = true;
    }
    
    if (disp.Y != 0 && !checkCollision({0,disp.Y},world)) {
        pos.Y += disp.Y;
        walking = true;
    }
    if (walking) {
        walkTicks += 1;
    } else {
        walkTicks = 0;
    }
    
    Camera::target = {pos.X*ZOOM,pos.Y*ZOOM};
    
}
