//
//  strings.h
//  Game
//
//  Created by Valentin Imbach on 27.05.20.
//  Copyright © 2020 Valentin Imbach. All rights reserved.
//

#pragma once

inline std::vector<std::string> splitString(const std::string& s) {
    std::vector<std::string> res = std::vector<std::string>();
    int last = -1;
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == ' ') {
            res.push_back(s.substr(last+1,i-last-1));
            last = i;
        }
    }
    res.push_back(s.substr(last+1));
    return res;
}

inline bool isNumber(const std::string& s) {
    return !s.empty() && std::all_of(s.begin()+1, s.end(), ::isdigit) && (std::isdigit(s[0]) || s[0] == '-');
}
