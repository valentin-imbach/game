
#include "Distribution.hpp"
#include "Environment.hpp"

PerlinNoise::PerlinNoise(uint seed, float per, float amp, float mean, int oct) : Distribution(seed), per(per), amp(amp), mean(mean), oct(oct) {}

float PerlinNoise::octave(uint seed, vec position) {
	pair steps[4] = {{0, 0}, {1, 0}, {0, 1}, {1, 1}};
	pair cell = vec::floor(position);
	vec offset = position - cell;
	float dots[4];
	for (int i = 0; i < 4; i++) {
		uint s = hash(seed, cell + steps[i]);
		vec unit = vec::polar(noise::Float(s, 2 * M_PI));
		dots[i] = vec::dot(unit, offset - steps[i]);
	}
	float left = lerp::smooth(offset.y, dots[0], dots[2]);
	float right = lerp::smooth(offset.y, dots[1], dots[3]);
	return lerp::smooth(offset.x, left, right);
}

float PerlinNoise::get(vec position) {
	uint s = seed;
	float p = per;
	float a = amp;
	float res = mean;
	for (int i = 0; i < oct; i++) {
		vec offset;
		offset.x = noise::Float(s++);
		offset.y = noise::Float(s++);
		res += a * octave(s++, offset + position / p);
		p /= 2;
		a /= 2;
	}
	return res;
}


ConstantDistribution::ConstantDistribution(float value) : Distribution(0), value(value) {}
float ConstantDistribution::get(vec position) {
	return value;
}

BoundDistribution::BoundDistribution(std::unique_ptr<Distribution> dist, float min, float max) : Distribution(0), distribution(std::move(dist)), min(min), max(max) {}

float BoundDistribution::get(vec position) {
	return std::max(min, std::min(max, distribution->operator()(position)));
}

// SpacedAllocation::SpacedAllocation(uint seed, int space, int radius) : Distribution(seed), space(space), radius(radius) {
// 	shift.x = noise::Float(seed + 1, space);
// 	shift.y = noise::Float(seed + 2, space);
// }

// bool SpacedAllocation::get(vec position) {
// 	pair cell = space * (vec::round(position + shift) / space);
// 	uint s = hash(seed, cell);
// 	float angle = noise::Float(s++, 2*M_PI);
// 	float dist = noise::Float(s++, radius);
// 	pair offset = vec::round(vec::polar(angle, dist));
// 	return position == cell + offset;
// }

TopoGraph::TopoGraph(uint seed, int space, int radius) : seed(seed), space(space), radius(radius), shift() {}

vec TopoGraph::position(pair node) {
	pair grid = space * node + shift;
	uint s = hash(seed, node);
	float angle = noise::Float(s++, 2*M_PI);
	float dist = noise::Float(s++, radius);
	pair offset = vec::round(vec::polar(angle, dist));
	return grid + offset;
}

pair TopoGraph::node(vec pos) {
	pair top_left = (vec::round(pos) - shift) / space;
	pair node = top_left;
	float dist = vec::dist(pos, position(node));
	for (pair step : {pair(1,0), pair(0,1), pair(1,1)}) {
		pair node2 = top_left + step;
		float dist2 = vec::dist(pos, position(node2));
		if (dist2 < dist) {
			node = node2;
			dist = dist2;
		};
	}
	return node;
}

Direction::value TopoGraph::down(pair node, Environment* environment) {
	Direction::value dir = Direction::NONE;
	float height = environment->elevationMap->get(position(node));
	for (int i = 1; i < Direction::count; i++) {
		pair node2 = node + Direction::taxi[i];
		float h = environment->elevationMap->get(position(node2));
		if (h < height) {
			dir = Direction::from_int(i);
			height = h;
		}
	}
	return dir;
}

bool TopoGraph::lake(pair node, Environment* environment) {
	float height = environment->elevationMap->get(position(node));
	return (height > 0 && down(node, environment) == Direction::NONE);
}

GroundId::value TopoGraph::ground(pair node, Environment* environment) {
	vec pos = position(node);
	float elevation = environment->elevationMap->get(pos);
	if (elevation < 0) return GroundId::WATER;
	if (lake(node, environment)) return GroundId::DIRT;
	return GroundId::GRASS;
}


Voronoi::Voronoi(uint seed, float space, float radius) : Distribution(seed), space(space), radius(radius) {
	shift.x = noise::Float(seed + 1, space);
	shift.y = noise::Float(seed + 2, space);
	assert(2 * radius < space);
}

pair Voronoi::ptoc(vec position) {
	return vec::round(position - shift) / space;
}

vec Voronoi::ctop(pair cell) {
	return space * vec(cell) + shift;
}

vec Voronoi::offset(pair cell) {
	uint s = hash(seed, cell);
	float angle = noise::Float(s++, 2 * M_PI);
	float dist = noise::Float(s++, radius);
	return vec::polar(angle, dist);
}

vec Voronoi::get(vec position) {
	pair c = ptoc(position);
	vec root = ctop(c) + offset(c);
	
	for (pair p : {pair(1,0), pair(0,1), pair(1,1)}) {
		pair nc = c + p;
		vec nr = ctop(nc) + offset(nc);
		if (vec::dist(nr, position) < vec::dist(root, position)) root = nr;
	}
	return root;
}


PerlinDistortion::PerlinDistortion(uint seed, float frequency, float radius) : Distribution(seed), angle(seed, 100, frequency, 0, 1), radius(radius) {}

vec PerlinDistortion::get(vec position) {
	float ang = angle(position);
	vec offset = vec::polar(ang, radius);
	return position + offset;
}