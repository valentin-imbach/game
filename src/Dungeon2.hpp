
#pragma once
#include "utils.hpp"

struct Access {
	bool door[4] = {};
	int pos[4] = {};
};

#define MIN_SPLIT_DIM 12
#define MIN_ROOM_DIM 5
#define MIN_DOOR_GAP 1

class Dungeon {
public:
	Dungeon(pair tl, pair br, uint seed) : layout(), seed(seed) {
		generate(tl, br);
	}

private:
	std::set<pair> layout;
	uint seed;

	void generate(pair tl, pair br, Access acc = {}) {
		pair size = br - tl;
		pair mid = tl + size/2;
		bool xsplit = noise::Bool(seed++);

		if (size.x >= MIN_SPLIT_DIM && (xsplit || size.y < MIN_SPLIT_DIM)) {
			int cut = tl.x + noise::Int(seed++, MIN_ROOM_DIM, size.x - MIN_ROOM_DIM - 1);
			int door = tl.y + noise::Int(seed++, MIN_DOOR_GAP, size.y - MIN_DOOR_GAP - 1);
			Access a1;
			a1.door[0] = true;
			a1.pos[0] = door;
			generate(tl, pair(cut, br.y), a1);

			Access a2;
			a2.door[2] = true;
			a2.pos[2] = door;
			generate(pair(cut+1, tl.y), br, a2);
		} else if (size.y >= MIN_SPLIT_DIM) {
			int cut = tl.y + noise::Int(seed++, MIN_ROOM_DIM, size.y - MIN_ROOM_DIM - 1);
			int door = tl.x + noise::Int(seed++, MIN_DOOR_GAP, size.x - MIN_DOOR_GAP - 1);
			generate(tl, pair(br.x, cut));
			generate(pair(tl.x, cut+1), br);
		} else {
			int w = noise::Int(seed++, MIN_ROOM_DIM, size.x + 1);
			int h = noise::Int(seed++, MIN_ROOM_DIM, size.y + 1);
			int xoff = noise::Int(seed++, size.x - w + 1);
			int yoff = noise::Int(seed++, size.y - h + 1);
			for (int x = 0; x < w; x++) {
				for (int y = 0; y < h; y++) {
					layout.emplace(tl.x + xoff + x, tl.y + yoff + y);
				}
			}
		}
	}

friend class Realm;
};
