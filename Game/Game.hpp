//
//  Game.hpp
//  Game
//
//  Created by Valentin Imbach on 2019-09-20.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#pragma once
#include "LayerSystem/Layers.h"
#include "Gui/GuiManager.hpp"
#include "Gui/PauseMenu.hpp"
#include "Gui/MainMenu.hpp"
#include "Console.hpp"
#include "StateController.h"
#include "World.hpp"
#include "serializer.h"
#include "SoundManager.hpp"

class Game : public Serializable {
public:
    Game();
    void render();
    void update();
    void handleEvents();
    static void loadWorld(std::string name);
    static void createWorld();
    
    static StateController controller;

    static World* world;
    PauseMenu pauseMenu;
    MainMenu mainMenu;
    Console console;
    
    v(std::string) worlds;
    
    SERIALIZE(SoundManager::volume,Camera::ZOOM,Window::MAX_FPS);
};
