
#pragma once
#include "utils.hpp"

struct IArgument {
	virtual ~IArgument() {}
};

template <typename T>
struct Argument : public IArgument {
	T value;
	T parse(std::string str) {
		return T::from_string(str);
	}
};

struct Command {
	std::vector<IArgument> args;
	Command(std::vector<IArgument> args = {}) : args(args) {}
};