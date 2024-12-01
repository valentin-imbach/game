
#pragma once
#include "utils.hpp"

struct DungeonRoom {
	std::vector<pair> neighbours;
	bool doors[4] = {};
	DungeonRoom() : neighbours() {}
};

typedef std::map<pair, DungeonRoom> DungeonMap;

struct DungeonGraph {
	DungeonMap map;

	DungeonGraph(uint seed, int depth) {
		DungeonMap m;
		pair start(0, 0);
		m[start] = DungeonRoom();
		map = generate(seed, m, start, depth);		
	}

	DungeonMap generate(uint seed, DungeonMap map, pair start, int depth) {
		assert(map.find(start) != map.end());
		if (depth == 0) return map;

		for (int i = 0; i < 3; i++) {
			int dir = noise::Int(seed++, 4);
			pair cell = start + Direction::taxi[2*dir + 1];
			if (map.find(cell) == map.end()) {
				map[cell].neighbours.push_back(start);
				map[cell].doors[(dir + 2) % 4] = true;
				map[start].neighbours.push_back(cell);
				map[start].doors[dir] = true;
				map = generate(noise::UInt(seed++), map, cell, depth - 1);
			}
		}
		return map;
	}

	void print() {
		// for (auto& r : map) {
		// 	LOG(r.first, ":", r.second.neighbours);
		// }

		std::string str = "\n";
		for (int y = -5; y <= 5; y++) {
			for (int x = -5; x <= 5; x++) {
				pair cell(x, y);
				str += (map.find(cell) == map.end()) ? " " : "o";
			}
			str += "\n";
		}
		LOG(str);
	}

};