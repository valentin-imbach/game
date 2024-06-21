
#include "ClusterTemplates.hpp"
#include "json.hpp"
#include "Window.hpp"

std::array<std::unique_ptr<Cluster>, ClusterId::count> Cluster::templates = {};

void Cluster::setTemplates() {
	std::ifstream file(Window::instance->root / "json/Clusters.json");
	if (!file) ERROR("File not found");
	nlohmann::json data = nlohmann::json::parse(file);
	file.close();

	std::function<std::unique_ptr<Cluster>(nlohmann::json&)> parse;
	parse = [&parse](nlohmann::json& value) -> std::unique_ptr<Cluster> {
		// LOG(value);
		if (value.contains("resource")) {
			ResourceId::value resourceId = ResourceId::from_string(value["resource"]);
			if (!resourceId) {
				WARNING("Unrecognised ResourceId:", value["resource"]);
				return nullptr;
			}
			return std::make_unique<ResourceCluster>(resourceId);
		} else if (value.contains("structure")) {
			StructureId::value structureId = StructureId::from_string(value["structure"]);
			if (!structureId) {
				WARNING("Unrecognised StructureId:", value["structure"]);
				return nullptr;
			}
			return std::make_unique<StructureCluster>(structureId);
		} else if (value.contains("station")) {
			StationId::value stationId = StationId::from_string(value["station"]);
			if (!stationId) {
				WARNING("Unrecognised StructureId:", value["station"]);
				return nullptr;
			}
			return std::make_unique<StationCluster>(stationId);
		} else if (value.contains("sequence")) {
			std::vector<std::unique_ptr<Cluster>> seq;
			for (auto& c: value["sequence"]) {
				std::unique_ptr<Cluster> cluster = parse(c);
				if (!cluster) return nullptr;
				seq.push_back(std::move(cluster));
			}
			return std::make_unique<SequenceCluster>(std::move(seq));
		} else if (value.contains("spread")) {
			int radius = value["radius"];
			int number = value["number"];
			std::unique_ptr<Cluster> cluster = parse(value["spread"]);
			if (!cluster) return nullptr;
			return std::make_unique<SpreadCluster>(std::move(cluster), radius, number);
		} else if (value.contains("surround")) {
			std::unique_ptr<Cluster> centre = parse(value["surround"][0]);
			std::unique_ptr<Cluster> surround = parse(value["surround"][1]);
			int number = value["number"];
			int radius = value["radius"];
			if (!centre || !surround) return nullptr;
			return std::make_unique<SurroundCluster>(std::move(centre), std::move(surround), number, radius);
		} else if (value.contains("choice")) {
			std::vector<std::unique_ptr<Cluster>> seq;
			for (auto& c: value["choice"]) {
				std::unique_ptr<Cluster> cluster = parse(c);
				if (!cluster) return nullptr;
				seq.push_back(std::move(cluster));
			}
			return std::make_unique<ChoiceCluster>(std::move(seq));
		}
		return nullptr;
	};

	templates = {};

	for (auto &[key, value] : data.items()) {
		ClusterId::value clusterId = ClusterId::from_string(key);
		if (!clusterId) {
			WARNING("Unrecognised ClusterId:", key);
			continue;
		}
		
		templates[clusterId] = parse(value);
		LOG("ClusterTemplate", key, "parsed");
	}
	LOG("ClusterTenplates loaded");

	
}