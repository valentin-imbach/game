
#pragma once
#include "utils.hpp"

struct Access {
	int door[4] = {};
};

#define MIN_SPLIT_DIM 12
#define MIN_ROOM_DIM 5
#define MIN_DOOR_GAP 1

class Dungeon {
public:
	Dungeon(pair tl, pair br, uint seed) : layout(), seed(seed) {
		Access acc = {br.y, br.x, br.y, br.x};
		generate(tl, br, acc);
	}

private:
	std::set<pair> layout;
	uint seed;

	void generate(pair tl, pair br, Access acc) {
		pair size = br - tl;
		pair mid = tl + size/2;
		bool xsplit = noise::Bool(seed++);

		if (tl.y <= acc.door[0] && acc.door[0] < br.y) {
			layout.emplace(br.x, acc.door[0]);
		}
		if (tl.x <= acc.door[3] && acc.door[3] < br.x) {
			layout.emplace(acc.door[1], br.y);
		}

		if (size.x >= MIN_SPLIT_DIM && (xsplit || size.y < MIN_SPLIT_DIM)) {
			int cut = tl.x + noise::Int(seed++, MIN_ROOM_DIM, size.x - MIN_ROOM_DIM - 1);
			int door = tl.y + noise::Int(seed++, MIN_DOOR_GAP, size.y - MIN_DOOR_GAP - 1);

			Access a1 = acc;
			a1.door[0] = door;
			generate(tl, pair(cut, br.y), a1);

			Access a2 = acc;
			a2.door[2] = door;
			generate(pair(cut+1, tl.y), br, a2);

		} else if (size.y >= MIN_SPLIT_DIM) {
			int cut = tl.y + noise::Int(seed++, MIN_ROOM_DIM, size.y - MIN_ROOM_DIM - 1);
			int door = tl.x + noise::Int(seed++, MIN_DOOR_GAP, size.x - MIN_DOOR_GAP - 1);

			Access a1 = acc;
			a1.door[3] = door;
			generate(tl, pair(br.x, cut), a1);

			Access a2 = acc;
			a2.door[1] = door;
			generate(pair(tl.x, cut+1), br, a2);

		} else {
			// int w = noise::Int(seed++, MIN_ROOM_DIM, size.x + 1);
			// int h = noise::Int(seed++, MIN_ROOM_DIM, size.y + 1);
			int w = size.x;
			int h = size.y;
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
