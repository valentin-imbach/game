//
//  EventSystem.cpp
//  Game
//
//  Created by Valentin Imbach on 26/11/2020.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#include "MessagingSystem.hpp"

vv(Observer*) MessageManager::subscribers = vv(Observer*)((int)MessageType::MAX,v(Observer*)());

void MessageManager::distribute(const Message &message) {
    for (Observer* observer : subscribers[(int)message.type]) observer -> onMessage(message);
}

void MessageManager::notify(const Message &message) {
    distribute(message);
}

bool Observer::isSubscribed(MessageType type) {
    return std::find(subscriptions.begin(), subscriptions.end(), type) != subscriptions.end();
}

void Observer::subscribe(MessageType type) {
    if (isSubscribed(type)) return;
    MessageManager::subscribers[(int)type].push_back(this);
    subscriptions.push_back(type);
}

void Observer::unsubscribe(MessageType type) {
    v(Observer*)& subers = MessageManager::subscribers[(int)type];
    subers.erase(std::remove(subers.begin(), subers.end(), this), subers.end());
    subscriptions.erase(std::remove(subscriptions.begin(),subscriptions.end(),type), subscriptions.end());
}

Observer::~Observer() {
    for (MessageType type : subscriptions) unsubscribe(type);
}
