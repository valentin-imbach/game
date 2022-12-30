
#pragma once
#include <fstream>

template <typename T>
inline void serialise_object(std::fstream& stream, T& object) {
	stream.write(reinterpret_cast<char*>(&object), sizeof(T));
}

template <typename T>
inline void deserialise_object(std::fstream& stream, T& object) {
	stream.read(reinterpret_cast<char*>(&object), sizeof(T));
}