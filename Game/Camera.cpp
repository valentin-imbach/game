//
//  Camera.cpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-20.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#include "Camera.hpp"
#include "Window.hpp"

pair<float> Camera::position = {0,0};
int Camera::ZOOM = 64;

void Camera::drawTexture(SDL_Texture *tex, int sx, int sy, int sw, int sh, pair<float> dest, pair<int> size, int offset, bool centred, int alpha) {
    float dx = ZOOM*(dest.X - position.X) + Window::size.X/2;
    float dy = ZOOM*(dest.Y - position.Y) + Window::size.Y/2 - ZOOM*offset/2;
    TextureManager::drawTexture(tex, BIT*sx, BIT*sy, BIT*sw, BIT*sh, dx, dy, ZOOM*size.X, ZOOM*size.Y, centred, alpha);
}

void Camera::drawSprite(Sprite sprite, pair<float> pos, bool centred, int alpha) {
    pair<int> spos = sprite.getPosition();
    drawTexture(sprite.texture, spos.X, spos.Y, sprite.size.X, sprite.size.Y, pos, sprite.size, 0, centred, alpha);
}

void Camera::drawRect(pair<float> pos, pair<float> size) {
    SDL_Rect rect;
    rect.x = ZOOM*(pos.X - position.X) + Window::size.X/2;
    rect.y = ZOOM*(pos.Y - position.Y) + Window::size.Y/2;
    rect.w = size.X*ZOOM;
    rect.h = size.Y*ZOOM;
    SDL_RenderDrawRect(Window::renderer, &rect);
}

pair<int> Camera::gtos(pair<float> p) {
    return {(int)(ZOOM*(p.X - position.X) + Window::size.X/2), (int)(ZOOM*(p.Y - position.Y) + Window::size.Y/2)};
}

pair<float> Camera::stog(pair<int> p) {
    return {(float)(p.X - Window::size.X/2)/ZOOM + position.X, (float)(p.Y - Window::size.Y/2)/ZOOM + position.Y};
}

void Camera::update(pair<float> p) {
    position = p;
}

void Camera::render() {
    pair<float> pos = stog(Window::mousePos);
    drawRect(pair<float>(round(pos.X) - 0.5, round(pos.Y) - 0.5), {1, 1});
}
