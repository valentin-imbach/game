//
//  logger.hpp
//  Game
//
//  Created by Valentin Imbach on 07.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once
#include "iostream"

template <typename T> inline void PRINT(T x) {
    std::cout << x << std::endl;
}

template <typename T, typename... Args> inline void PRINT(T x, Args... args) {
    std::cout << x << " ";
    PRINT(args...);
}

template <typename... Args> inline void LOG(Args... args) {
    std::cout << "  LOG: ";
    PRINT(args...);
}

template <typename... Args> inline void ERROR(Args... args) {
    std::cout << "    ERROR: ";
    PRINT(args...);
}

template <typename... Args> inline void WARNING(Args... args) {
    std::cout << "    WARNING: ";
    PRINT(args...);
}
