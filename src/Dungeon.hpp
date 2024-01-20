
#pragma once
#include "utils.hpp"
#include <map>

namespace dungeon {

	enum Face : ushort {
		EAST,
		NORTH,
		WEST,
		SOUTH
	};

	struct Layout {
		void insert(pair pos) {
			grid.insert(pos);
			xBounds[pos.y].x = std::min(xBounds[pos.y].x, pos.x);
			xBounds[pos.y].y = std::max(xBounds[pos.y].y, pos.x);
			yBounds[pos.x].x = std::min(yBounds[pos.x].x, pos.y);
			yBounds[pos.x].y = std::max(yBounds[pos.x].y, pos.y);
		}
		void shiftUnion(Layout& other, pair offset, int pad, Face face) {
			pair shift(0, 0);
			if (face == EAST) {
				shift.x = pad;
				for (auto bounds : xBounds) {
					int off = bounds.second.y - other.xBounds[bounds.first - offset.y].x - offset.x;
					shift.x = std::max(shift.x, off);
				}
			} else if (face == NORTH) {
				shift.y = -pad;
				for (auto bounds : yBounds) {
					int off = bounds.second.x - other.yBounds[bounds.first - offset.x].y - offset.y;
					shift.y = std::min(shift.y, off);
				}
			} else if (face == WEST) {
				shift.x = -pad;
				for (auto bounds : xBounds) {
					int off = bounds.second.x - other.xBounds[bounds.first - offset.y].y - offset.x;
					shift.x = std::min(shift.x, off);
				}
			} else if (face == SOUTH) {
				shift.y = pad;
				for (auto bounds : yBounds) {
					int off = bounds.second.y - other.yBounds[bounds.first - offset.x].x - offset.y;
					shift.y = std::max(shift.y, off);
				}
			}

			for (pair pos : other.grid) {
				insert(pos + offset + shift);
			}
		}
		std::unordered_set<pair> grid;
		std::map<int, pair> xBounds;
		std::map<int, pair> yBounds;
	};

	inline pair steps[4] = { pair(1, 0), pair(0, -1), pair(-1, 0), pair(0, 1) };

	struct Node {
		Node(uint seed, uint depth, Face entry) : seed(seed), depth(depth), entry(entry) {}
		virtual ~Node() = default;
		uint seed;
		uint depth;
		Face entry;

		Layout layout;

		pair size;
		pair doors[4];

		virtual void generate() = 0;

		void build() {
			for (int i = 0; i < 4; i++) {
				if (children[i]) children[i]->build();
			}
			generate();
		}
		std::array<std::unique_ptr<Node>, 4> children;
	};

	struct EntryNode : public Node {
		EntryNode(uint seed, int depth = 0, Face entry = SOUTH) : Node(seed, depth, entry) {
			if (depth < 3) {
				for (int i = 0; i < 4; i++) {
					if (i == entry) continue;
					if (noise::Bool(seed++)) {
						uint newSeed = noise::UInt(seed++);
						children[i] = std::make_unique<EntryNode>(newSeed, depth + 1, Face((i + 2) % 4));
					}
				}
			}
		}
		void generate() override {
			uint s = seed;

			size.x = noise::Int(s++, 4, 7);
			size.y = noise::Int(s++, 4, 7);

			doors[0] = pair(size.x - 1, noise::Int(s++, 1, size.y - 1));
			doors[1] = pair(noise::Int(s++, 1, size.x - 1), 0);
			doors[2] = pair(0, noise::Int(s++, 1, size.y - 1));
			doors[3] = pair(noise::Int(s++, 1, size.x - 1), size.y - 1);

			pair zero = doors[entry];

			for (int x = 0; x < size.x; x++) {
				for (int y = 0; y < size.y; y++) {
					pair pos(x,y);
					layout.insert(pos - zero);
				}
			}

			for (int i = 0; i < 4; i++) {
				if (!children[i]) continue;
				int corridor = noise::Int(s++, 3, 7);
				pair offset = doors[i] - zero + corridor * steps[i];
				for (int c = 0; c < corridor; c++) {
					layout.insert(doors[i] - zero + c * steps[i]);
				}

				layout.shiftUnion(children[i]->layout, offset, 2, Face(i));
			}
		}
	};

}