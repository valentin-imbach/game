
#pragma once
#include "random.hpp"
#include "vec.hpp"
#include "lerp.hpp"
#include "direction.hpp"

template <typename T>
[[nodiscard]] inline uint hash(T x) {
	return noise::UInt(uint(x));
}

template <typename T, typename... TT>
[[nodiscard]] inline uint hash(T first, TT... args) {
	return hash(first) ^ (hash(args...) << 1);
}

template <>
[[nodiscard]] inline uint hash(pair x) {
	return hash(x.x, x.y);
}

template <typename T>
class Distribution {
public:
	Distribution(uint seed) : seed(seed) {}
	virtual T get (vec position) = 0;
	T operator () (vec position) { return get(position); }
	virtual ~Distribution() = default;
protected:
	uint seed;
};

class PerlinNoise : public Distribution<float> {
public:
	PerlinNoise(uint seed = 0, float per = 1, float amp = 1, float mean = 0, int oct = 1);
	float get(vec position) override;

private:
	float octave(uint seed, vec position);
	float per;
	float amp;
	float mean;
	int oct;
};

class ConstantDistribution : public Distribution<float> {
public:
	ConstantDistribution(float value = 0);
	float get(vec position) override;

private:
	float value;
};

class BoundDistribution : public Distribution<float> {
public:
	BoundDistribution(std::unique_ptr<Distribution> dist, float min, float max);
	float get(vec position) override;

private:
	std::unique_ptr<Distribution> distribution;
	float min;
	float max;
};

// class SpacedAllocation : public Distribution<bool> {
// public:
// 	SpacedAllocation(uint seed, int space, int radius);
// 	bool get(vec position) override;

// private:
// 	int space, radius;
// 	vec shift;
// };

#include "definitions.hpp"
class Environment;

struct TopoGraph {
	TopoGraph(uint seed, int space, int radius);
	pair node(vec pos);
	vec position(pair node);
	GroundId::value ground(pair node, Environment* environment);
	Direction::value down(pair node, Environment* environment);
	bool lake(pair node, Environment* environment);

private:
	uint seed;
	int space;
	int radius;
	pair shift;
};

class Voronoi : public Distribution<vec> {
public:
	Voronoi(uint seed, float space, float radius);
	vec get(vec position) override;

private:
	float space;
	float radius;
	vec shift;

	pair ptoc(vec position);
	vec ctop(pair cell);
	vec offset(pair cell);
};

class PerlinDistortion : public Distribution<vec> {
public:
	PerlinDistortion(uint seed);
	vec get(vec position) override;

private:
	PerlinNoise angle;
};