//
//  serializer.h
//  Game
//
//  Created by Valentin Imbach on 29.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once

#include "tools.h"

#define SERIALIZE(...)  int get_size() override { return get_size_(__VA_ARGS__); }\
                        void serialize(std::fstream& stream) override { serialize_(stream, __VA_ARGS__); }\
                        void deserialize(std::fstream& stream) override { deserialize_(stream, __VA_ARGS__); }

template <typename T> inline void serialize_(std::fstream& stream, T& x) {
    stream.write((char*)&x,sizeof(x));
    //LOG("Serialzed",(int)x,sizeof(x));
}

template <typename T, typename... Args> inline void serialize_(std::fstream& stream, T& x, Args & ... args) {
    serialize_(stream, x);
    serialize_(stream, args...);
}


template <typename T> inline void deserialize_(std::fstream& stream, T& x) {
    stream.read((char*)&x,sizeof(x));
    //LOG("Deserialized",(int)x,sizeof(x));
}

template <typename T, typename... Args> inline void deserialize_(std::fstream& stream, T& x, Args & ... args) {
    deserialize_(stream, x);
    deserialize_(stream, args...);
}


template <typename T> inline int get_size_(T& x) {
    return (int)sizeof(x);
}

template <typename T, typename... Args> inline int get_size_(T& x, Args & ... args) {
    return (get_size_(x) + get_size_(args...));
}


struct Serializable {
    virtual void serialize(std::fstream& stream) {}
    virtual void deserialize(std::fstream& stream) {}
    virtual int get_size() { return 0; }
};
