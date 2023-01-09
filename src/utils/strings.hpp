
#pragma once
#include <string>
#include <vector>
#include <array>

inline std::vector<std::string> string_split(const std::string& str, const char* del = " ") {
	std::vector<std::string> res;
	std::string s = str;
	char* token = strtok(&s[0], del);
	while (token != nullptr) {
		res.emplace_back(token);
		token = strtok(nullptr, del);
	}
	return res;
}

inline std::string& string_lower(std::string& str) {
	for (auto& c : str) c = char(std::tolower(c));
	return str;
}



inline bool string_is_int(const std::string& s) {
	return !s.empty() && std::all_of(s.begin() + 1, s.end(), ::isdigit) && (std::isdigit(s[0]) || s[0] == '-');
}

inline bool string_is_uint(const std::string& s) {
	return !s.empty() && std::all_of(s.begin(), s.end(), ::isdigit);
}

inline std::vector<std::string> string_split_and_lower(const std::string& str) {
	std::vector<std::string> res = string_split(str, ", ");
	for (auto& s : res) string_lower(s);
	return res;
}
