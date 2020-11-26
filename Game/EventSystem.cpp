//
//  EventSystem.cpp
//  Game
//
//  Created by Valentin Imbach on 26/11/2020.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#include "EventSystem.hpp"

GameEvent::GameEvent(EventType type) : type(type) {}

void Observer::onEvent(GameEvent event) {}
void Observer::subscribe(EventType type) {
    EventManager::Instance -> subscribers[(int)type].push_back(this);
}

EventManager* EventManager::Instance = nullptr;
EventManager::EventManager() {
    Instance = this;
}

void EventManager::distribute(GameEvent event) {
    for (Observer* observer : subscribers[(int)event.type]) {
        observer -> onEvent(event);
    }
}

