
#include "Realm.hpp"

void Realm::update(uint dt) {
	for (auto& item : chunks) item.second.update(dt);
}

void Realm::draw() {
	for (auto& item : chunks) item.second.draw();
}