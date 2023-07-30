
#pragma once
#include "utils.hpp"
#include "Realm.hpp"
#include "serialiser.hpp"

using RealmId = unsigned char;

class RealmManager {
public:
	RealmManager(uchar maxRealms) : maxRealms(maxRealms) {
		availableRealms.reserve(maxRealms);
		for (uint i = maxRealms; i > 0; i--) availableRealms.push_back(i);
	}

	Realm* addRealm(World* world, pair size, uint seed, RealmType::value realmType = RealmType::WORLD) {
		if (availableRealms.empty()) {
			WARNING("Max realms reached");
			return 0;
		}
		RealmId realmId = availableRealms.back();
		availableRealms.pop_back();
		realms[realmId] = std::make_unique<Realm>(realmId, size, seed, realmType);
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

	void serialise(std::fstream& stream) {
		uint num = realms.size();
		serialise_object(stream, num);
		for (const auto& pair : realms) pair.second->serialise(stream);
		uint available = availableRealms.size();
		serialise_object(stream, available);
		for (RealmId realmId : availableRealms) serialise_object(stream, realmId);
	}

	void deserialise(World* world, std::fstream& stream) {
		uint num;
		deserialise_object(stream, num);
		for (int i = 0; i < num; i++) {
			std::unique_ptr<Realm> realm = std::make_unique<Realm>(stream);
			realms[realm->realmId] = std::move(realm);
		}
		uint available;
		deserialise_object(stream, available);
		availableRealms.clear();
		for (int i = 0; i < available; i++) {
			RealmId realmId;
			deserialise_object(stream, realmId);
			availableRealms.push_back(realmId);
		}
	}

private:
	uchar maxRealms;
	std::unordered_map<RealmId, std::unique_ptr<Realm>> realms;
	std::vector<RealmId> availableRealms;
};