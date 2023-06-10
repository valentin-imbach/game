
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
			for (int d = 1; d < 9; d += (2 - diagonal)) {
				pair next = node.second + Direction::taxi[d];
				if (d % 2 == 0) {
					pair left = node.second + Direction::taxi[d % 8 + 1];
					pair right = node.second + Direction::taxi[(d + 6) % 8 + 1];
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
			pos += Direction::taxi[(visited[pos] + 3) % 8 + 1];
		}
		std::reverse(res.begin(), res.end());
		return res;
	}
};

inline std::pair<bool, pair> visible(vec start, vec end, std::unordered_set<pair>& map) {
	float d = dist(start, end);
	int steps = std::max(int(std::ceil(10 * d)), 2);
	vec offset = (end - start)/steps;
	vec pos = start;
	for (int i = 0; i <= steps; i++) {
		pair p = round(pos);
		if (map.find(p) != map.end()) return {false, p};
		pos += offset;
	}
	return {true, {}};
}

inline bool old_visible(vec start, vec end, std::unordered_set<pair>& map) { //TODO floating point problem
	int sx = round(start.x);
	int xrange = abs(round(end.x) - sx);
	float dy = (end.y-start.y)/abs(end.x-start.x);
	int dir = (end.x > start.x) ? 1 : -1;

	float h = start.y;
	for (int i = 0; i <= xrange; i++) {
		float y1 = round(h);
		if (i == 0) {
			h = start.y + (sx + 0.5f - start.x) * dy;
		} else if (i == xrange) {
			h = end.y;
		} else {
			h += dy;
		}
		float y2 = round(h);

		for (int y = std::min(y1, y2); y <= std::max(y1, y2); y++) {
			pair pos(sx + dir * i, y);
			//LOG(pos);
			if (map.find(pos) != map.end()) return false;
		}
	}
	return true;
}