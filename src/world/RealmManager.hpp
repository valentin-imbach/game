
#pragma once
#include "Realm.hpp"

using RealmId = unsigned char;

class RealmManager {
public:
	RealmManager(uchar maxRealms);

	Realm* addRealm(World* world, uint seed, RealmType::value realmType = RealmType::WORLD);
	void removeRealm(RealmId realmId);
	Realm* getRealm(RealmId realmId);

	void serialise(std::fstream& stream);
	void deserialise(World* world, std::fstream& stream);

private:
	uchar maxRealms;
	std::unordered_map<RealmId, std::unique_ptr<Realm>> realms;
	std::vector<RealmId> availableRealms;
};