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
