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
    TextManager::loadFont("assets/font.ttf", 32);
}

pair<int> TextManager::textSize(std::string &text) {
    int w = 0, h = 0;
    if (TTF_SizeText(font, text.c_str(), &w, &h)) { ERROR("Failed to determine size of", text); }
    return {w,h};
}

pair<int> TextManager::textSize(v(std::string) &text, int dy) {
    int w = 0;
    for (std::string s : text) { w = std::max(w,textSize(s).X); }
    int h = ((int)text.size()-1)*dy + TTF_FontHeight(font);
    return {w,h};
}

void TextManager::drawText(std::string &text, pair<int> pos, bool centre, SDL_Color color) {
    if (text.length() == 0) { return; }
    if (!font) { ERROR("No font loaded"); return; }
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Window::renderer, textSurface);
    if (centre) { pos.X -= (textSurface -> w)/2; pos.Y -= (textSurface -> h)/2; }
    SDL_Rect renderQuad = {pos.X, pos.Y, textSurface -> w, textSurface -> h};
    SDL_FreeSurface(textSurface);
    SDL_RenderCopy(Window::renderer, texture, NULL, &renderQuad);
    SDL_DestroyTexture(texture);
}

void TextManager::drawText(v(std::string) &text, pair<int> pos, int dy, bool centre, SDL_Color color) {
    for (int i = 0; i < text.size(); i++) {
        drawText(text[i], {pos.X, pos.Y + i*dy}, centre, color);
    }
}

void TextManager::loadFont(const char* path, int size) {
    font = TTF_OpenFont(path, size);
    if (font) { LOG("Font loaded from", path); } else { ERROR("Failed to load font from", path); }
}
