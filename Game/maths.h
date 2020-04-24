//
//  maths.h
//  Game
//
//  Created by Valentin Imbach on 22.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once

template<typename T> struct pair {
    
    T X = NULL;
    T Y = NULL;
    
    pair() {}
    pair(T x, T y) {
        X = x;
        Y = y;
    }
    template <typename TT> pair(pair<TT> p) {
        X = p.X;
        Y = p.Y;
    }
    
    void operator=(const pair<T> other) {
        X = other.X;
        Y = other.Y;
    }
    
    pair<T> operator+(const pair<T> other) const {
        return {X+other.X,Y+other.Y};
    }
    
    pair<T> operator-(const pair<T> other) const {
        return {X-other.X,Y-other.Y};
    }
    
    pair<T> operator*(const pair<T> other) const {
        return {X*other.X,Y*other.Y};
    }
    
    pair<T> operator*(const int other) const {
        return {X*other,Y*other};
    }
    
    pair<T> operator/(const int other) const {
        return {X/other,Y/other};
    }
    
    bool operator<(const pair<T> other) const {
        return ((X == other.X) ? (Y < other.Y) : (X < other.X));
    }
    
};

template<typename T> std::ostream& operator<<(std::ostream& stream, const pair<T> p) {
    stream << "(" << p.X << ", " << p.Y << ")";
    return stream;
}
