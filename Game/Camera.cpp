//
//  Camera.cpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-20.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#include "Camera.hpp"

pout Camera::pos = {0,0};
int Camera::speed = 6;
int Camera::mode = 0;

void Camera::placeTexture(SDL_Texture* tex, int x, int y, int w, int h, pout p) {
    int dx = ZOOM*(p.X-pos.X)+Window::size.X/2-ZOOM/2;
    int dy = ZOOM*(p.Y-pos.Y)+Window::size.Y/2-ZOOM/2;
    TextureManager::drawTexture(tex,TILE_SIZE*x,TILE_SIZE*y,TILE_SIZE*w,TILE_SIZE*h,dx,dy,ZOOM*w,ZOOM*h);
}

void Camera::renderTexture(SDL_Texture *tex, SDL_Rect src, float x, float y, float offset) {
    float w = float(src.w);
    float h = float(src.h);
    float dx = ZOOM*(x-pos.X)+Window::size.X/2-w/2*ZOOM/TILE_SIZE;
    float dy = ZOOM*(y-pos.Y)+Window::size.Y/2-h/2*ZOOM/TILE_SIZE-ZOOM*offset/2;
    TextureManager::drawTexture(tex,src.x,src.y,src.w,src.h,dx,dy,ZOOM*src.w/TILE_SIZE,ZOOM*src.h/TILE_SIZE);
}

void Camera::renderRect(float x, float y, float w, float h) {
    SDL_Rect rect;
    rect.x = ZOOM*(x-pos.X)+Window::size.X/2-1;
    rect.y = ZOOM*(y-pos.Y)+Window::size.Y/2-1;
    rect.w = w*ZOOM+2;
    rect.h = h*ZOOM+2;
    SDL_RenderDrawRect(Window::renderer, &rect);
}

pout Camera::gtos(pout p) {
    return {ZOOM*(p.X-pos.X)+Window::size.X/2,ZOOM*(p.Y-pos.Y)+Window::size.Y/2};
}

pout Camera::stog(pout p) {
    return {(p.X-Window::size.X/2)/ZOOM+pos.X,(p.Y-Window::size.Y/2)/ZOOM+pos.Y};
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
                        }
                        break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                //LOGP(stog(Window::mousePos));
                break;
        }
    }
}

void Camera::update() {
    handleEvents();
}

void Camera::render() {
    pout p = stog(Window::mousePos);
    renderRect(round(p.X)-0.5, round(p.Y)-0.5, 1, 1);
}
