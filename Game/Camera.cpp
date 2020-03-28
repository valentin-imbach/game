//
//  Camera.cpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-20.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#include "Camera.hpp"

pint Camera::pos = {512,512};
int Camera::speed = 6;
pint Camera::target = {512,512};
int Camera::mode = 0;

void Camera::placeTexture(SDL_Texture* tex, int x, int y, int w, int h, pout p) {
    int dx = ZOOM*p.X-pos.X+Window::size.X/2;
    int dy = ZOOM*p.Y-pos.Y+Window::size.Y/2;
    TextureManager::drawTexture(tex,TILE_SIZE*x,TILE_SIZE*y,TILE_SIZE*w,TILE_SIZE*h,dx,dy,ZOOM*w,ZOOM*h);
}

void Camera::renderTexture(SDL_Texture *tex, SDL_Rect src, float x, float y) {
    int dx = ZOOM*x-pos.X+Window::size.X/2;
    int dy = ZOOM*y-pos.Y+Window::size.Y/2;
    TextureManager::drawTexture(tex,src.x,src.y,src.w,src.h,dx,dy,ZOOM*src.w/TILE_SIZE,ZOOM*src.h/TILE_SIZE);
}

void Camera::renderRect(float x, float y, float w, float h) {
    SDL_Rect rect;
    rect.x = x*ZOOM-pos.X+Window::size.X/2;
    rect.y = y*ZOOM-pos.Y+Window::size.Y/2;
    rect.w = w*ZOOM;
    rect.h = h*ZOOM;
    SDL_RenderDrawRect(Window::renderer, &rect);
}

void Camera::handleEvents() {
    for (SDL_Event e : Window::events) {
        switch (e.type) {
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym) {
                    case (SDLK_c):
                        if (mode == 0) {
                            mode = 1;
                        } else {
                            mode = 0;
                            pos = target;
                        }
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                break;
        }
    }
}

pint Camera::getPosition(pint mouse) {
    int x = pos.X+mouse.X-Window::size.X/2;
    int y = pos.Y+mouse.Y-Window::size.Y/2;
    //LOG2(x/ZOOM,y/ZOOM);
    return {x/ZOOM,y/ZOOM};
}

void Camera::update() {
    handleEvents();
    if (mode == 0) {
        float d = DIST(pos,target);
        if (d > RADIUS*fmin(Window::size.X,Window::size.Y)) {
            pos.X += speed*(target.X-pos.X)/d;
            pos.Y += speed*(target.Y-pos.Y)/d;
        }
    } else if (mode == 1) {
        if (Window::keys[SDL_SCANCODE_W]) { pos.Y -= speed*3; }
        if (Window::keys[SDL_SCANCODE_A]) { pos.X -= speed*3; }
        if (Window::keys[SDL_SCANCODE_S]) { pos.Y += speed*3; }
        if (Window::keys[SDL_SCANCODE_D]) { pos.X += speed*3; }

    }
}
