#pragma once
#include "random.hpp"
#include "strings.hpp"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#define ENUM(name, items...)                                                         \
	namespace name {                                                                 \
	enum value : uint8_t { NONE,                                                     \
		items,                                                                       \
		MAX };                                                                       \
	constexpr size_t count = value::MAX;                                             \
	inline value from_int(int n) {                                                   \
		assert(0 <= n && n < value::MAX);                                            \
		return value(n);                                                             \
	}                                                                                \
	const std::vector<std::string> strings = string_split_and_lower("NONE," #items); \
	inline value from_string(std::string str) {                                      \
		auto it = std::find(strings.begin(), strings.end(), str);                    \
		if (it == strings.end()) return value::NONE;                                 \
		return value(it - strings.begin());                                          \
	}                                                                                \
	inline std::string to_string(value val) {                                        \
		return strings[val];                                                         \
	}                                                                                \
	inline value random(uint seed) {                                                 \
		return value(rand_int(seed, 1, value::MAX));                                 \
	}                                                                                \
	};                                                                               \
	inline std::ostream& operator<<(std::ostream& s, name::value val) {              \
		return s << '(' << to_string(val) << ')';                                    \
	}
