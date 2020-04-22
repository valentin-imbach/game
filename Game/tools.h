//
//  tools.h
//  Game
//
//  Created by Valentin Imbach on 2019-09-19.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#ifndef TOOLS
#define TOOLS

#include <ctime>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <array>

#include "FastNoise.h"
#include "maths.h"

#define v(x) std::vector<x>
#define vv(x) std::vector<std::vector<x>>

#define vup(x) std::vector<std::unique_ptr<x>>
#define vvup(x) std::vector<std::vector<std::unique_ptr<x>>>

#define PRINT(x) std::cout << x << std::endl
#define LOG(x) std::cout << "  LOG: " << x << std::endl
#define ERROR(x) std::cout << "    ERROR: " << x << std::endl

const pair<int> dirs[4] = {{1,0},{0,-1},{-1,0},{0,1}};
const pair<int> dirs2[8] = {{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1},{1,1}};

struct Timer {
    long double t;
    std::string name;
    Timer(std::string s) {
        t = clock();
        name = s;
    }
    ~Timer() {
        std::cout << "  TIMER (" << name << "): " << clock()-t << " ms" << std::endl;
    }
};

#endif
