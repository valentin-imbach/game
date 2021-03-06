//
//  maths.h
//  Game
//
//  Created by Valentin Imbach on 22.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once

template<typename T> struct pair {
    
    T X, Y;
    
    pair(T x = NULL, T y = NULL) {
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
    
    pair<T> operator+=(const pair<T> other) {
        X += other.X;
        Y += other.Y;
        return {X,Y};
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
    
    template <typename TT> pair<T> operator/(const TT other) const {
        return {X/other,Y/other};
    }
    
    operator bool() const {
        return (X != 0 && Y != 0);
    }
    
    bool operator<(const pair<T> other) const {
        return ((X == other.X) ? (Y < other.Y) : (X < other.X));
    }
    
    pair<int> rounded() const {
        return pair<int>(round(X),round(Y));
    }
    
};

template<typename T> std::ostream& operator<<(std::ostream& stream, const pair<T> p) {
    stream << "(" << p.X << ", " << p.Y << ")";
    return stream;
}

inline int randRange(int a, int b) {
    assert(a < b);
    return a + rand() % (b-a);
}

template<typename T> float dist(pair<T> a, pair<T> b) {
    return sqrt((b.X-a.X)*(b.X-a.X) + (b.Y-a.Y)*(b.Y-a.Y));
}

template<typename T> bool isIn(pair<T> a, pair<T> pos, pair<T> size) {
    return pos.X <= a.X && a.X <= pos.X+size.X &&  pos.Y <= a.Y && a.Y <= pos.Y+size.Y;
}
