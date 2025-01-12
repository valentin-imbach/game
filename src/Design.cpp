
#include "Design.hpp"
#include "EntityFactory.hpp"

using namespace design;

std::array<std::unique_ptr<Node>, DesignId::count> Node::templates = {};

bool Refrence::generate(uint seed, Realm* realm, pair position) {
	return Node::templates[designId]->generate(seed++, realm, position);
}

//* Flow

bool Sequence::generate(uint seed, Realm* realm, pair position) {
	bool success;
	for (auto& n : children) success |= n->generate(seed++, realm, position);
	return success;
}

bool Repeat::generate(uint seed, Realm* realm, pair position) {
	bool success;
	for (int i = 0; i < number; i++) {
		for (auto& n : children) success |= n->generate(seed++, realm, position);
	}
	return success;
}

bool Choice::generate(uint seed, Realm* realm, pair position) {
	int index = noise::Int(seed++, children.size());
	return children[index]->generate(seed, realm, position);
	//TODO repeat etc
}

//* Modifier

bool Offset::generate(uint seed, Realm* realm, pair position) {
	bool success;
	for (auto& n : children) {
		int x = noise::Int(seed++, xr.x, xr.y);
		int y = noise::Int(seed++, yr.x, yr.y);
		pair offset(x, y);
		success |= n->generate(seed++, realm, position + offset);
	}
	return success;
}

bool Spread::generate(uint seed, Realm* realm, pair position) {
	bool success;
	for (auto& n : children) {
		float rad = noise::Float(seed++, radr.x, radr.y);
		float ang = noise::Float(seed++, angr.x, angr.y);
		pair offset = vec::round(vec::polar(ang, rad));
		success |= n->generate(seed++, realm, position + offset);
	}
	return success;
}

//* Leaf

bool Place::generate(uint seed, Realm* realm, pair position) {
	switch (id.index()) {
	case 0: 
		return EntityFactory::createResource(std::get<ResourceId::value>(id), realm->realmId, position);
	case 1:
		return EntityFactory::createStructure(std::get<StructureId::value>(id), realm->realmId, position);
	case 2:
		return EntityFactory::createStation(std::get<StationId::value>(id), realm->realmId, position);
	}
	return false;
}