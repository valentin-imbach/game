//
//  logger.h
//  Game
//
//  Created by Valentin Imbach on 22.04.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once

template <typename T> void PRINT(T x) {
    std::cout << x << std::endl;
}

template <typename T, typename... Args> void PRINT(T x, Args... args) {
    std::cout << x << " ";
    PRINT(args...);
}

template <typename... Args> void LOG(Args... args) {
    std::cout << "  LOG: ";
    PRINT(args...);
}

template <typename... Args> void ERROR(Args... args) {
    std::cout << "    ERROR: ";
    PRINT(args...);
}
