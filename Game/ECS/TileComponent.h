//
//  TileComponent.h
//  Game
//
//  Created by Valentin Imbach on 07.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "Camera.hpp"
#include "../TextureManager.hpp"
#include "Components.h"

class TileComponent : public Component {
public:
    static ComponentType componentType;
    v(Sprite) sprites;
    TileComponent* neig[8];
    pair<int> position;
    TileID tileID;

    TileComponent(pair<int> pos = {0,0}, TileID tid = (TileID)0) : position(pos), tileID(tid) {}
    
    void init() override {
        entity -> addTag(EntityTag::TILE);
    }
    
    void render() override {
        for (auto s : sprites)
            Camera::drawTexture(s.texture, s.position.X, s.position.Y, 1, 1, position, {1, 1}, 0, true);
    }
    
    void updateStyle() {
        sprites.clear();
        sprites.emplace_back(TextureManager::tilesets[tileID],pair<int>(1,1));
        for (int d = 0; d < 8; d += 2) {
            int l = neig[(d+2) % 8] -> tileID;
            int r = neig[(d+6) % 8] -> tileID;
            if (neig[d]  -> tileID < tileID && l != neig[d] -> tileID && r != neig[d] -> tileID)
                sprites.emplace_back(TextureManager::tilesets[neig[d] -> tileID],pair<int>(1-dirs2[d].X,1-dirs2[d].Y));
        }
        
        for (int d = 1; d < 8; d += 2) {
            int l = neig[(d+1) % 8] -> tileID;
            int r = neig[(d+7) % 8] -> tileID;
            if (neig[d] -> tileID < tileID && l != neig[d] -> tileID && r != neig[d] -> tileID)
                sprites.emplace_back(TextureManager::tilesets[neig[d] -> tileID],pair<int>(1-dirs2[d].X,1-dirs2[d].Y));
            if (l < tileID && l == r)
                sprites.emplace_back(TextureManager::tilesets[neig[d] -> tileID],pair<int>(1-dirs2[d].X,4-dirs2[d].Y));
        }
        //std::sort(sprites.begin(), sprites.end(),[](Sprite& a, Sprite&b){ return a.priority > b.priority; });
    }
    
};

