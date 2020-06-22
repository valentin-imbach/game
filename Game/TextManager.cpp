//
//  TextManager.cpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-20.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#include "TextManager.hpp"
#include "Window.hpp"

TTF_Font* TextManager::font = nullptr;

void TextManager::Init() {
    if (TTF_Init() == 0) { LOG("TextManager initialized"); } else { ERROR("Failed to initialize TTF"); }
    TextManager::loadFont("assets/font.ttf", 16);
}

void TextManager::drawText(std::string text, pair<int> pos, bool centre, SDL_Color color) {
    if (text.length() == 0) { return; }
    if (!font) { ERROR("No font loaded"); return; }
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Window::renderer, textSurface);
    if (centre) { pos.X -= textSurface -> w; }
    SDL_Rect renderQuad = {pos.X, pos.Y - textSurface -> h, 2*(textSurface -> w), 2*(textSurface -> h)};
    SDL_FreeSurface(textSurface);
    SDL_RenderCopy(Window::renderer, texture, NULL, &renderQuad);
    SDL_DestroyTexture(texture);
}

void TextManager::drawText(int a, pair<int> pos, bool centre, SDL_Color color) {
    drawText(std::to_string(a), pos, centre, color);
}

void TextManager::loadFont(const char* path, int size) {
    font = TTF_OpenFont(path, size);
    if (font) { LOG("Font loaded from",path); } else { ERROR("Failed to load font from",path); }
}
