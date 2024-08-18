
#pragma once

#include <variant>
#include <string>
#include <vector>
#include <map>
#include <fstream>

namespace json {

struct Value;
typedef std::vector<Value> Array;
typedef std::map<std::string, Value> Object;
typedef std::monostate Null;
typedef std::string::iterator StrIt;

struct Error {
	Error(std::string text) : text(text) {}
	std::string text;
};

class Value {
public:
	Value() {}
	template <typename T>
	Value(T t) : variant(t) {}

	template <typename T>
	T get() {
		return std::get<T>(variant);
	}

	template <typename T>
	bool is() const {
		return std::holds_alternative<T>(variant);
	}

	Value operator [](int i) {
		if (!is<Array>()) return Value(Error("Tried to index non-array"));
		Array arr = get<Array>();
		if (0 > i || i >= arr.size()) return Value(Error("Array index out of bounds"));
		return arr[i];
	}

	Value operator [](const char* key) {
		if (!is<Object>()) return Value(Error("Tried to index non-object"));
		Object obj = get<Object>();
		if (obj.find(key) == obj.end()) return Value(Error("Key not present in Object"));
		return obj[key];
	}

	size_t size() {
		if (is<Array>()) return get<Array>().size();
		if (is<Object>()) return get<Object>().size();
		return 0;
	}

	explicit operator int() {
		assert(is<int>());
		return get<int>();
	}

	explicit operator float() {
		if(is<float>()) return get<float>();
		if(is<int>()) return get<int>();
		assert(false);
	}

	explicit operator std::string() {
		assert(is<std::string>());
		return get<std::string>();
	}

	std::string print(int tabs = 0, bool indent = true) {
		std::string str;
		if (indent) for (int i = 0; i < tabs; i++) str += "  ";
		switch (variant.index()) {
		case 0:
			return str + "null";
		case 1:
			return str + std::to_string(get<int>());
		case 2:
			return str + std::to_string(get<float>());
		case 3:
			return str + (get<bool>() ? "true" : "false");
		case 4:
			return str + "\"" + get<std::string>() + "\"";
		case 5:
			str += "[";
			for (int i = 0; i < std::get<Array>(variant).size(); i++) {
				str += "\n" + std::get<Array>(variant)[i].print(tabs + 1);
				if (i + 1 < std::get<Array>(variant).size()) str += ",";
			}
			str += '\n';
			for (int i = 0; i < tabs; i++) str += "  ";
			str += "]";
			return str;
		case 6:
			if (std::get<Object>(variant).empty()) return "{}";
			str += "{";
			for (auto& [key, val] : std::get<Object>(variant)) {
				str += "\n";
				for (int i = 0; i < tabs + 1; i++) str += "  ";
				str += "\"" + key + "\": " + val.print(tabs + 1, false) + ",";
			}
			str[str.size() - 1] = '\n';
			for (int i = 0; i < tabs; i++) str += "  ";
			str += "}";
			return str;
		case 7:
			Error e = std::get<Error>(variant);
			return "Error(" + e.text + ")";
		}
		return "Error";
	}

	operator bool() const {
		return !std::holds_alternative<Error>(variant);
	}

private:
	std::variant<Null, int, float, bool, std::string, Array, Object, Error> variant;
};

inline Value parseString(std::string& str, StrIt& it) {
	assert(*(it++) == '"');
	StrIt start = it;
	while (*it != '"' && it < str.end()) it++;
	if (it == str.end()) return Value(Error("String never ends"));
	return Value(str.substr(start - str.begin(), (it++) - start));
}

inline Value parseNumber(std::string& str, StrIt& it) {
	StrIt start = it;
	assert(isdigit(*it) || *it == '.' || *it == '-');
	bool fp = false;
	if (*it == '-') it++;
	while (it < str.end()) {
		if (isdigit(*it)) {
			it++;
		} else if (*it == '.' && !fp) {
			fp = true;
			it++;
		} else {
			break;
		}
	}
	std::string value = str.substr(start - str.begin(), it - start);
	if (fp) return Value(std::stof(value));
	return Value(std::stoi(value));
}

inline Value parseBool(std::string& str, StrIt& it) {
	if (it + 3 < str.end() && std::equal(it, it + 4, "true")) {
		it += 4;
		return Value(true);
	}
	if (it + 4 < str.end() && std::equal(it, it + 5, "false")) {
		it += 5;
		return Value(false);
	}
	return Value(Error("Misspelled boolean?"));
}

inline Value parse(std::string& str, StrIt& it);

inline Value parseArray(std::string& str, StrIt& it) {
	Array array;
	assert(*(it++) == '[');
	while(std::isspace(*it) && it < str.end()) it++;
	if (*it == ']') {
		it++;
		return Value(array);
	}
	while (it < str.end()) {
		Value v = parse(str, it);
		if (!v) return v;
		array.emplace_back(v);
		while(std::isspace(*it) && it < str.end()) it++;
		if (it == str.end()) break;
		if (*it == ']') {
			it++;
			return Value(array);
		} else if (*(it++) != ',') return Value(Error("Expected comma"));
	}
	return Value(Error("Array never ends"));
}

inline Value parseObject(std::string& str, StrIt& it) {
	Object object;
	assert(*(it++) == '{');
	while(std::isspace(*it) && it < str.end()) it++;
	if (*it == '}') {
		it++;
		return Value(object);
	}
	while(it < str.end()) {
		Value key = parse(str, it);
		if (!key.is<std::string>()) return Value(Error("Expected object key"));
		std::string ks = key.get<std::string>();
		if (object.find(ks) != object.end()) return Value(Error("Duplicate object key"));
		while(std::isspace(*it) && it < str.end()) it++;
		if (it >= str.end() || *(it++) != ':') return Value(Error("Expected colon"));
		Value val = parse(str, it);
		if (!val) return val;
		object[ks] = val;
		while(std::isspace(*it) && it < str.end()) it++;
		if (it == str.end()) break;
		if (*it == '}') {
			it++;
			return Value(object);
		} else if (*(it++) != ',') return Value(Error("Expected comma"));;
	}
	return Value(Error("Object never ends"));
}

inline Value parse(std::string& str, StrIt& it) {
	while(std::isspace(*it) && it < str.end()) it++;
	if (it == str.end()) return Value();
	if (*it == '{') return parseObject(str, it);
	if (*it == '[') return parseArray(str, it);
	if (*it == '"') return parseString(str, it);
	if (*it == 't' || *it == 'f') return parseBool(str, it);
	if (isdigit(*it) || *it == '.' || *it == '-') return parseNumber(str, it);
	return Value();
}

inline Value parseFile(const std::string& filepath) {
	std::string data;
	std::vector<size_t> lbs = {0};

	std::ifstream file(filepath);
	std::string line;
	while (std::getline(file, line)) {
		// line.erase(std::remove_if(line.begin(), line.end(), isspace), line.end());
		lbs.push_back(lbs.back() + line.size());
		data.append(line);
	}
	file.close();

	StrIt it = data.begin();
	Value res = parse(data, it);
	size_t charsParsed = it - data.begin();
	// if (!res) {
	// 	int l = 0;
	// 	while (lbs[l] < charsParsed) l++;
	// 	std::cout << "Error on line " << l << std::endl;
	// }
	return res;
}

}
