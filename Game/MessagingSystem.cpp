//
//  EventSystem.cpp
//  Game
//
//  Created by Valentin Imbach on 26/11/2020.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#include "MessagingSystem.hpp"

Message::Message(MessageType type) : type(type) {}

void Observer::onMessage(Message event) {}
void Observer::subscribe(MessageType type) {
    MessageManager::Instance -> subscribers[(int)type].push_back(this);
}

MessageManager* MessageManager::Instance = nullptr;
MessageManager::MessageManager() {
    Instance = this;
}

void MessageManager::distribute(Message message) {
    for (Observer* observer : subscribers[(int)message.type]) {
        observer -> onMessage(message);
    }
}

