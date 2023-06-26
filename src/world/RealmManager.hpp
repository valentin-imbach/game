
#pragma once
#include "utils.hpp"
#include "Realm.hpp"

using RealmId = unsigned char;

class RealmManager {
public:
	RealmManager(uchar maxRealms) : maxRealms(maxRealms) {
		availableRealms.reserve(maxRealms);
		for (uint i = maxRealms; i > 0; i--) availableRealms.push_back(i);
	}

	Realm* addRealm(World* world, pair size, uint seed) {
		if (availableRealms.empty()) {
			WARNING("Max realms reached");
			return 0;
		}
		RealmId realmId = availableRealms.back();
		availableRealms.pop_back();
		realms[realmId] = std::make_unique<Realm>(realmId, world, size, seed);
		LOG("Realm", int(realmId), "created");
		return realms[realmId].get();
	}

	void removeRealm(RealmId realmId) {
		if (!realmId) return;
		if (realms.find(realmId) == realms.end()) {
			WARNING("Trying to remove non-existing realm");
			return;
		}
		realms.erase(realmId);
		availableRealms.push_back(realmId);
		LOG("Realm", realmId, "destroyed");
	}

	Realm* getRealm(RealmId realmId) {
		if (!realmId) return nullptr;
		if (realms.find(realmId) == realms.end()) {
			WARNING("Trying to access non-existing realm");
			return nullptr;
		}
		return realms[realmId].get();
	}

private:
	uchar maxRealms;
	std::unordered_map<RealmId, std::unique_ptr<Realm>> realms;
	std::vector<RealmId> availableRealms;
};