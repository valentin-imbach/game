//
//  tools.h
//  Game
//
//  Created by Valentin Imbach on 2019-09-19.
//  Copyright © 2019 Valentin Imbach. All rights reserved.
//

#pragma once

#include <ctime>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <array>

#define v(x) std::vector<x>
#define vv(x) std::vector<std::vector<x>>
#define pint std::pair<int,int>
#define pout std::pair<float,float>
#define X first
#define Y second

#define vup(x) std::vector<std::unique_ptr<x>>
#define vvup(x) std::vector<std::vector<std::unique_ptr<x>>>

#define PRINT(x) std::cout << x << std::endl
#define LOG(x) std::cout << "  LOG: " << x << std::endl
#define LOG2(x,y) std::cout << "  LOG: " << x << " " << y << std::endl
#define LOGP(x) std::cout << "  LOG: " << x.first << " " << x.second << std::endl
#define ERROR(x) std::cout << "ERROR: " << x << std::endl
#define ERROR2(x,y) std::cout << "ERROR: " << x << " (" << y << ")" << std::endl

#define DIST(a,b) sqrt(pow(a.X-b.X,2)+pow(a.Y-b.Y,2))
#define sign(x) (x == 0 ? 0 : abs(x)/x)

const pint dirs[4] = {{1,0},{0,-1},{-1,0},{0,1}};
const pint dirs2[8] = {{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1},{1,1}};

struct Timer {
    long double t;
    Timer() {
        t = clock();
    }
    ~Timer() {
        std::cout << "TIMER: " << clock()-t << std::endl;
    }
};

