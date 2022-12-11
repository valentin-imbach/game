
#pragma once
#include "vec.hpp"

struct Collider {
	vec offset;
	vec size;

	static bool AABB(Collider colliderA, vec positionA, Collider colliderB, vec positionB) {
		bool xBA = (positionA.x + colliderA.offset.x > positionB.x + colliderB.offset.x + colliderB.size.x);
		bool xAB = (positionB.x + colliderB.offset.x > positionA.x + colliderA.offset.x + colliderA.size.x);
		bool yBA = (positionA.y + colliderA.offset.y > positionB.y + colliderB.offset.y + colliderB.size.y);
		bool yAB = (positionB.y + colliderB.offset.y > positionA.y + colliderA.offset.y + colliderA.size.y);
		return !xAB && !xBA && !yAB && !yBA;
	}
};