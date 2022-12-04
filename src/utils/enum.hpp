#pragma once
#include <cassert>
#include <string>
#include <vector>
#include "strings.hpp"

#define ENUM(name, items...) struct name {\
    enum impl : unsigned short {NONE, items, MAX};\
    static impl from_int(int val) {\
        assert(0 <= val && val < impl::MAX);\
        return impl(val);\
    }\
    static impl from_string(std::string str) {\
        auto it = std::find(strings.begin(), strings.end(), str);\
        if (it == strings.end()) return impl::NONE;\
        return impl(it - strings.begin());\
    }\
    static std::string to_string(impl val) {\
        return strings[val];\
    }\
    static constexpr int size = impl::MAX - 1;\
    static const std::vector<std::string> strings;\
};\
const std::vector<std::string> name::strings = string_split_and_lower("NONE," #items);