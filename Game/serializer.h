//
//  serializer.h
//  Game
//
//  Created by Valentin Imbach on 29.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once

#include "tools.h"

#define SERIALIZE(...)  void serialize(std::fstream& stream) {serialize_(stream, __VA_ARGS__);}\
                        void deserialize(std::fstream& stream) {deserialize_(stream, __VA_ARGS__);}

template <typename T> inline T* createDefeult(T* x) {
    return new T();
}

template <typename T> inline void serialize_(std::fstream& stream, T& x) {
    stream.write((char*)&x,sizeof(x));
    //LOG(x,sizeof(x));
}

template <typename T> inline void deserialize_(std::fstream& stream, T& x) {
    stream.read((char*)&x,sizeof(x));
    LOG(x,sizeof(x));
}

template <typename T, typename... Args> inline void serialize_(std::fstream& stream, T& x, Args & ... args) {
    serialize_(stream, x);
    serialize_(stream, args...);
}

template <typename T, typename... Args> inline void deserialize_(std::fstream& stream, T& x, Args & ... args) {
    deserialize_(stream, x);
    deserialize_(stream, args...);
}
