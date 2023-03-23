
#pragma once
#include "utils.hpp"

using Node = std::pair<int, pair>;

struct PathFinder {
	static Direction::value find_direction(pair start, pair end, std::unordered_set<pair>& map, bool diagonal = false) {
		std::vector<Direction::value> path = bfs(start, end, map, diagonal);
		return path.empty() ? Direction::NONE : path[0];
	}

	static std::vector<Direction::value> bfs(pair start, pair end, std::unordered_set<pair>& map, bool diagonal = false) {
		std::unordered_map<pair, Direction::value> visited;
		auto comp = [&](Node& left, Node& right) {
			return left.first + dist(left.second, end) > right.first + dist(right.second, end);
		};
		std::priority_queue<Node, std::vector<Node>, decltype(comp)> queue(comp);
		visited[start] = Direction::NONE;
		queue.emplace(0, start);
		int counter = 0;
		bool found = false;

		while (!queue.empty() && counter < 1000 && !found) {
			Node node = queue.top();
			queue.pop();
			for (int d = 1; d < 9; d += (2-diagonal)) {
				pair next = node.second + taxiSteps[d];
				if (d % 2 == 0) {
					pair left = node.second + taxiSteps[d % 8 + 1];
					pair right = node.second + taxiSteps[(d+6) % 8 + 1];
					if (map.find(left) != map.end() || map.find(right) != map.end()) continue;
				}
				if (visited.find(next) == visited.end() && map.find(next) == map.end()) {
					visited[next] = Direction::from_int(d);
					if (next == end) {
						found = true;
						break;
					}
					queue.emplace(node.first + 1, next);
				}
			}
			counter += 1;
		}

		std::vector<Direction::value> res;
		if (!found) return res;
		pair pos = end;
		while (pos != start) {
			res.push_back(visited[pos]);
			pos += taxiSteps[(visited[pos] + 3) % 8 + 1];
		}
		std::reverse(res.begin(), res.end());
		return res;
	}
};