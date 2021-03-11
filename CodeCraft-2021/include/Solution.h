//
// Created by zcz on 2021/3/11.
//

#ifndef CODECRAFT_2021_SOLUTION_H
#define CODECRAFT_2021_SOLUTION_H
#include <iostream>
#include "common.h"

class Solution {
public:
    Solution(std::string &filename);
    void run();
    void input();
    void write_answer();
public:
    struct Server mServer;
    std::string mFilename;
};


#endif //CODECRAFT_2021_SOLUTION_H
