#pragma once
#include "random.hpp"
#include "strings.hpp"
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

#define ENUM(name, items...)                                                              \
	namespace name {                                                                      \
		enum value : uchar { NONE, items, MAX };                                          \
		constexpr uint count = value::MAX;                                                \
		inline value from_int(int n) {                                                    \
			assert(0 <= n && n < count);                                                  \
			return value(n);                                                              \
		}                                                                                 \
		const std::vector<std::string> strings = string::split_and_lower("NONE," #items); \
		inline value from_string(std::string str) {                                       \
			auto it = std::find(strings.begin(), strings.end(), str);                     \
			if (it == strings.end()) return value::NONE;                                  \
			return value(it - strings.begin());                                           \
		}                                                                                 \
		inline std::string to_string(value val) {                                         \
			return strings[val];                                                          \
		}                                                                                 \
		inline value random(uint seed) {                                                  \
			return value(noise::Int(seed, 1, count));                                     \
		}                                                                                 \
		};                                                                                \
	inline std::ostream& operator<<(std::ostream& s, name::value val) {                   \
		return s << '(' << to_string(val) << ')';                                         \
	}
