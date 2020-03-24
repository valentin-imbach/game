//
//  GameObject.cpp
//  Game
//
//  Created by Valentin Imbach on 17.12.19.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#include "GameObject.hpp"

GameObject::GameObject() {};

void GameObject::update() {};

void GameObject::render() {
    Camera::placeTexture(TextureManager::spriteSheet, sprite.X, sprite.Y, dim.X, dim.Y+height, {pos.X,pos.Y-height});
};
