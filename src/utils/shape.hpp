
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

	Shape(vec offset, vec size) : offset(offset), type(ShapeId::RECTANGLE), size(size), radius() {}
	Shape(vec offset, float radius) : offset(offset), type(ShapeId::CIRCLE), size(), radius(radius) {}

	vec topBottom(vec position) {
		if (type == ShapeId::CIRCLE) {
			return vec(position.y + offset.y - radius, position.y + offset.y + radius);
		} else if (type == ShapeId::RECTANGLE) {
			return vec(position.y + offset.y - size.y/2, position.y + offset.y + size.y/2);
		} else {
			WARNING("Unhandled Shape");
			return pair(0, 0);
		}
	}

	static bool RR(Shape shapeA, vec posA, Shape shapeB, vec posB) {
		vec a = posA + shapeA.offset;
		vec b = posB + shapeB.offset;
		vec sa = shapeA.size;
		vec sb = shapeB.size;
		bool xAB = (a.x + sa.x/2 < b.x - sb.x/2);
		bool xBA = (b.x + sb.x/2 < a.x - sa.x/2);
		bool yAB = (a.y + sa.y/2 < b.y - sb.y/2);
		bool yBA = (b.y + sb.y/2 < a.y - sa.y/2);
		return !xAB && !xBA && !yAB && !yBA;
	}

	static bool RC(Shape shapeA, vec posA, Shape shapeB, vec posB) {
		vec a = posA + shapeA.offset;
		vec b = posB + shapeB.offset;
		vec s = shapeA.size;
		float r = shapeB.radius;

		vec tl(a.x - s.x/2, a.y - s.y/2);
		vec tr(a.x + s.x/2, a.y - s.y/2);
		vec bl(a.x - s.x/2, a.y + s.y/2);
		vec br(a.x + s.x/2, a.y + s.y/2);

		if (b.x <= tl.x && b.y <= tl.y) return vec::dist(b, tl) < r;
		if (b.x <= bl.x && b.y >= bl.y) return vec::dist(b, bl) < r;
		if (b.x >= tr.x && b.y <= tr.y) return vec::dist(b, tr) < r;
		if (b.x >= br.x && b.y >= br.y) return vec::dist(b, br) < r;
		return (b.x + r > a.x - s.x/2 && b.y + r > a.y - s.y/2 && b.x - r < a.x + s.x/2 && b.y - r < a.y + s.y/2);
	}

	static bool CC(Shape shapeA, vec posA, Shape shapeB, vec posB) {
		vec a = posA + shapeA.offset;
		vec b = posB + shapeB.offset;
		return vec::dist(a, b) < shapeA.radius + shapeB.radius;
	}

	static bool colide(Shape shapeA, vec posA, Shape shapeB, vec posB) {
		if (shapeA.type == ShapeId::RECTANGLE && shapeB.type == ShapeId::RECTANGLE) return RR(shapeA, posA, shapeB, posB);
		if (shapeA.type == ShapeId::CIRCLE && shapeA.type == ShapeId::CIRCLE) return CC(shapeA, posA, shapeB, posB);
		if (shapeA.type == ShapeId::RECTANGLE && shapeA.type == ShapeId::CIRCLE) return RC(shapeA, posA, shapeB, posB);
		if (shapeA.type == ShapeId::CIRCLE && shapeA.type == ShapeId::RECTANGLE) return RC(shapeB, posB, shapeA, posA);
		WARNING("Shape not handled");
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