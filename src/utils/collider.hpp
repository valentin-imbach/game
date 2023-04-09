
#pragma once
#include "vec.hpp"
#include "enum.hpp"

ENUM(Shape,
RECTANGLE,
CIRCLE)

struct Collider {
	vec offset;
	Shape::value type;
	vec size;
	float radius;

	Collider() = default;
	Collider(vec offset, vec size) : offset(offset), type(Shape::RECTANGLE), size(size) {}
	Collider(vec offset, float radius) : offset(offset), type(Shape::CIRCLE), radius(radius) {}

	vec topBottom(vec position) {
		if (type == Shape::CIRCLE) {
			return {position.y + offset.y - radius, position.y + offset.y + radius};
		}
		return {position.y + offset.y, position.y + offset.y + size.y};
	}

	static bool RR(Collider A, vec a, Collider B, vec b) {
		bool xBA = (a.x + A.offset.x > b.x + B.offset.x + B.size.x);
		bool xAB = (b.x + B.offset.x > a.x + A.offset.x + A.size.x);
		bool yBA = (a.y + A.offset.y > b.y + B.offset.y + B.size.y);
		bool yAB = (b.y + B.offset.y > a.y + A.offset.y + A.size.y);
		return !xAB && !xBA && !yAB && !yBA;
	}

	static bool RC(Collider colliderA, vec positionA, Collider colliderB, vec positionB) {
		vec a = positionA + colliderA.offset;
		vec s = colliderA.size;
		vec b = positionB + colliderB.offset;
		float r = colliderB.radius;

		if (b.x + r < a.x || b.y + r < a.y || b.x > a.x + s.x + r || b.y > a.y + s.y + r) return false;
		if (b.x < a.x && b.y < a.y && dist(a, b) > r) return false;
		if (b.x < a.x && b.y > a.y + s.y && dist({a.x, a.y + s.y}, b) > r) return false;
		if (b.x > a.x + s.x && b.y < a.y && dist({a.x + s.x, a.y}, b) > r) return false;
		if (b.x > a.x + s.x && b.y > a.y + s.y && dist(a + s, b) > r) return false;
		return true;
	}

	static bool CC(Collider A, vec a, Collider B, vec b) {
		return dist(a + A.offset, b + B.offset) < A.radius + B.radius;
	}

	static bool colide(Collider A, vec a, Collider B, vec b) {
		if (A.type == Shape::RECTANGLE && B.type == Shape::RECTANGLE) return RR(A, a, B, b);
		if (A.type == Shape::CIRCLE && B.type == Shape::CIRCLE) return CC(A, a, B, b);

		if (A.type == Shape::RECTANGLE && B.type == Shape::CIRCLE) return RC(A, a, B, b);
		if (A.type == Shape::CIRCLE && B.type == Shape::RECTANGLE) return RC(B, b, A, a);
		return false;
	}
};