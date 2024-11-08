
#pragma once
#include "utils.hpp"

template <typename T>
inline T parse(std::string);

template <>
inline int parse<int>(std::string str) { return std::stoi(str); }

template <>
inline float parse<float>(std::string str) { return std::stof(str); }

template <>
inline std::string parse<std::string>(std::string str) { return str; }

template <typename T>
inline std::tuple<T> parseAll(typename std::vector<std::string>::iterator it) {
	return std::make_tuple(parse<T>(*it));
}

template <typename U, typename V, typename... T>
inline std::tuple<U, V, T...> parseAll(typename std::vector<std::string>::iterator it) {
	std::tuple<U> head = parse<U>(*it);
	std::tuple<V, T...> tail = parseAll<V, T...>(++it);
	return std::tuple_cat(head, tail);
}

inline std::tuple<> parseAll(typename std::vector<std::string>::iterator it) {
	return std::make_tuple();
}



struct ICommand {
	ICommand() = default;
	virtual bool execute(std::vector<std::string>& strings) = 0;
	virtual ~ICommand() = default;
};

template <typename... T>
struct Command : public ICommand {
	Command(std::function<bool(T...)> fun) : fun(fun) {}

	bool execute(std::vector<std::string>& strings) override {
		if (sizeof...(T) != strings.size()) {
			WARNING("Incorrect command argument number");
			return false;
		}

		if constexpr (sizeof...(T) == 0) {
			return fun();
		} else {
			return std::apply(fun, parseAll<T...>(strings.begin()));
		}
	}

	std::function<bool(T...)> fun;
};
