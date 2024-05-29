
#pragma once
#include "utils.hpp"

namespace ai {

	using GridPred = std::function<bool(pair)>;
	using Node = std::pair<int, pair>;

	inline std::vector<Direction::value> a_star(std::function<bool(pair)> space, pair start, std::function<bool(pair)> target, std::function<float(pair)> cost, std::function<float(pair)> heuristic, bool diagonal, uint steps) {
		std::unordered_map<pair, Direction::value> entered;
		using Node = std::pair<float, pair>;

		auto comparison = [&](Node& left, Node& right) {
			return left.first + heuristic(left.second) > right.first + heuristic(right.second);
		};

		std::priority_queue<Node, std::vector<Node>, decltype(comparison)> queue(comparison);

		if (target(start)) return {};
		entered[start] = Direction::NONE;
		queue.emplace(cost(start), start);

		bool found = false;
		while (!queue.empty() && steps && !found) {
			Node node = queue.top();
			queue.pop();
			for (int d = 1; d < 9; d += (2 - diagonal)) {
				pair next = node.second + Direction::taxi[d];
				if (d % 2 == 0) {
					pair left = node.second + Direction::taxi[Direction::rotate(d, 1)];
					pair right = node.second + Direction::taxi[Direction::rotate(d, -1)];
					if (!space(left) || !space(right)) continue;
				}
				if (space(next) && entered.find(next) == entered.end()) {
					entered[next] = Direction::from_int(d);
					queue.emplace(node.first + cost(next), next);
					if (target(next)) {
						found = true;
						break;
					}
				}
			}
			steps -= 1;
		}

		std::vector<Direction::value> path;
		pair pos = queue.top().second;
		while (pos != start) {
			path.push_back(entered[pos]);
			pos += Direction::taxi[Direction::rotate(entered[pos], 4)];
		}
		std::reverse(path.begin(), path.end());
		return path;
	}

	inline std::vector<Direction::value> bfs(pair start, pair end, GridPred free, bool diagonal = false) {
		std::unordered_map<pair, Direction::value> visited;

		auto comp = [&](Node& left, Node& right) {
			return left.first + vec::dist(left.second, end) > right.first + vec::dist(right.second, end);
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
					if (!free(left) || !free(right)) continue;
				}
				if (free(next) && visited.find(next) == visited.end()) {
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

	inline Direction::value find_direction(pair start, pair end, GridPred free, bool diagonal = false) {
		std::vector<Direction::value> path = bfs(start, end, free, diagonal);
		return path.empty() ? Direction::NONE : path[0];
	}

	inline Direction::value seek(pair start, pair end, std::function<bool(pair)> space, bool diagonal = false, uint steps = 100) {
		std::function<bool(pair)> target = [end](pair pos) { return pos == end; };
		std::function<float(pair)> cost = [](pair pos) { return 1.0f; };
		std::function<float(pair)> heuristic = [end](pair pos) { return vec::dist(pos, end)*2; };

		std::vector<Direction::value> path = a_star(space, start, target, cost, heuristic, diagonal, 100);
		return path.empty() ? Direction::NONE : path[0];
	}

	inline Direction::value avoid(pair start, pair avoid, std::function<bool(pair)> space, bool diagonal = false, uint steps = 100) {
		std::function<bool(pair)> target = [avoid](pair pos) { return vec::dist(pos, avoid) > 5; };
		std::function<float(pair)> cost = [](pair pos) { return 1.0f; };
		std::function<float(pair)> heuristic = [avoid](pair pos) { return -vec::dist(pos, avoid)*2; };

		std::vector<Direction::value> path = a_star(space, start, target, cost, heuristic, diagonal, 100);
		return path.empty() ? Direction::NONE : path[0];
	}

	inline std::pair<bool, pair> visible(vec start, vec end, GridPred free) {
		float d = vec::dist(start, end);
		int steps = std::max(int(std::ceil(10 * d)), 2);
		vec offset = (end - start)/steps;
		vec pos = start;
		for (int i = 0; i <= steps; i++) {
			pair p = vec::round(pos);
			if (!free(p)) return {false, p};
			pos += offset;
		}
		return {true, {}};
	}

	inline bool old_visible(vec start, vec end, std::unordered_set<pair>& map) { //TODO floating point problem
		int sx = std::round(start.x);
		int xrange = std::abs(std::round(end.x) - sx);
		float dy = (end.y - start.y) / std::abs(end.x - start.x);
		int dir = (end.x > start.x) ? 1 : -1;

		float h = start.y;
		for (int i = 0; i <= xrange; i++) {
			float y1 = std::round(h);
			if (i == 0) {
				h = start.y + (sx + 0.5f - start.x) * dy;
			} else if (i == xrange) {
				h = end.y;
			} else {
				h += dy;
			}
			float y2 = std::round(h);

			for (int y = std::min(y1, y2); y <= std::max(y1, y2); y++) {
				pair pos(sx + dir * i, y);
				if (map.find(pos) != map.end()) return false;
			}
		}
		return true;
	}
}
