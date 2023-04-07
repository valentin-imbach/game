#pragma once
#include <iostream>
#include <vector>
#include "vec.hpp"
#include "pair.hpp"
#include "strings.hpp"

#define LOG(args...) log(std::clog, "LOG", __FILE__, __LINE__, args);
#define ERROR(args...) log(std::cerr, "ERROR", __FILE__, __LINE__, args);
#define WARNING(args...) log(std::cerr, "WARNING", __FILE__, __LINE__, args);

inline std::ostream& operator<<(std::ostream& s, vec v) {
	return s << "vec(" << v.x << ", " << v.y << ')';
}

inline std::ostream& operator<<(std::ostream& s, pair p) {
	return s << "pair(" << p.x << ", " << p.y << ')';
}

template <typename T>
inline std::ostream& operator<<(std::ostream& s, std::vector<T> v) {
	if (v.empty()) return s << "{}";
	s << '{' << v[0];
	for (int i = 1; i < v.size(); i++) s << ", " << v[i];
	return s << '}';
}

template <typename T>
void debug_log(std::ostream& stream, T x) { stream << x; }

template <typename T, typename... TT>
void debug_log(std::ostream& stream, T x, TT... y) {
	debug_log(stream, x);
	debug_log(stream, ' ');
	debug_log(stream, y...);
}

template <typename... T>
void log(std::ostream& stream, const char* title, const char* file, int line, T... args) {
	debug_log(stream, title, '@', string_split(file, "/").back(), line);
	debug_log(stream, ":", args..., '\n');
}