
#pragma once
#include "vec.hpp"
#include "enum.hpp"
#include "logger.hpp"

ENUM(ShapeId,
RECTANGLE,
CIRCLE)

struct Shape {
	vec offset;
	ShapeId::value type;
	vec size;
	float radius;

	Shape() = default;
	Shape(vec offset, vec size) : offset(offset), type(ShapeId::RECTANGLE), size(size), radius() {}
	Shape(vec offset, float radius) : offset(offset), type(ShapeId::CIRCLE), size(), radius(radius) {}

	vec topBottom(vec position) {
		if (type == ShapeId::CIRCLE) {
			return vec(position.y + offset.y - radius, position.y + offset.y + radius);
		} else if (type == ShapeId::RECTANGLE) {
			return vec(position.y + offset.y, position.y + offset.y + size.y);
		} else {
			WARNING("Unhandled Shape");
			return pair(0, 0);
		}
	}

	static bool RR(Shape A, vec a, Shape B, vec b) {
		bool xBA = (a.x + A.offset.x > b.x + B.offset.x + B.size.x);
		bool xAB = (b.x + B.offset.x > a.x + A.offset.x + A.size.x);
		bool yBA = (a.y + A.offset.y > b.y + B.offset.y + B.size.y);
		bool yAB = (b.y + B.offset.y > a.y + A.offset.y + A.size.y);
		return !xAB && !xBA && !yAB && !yBA;
	}

	static bool RC(Shape colliderA, vec positionA, Shape colliderB, vec positionB) {
		vec a = positionA + colliderA.offset;
		vec s = colliderA.size;
		vec b = positionB + colliderB.offset;
		float r = colliderB.radius;

		if (b.x + r < a.x || b.y + r < a.y || b.x > a.x + s.x + r || b.y > a.y + s.y + r) return false;
		if (b.x < a.x && b.y < a.y && vec::dist(a, b) > r) return false;
		if (b.x < a.x && b.y > a.y + s.y && vec::dist(vec(a.x, a.y + s.y), b) > r) return false;
		if (b.x > a.x + s.x && b.y < a.y && vec::dist(vec(a.x + s.x, a.y), b) > r) return false;
		if (b.x > a.x + s.x && b.y > a.y + s.y && vec::dist(a + s, b) > r) return false;
		return true;
	}

	static bool CC(Shape A, vec a, Shape B, vec b) {
		return vec::dist(a + A.offset, b + B.offset) < (A.radius + B.radius);
	}

	static bool colide(Shape A, vec a, Shape B, vec b) {
		if (A.type == ShapeId::RECTANGLE && B.type == ShapeId::RECTANGLE) return RR(A, a, B, b);
		if (A.type == ShapeId::CIRCLE && B.type == ShapeId::CIRCLE) return CC(A, a, B, b);
		if (A.type == ShapeId::RECTANGLE && B.type == ShapeId::CIRCLE) return RC(A, a, B, b);
		if (A.type == ShapeId::CIRCLE && B.type == ShapeId::RECTANGLE) return RC(B, b, A, a);
		return false;
	}

	static bool inside(vec point, Shape shape, vec position) {
		if (shape.type == ShapeId::CIRCLE) {
			return vec::dist(point, position + shape.offset) < shape.radius;
		} else if (shape.type == ShapeId::RECTANGLE) {
			return vec::inside(point, position + shape.offset, shape.size);
		} else {
			WARNING("Shape not handled");
			return false;
		}
	}
};