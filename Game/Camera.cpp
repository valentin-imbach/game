//
//  Camera.cpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-20.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#include "Camera.hpp"
#include "TextureManager.hpp"
#include "Window.hpp"

pair<float> Camera::pos = {0,0};
int Camera::speed = 6;
int Camera::mode = 0;

void Camera::placeTexture(SDL_Texture* tex, int x, int y, int w, int h, pair<float> p) {
    int dx = ZOOM*(p.X-pos.X)+Window::size.X/2-ZOOM/2;
    int dy = ZOOM*(p.Y-pos.Y)+Window::size.Y/2-ZOOM/2;
    TextureManager::drawTexture(tex,BIT*x,BIT*y,BIT*w,BIT*h,dx,dy,ZOOM*w,ZOOM*h);
}

void Camera::renderTexture(SDL_Texture *tex, SDL_Rect src, float x, float y, float offset) {
    float w = float(src.w);
    float h = float(src.h);
    float dx = ZOOM*(x-pos.X)+Window::size.X/2-w/2*ZOOM/BIT;
    float dy = ZOOM*(y-pos.Y)+Window::size.Y/2-h/2*ZOOM/BIT-ZOOM*offset/2;
    TextureManager::drawTexture(tex,src.x,src.y,src.w,src.h,dx,dy,ZOOM*src.w/BIT,ZOOM*src.h/BIT);
}

void Camera::renderRect(float x, float y, float w, float h) {
    SDL_Rect rect;
    rect.x = ZOOM*(x-pos.X)+Window::size.X/2;
    rect.y = ZOOM*(y-pos.Y)+Window::size.Y/2;
    rect.w = w*ZOOM;
    rect.h = h*ZOOM;
    SDL_RenderDrawRect(Window::renderer, &rect);
}

pair<float> Camera::gtos(pair<float> p) {
    return {ZOOM*(p.X-pos.X)+Window::size.X/2,ZOOM*(p.Y-pos.Y)+Window::size.Y/2};
}

pair<float> Camera::stog(pair<float> p) {
    return {(p.X-Window::size.X/2)/ZOOM+pos.X,(p.Y-Window::size.Y/2)/ZOOM+pos.Y};
}

void Camera::update(pair<float> p) {
    pos = p;
}

void Camera::render() {
    pair<float> p = stog(Window::mousePos);
    renderRect(round(p.X)-0.5, round(p.Y)-0.5, 1, 1);
}
