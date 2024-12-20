
#pragma once
#include "ECS.hpp"
#include "utils.hpp"
#include "EntityFactory.hpp"
#include "ResourceTemplates.hpp"
#include "StructureTemplates.hpp"

ENUM(ClusterId,
BASALT_ROCKS,
CAMP)

struct Cluster {
	virtual bool generate(uint seed, Realm* realm, pair position) = 0;
	virtual ~Cluster() = default;
	static std::array<std::unique_ptr<Cluster>, ClusterId::count> templates;
	static void setTemplates(std::filesystem::path root);
};

struct ResourceCluster : public Cluster {
	ResourceId::value resourceId;
	ResourceCluster(ResourceId::value resourceId) : resourceId(resourceId) {}

	bool generate(uint seed, Realm* realm, pair position) override {
		ResourceTemplate temp = ResourceTemplate::templates[resourceId];
		position -= temp.size / 2;
		if (realm->free(position, temp.size)) {
			EntityFactory::createResource(resourceId, realm->realmId, position);
			return true;
		}
		return false;
	}
};

struct StructureCluster : public Cluster {
	StructureId::value structureId;
	StructureCluster(StructureId::value structureId) : structureId(structureId) {}

	bool generate(uint seed, Realm* realm, pair position) override {
		StructureTemplate temp = StructureTemplate::templates[structureId];
		position -= temp.size / 2;
		if (realm->free(position, temp.size)) {
			EntityFactory::createStructure(structureId, realm->realmId, position);
			return true;
		}
		return false;
	}
};

struct StationCluster : public Cluster {
	StationId::value stationId;
	StationCluster(StationId::value stationId) : stationId(stationId) {}

	bool generate(uint seed, Realm* realm, pair position) override {
		if (realm->free(position)) {
			EntityFactory::createStation(stationId, realm->realmId, position);
			return true;
		}
		return false;
	}
};

struct ChoiceCluster : public Cluster {
	std::vector<std::unique_ptr<Cluster>> clusters;
	ChoiceCluster(std::vector<std::unique_ptr<Cluster>> clusters) : clusters(std::move(clusters)) {
		assert(this->clusters.size() > 0);
	}

	bool generate(uint seed, Realm* realm, pair position) override {
		uint index = noise::Int(seed++, clusters.size());
		return clusters[index]->generate(seed++, realm, position);
	}
};

struct SpreadCluster : public Cluster {
	std::unique_ptr<Cluster> cluster;
	int number;
	int radius;
	int trials;

	SpreadCluster(std::unique_ptr<Cluster> cluster, int radius, int number = 1, int trials = 10) : cluster(std::move(cluster)), radius(radius), number(number), trials(trials) {}

	bool generate(uint seed, Realm* realm, pair position) override {
		int n = 0;
		int t = 0;
		while (n < number && t < trials) {
			float angle = noise::Float(seed++, 0, 2) * M_PI;
			float distance = noise::Float(seed++, 0, radius);
			pair offset = vec::round(vec::polar(angle, distance));
			if (cluster->generate(noise::UInt(seed++), realm, position + offset)) n += 1;
			t += 1;
		}
		return n > 0;
	}
	
};

struct SequenceCluster : public Cluster {
	std::vector<std::unique_ptr<Cluster>> clusters;
	SequenceCluster(std::vector<std::unique_ptr<Cluster>> clusters) : clusters(std::move(clusters)) {}
	bool generate(uint seed, Realm* realm, pair position) override {
		for (int i = 0; i < clusters.size(); i++) {
			if (!clusters[i]->generate(seed++, realm, position)) return i != 0;
		}
		return true;
	}
};

struct SurroundCluster : public Cluster {
	std::unique_ptr<Cluster> centre;
	std::unique_ptr<Cluster> surround;

	uchar distance;
	uchar number;

	SurroundCluster(std::unique_ptr<Cluster> center, std::unique_ptr<Cluster> surround, uchar number, uchar distance) : centre(std::move(center)), surround(std::move(surround)), number(number), distance(distance) {}

	bool generate(uint seed, Realm* realm, pair position) override {
		if (!centre->generate(noise::UInt(seed++), realm, position)) return false;
		for (int i = 0; i < number; i++) {
			float angle = i * 2 * M_PI / number;
			pair offset = vec::round(vec::polar(angle, distance));
			surround->generate(noise::UInt(seed++), realm, position + offset);
		}
		return true;
	}
};