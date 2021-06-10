//
// Created by csh on 2021/6/4.
//

#ifndef EXPCALC_FUNC_H
#define EXPCALC_FUNC_H
#include <iostream>
#include <vector>
#include<sstream>
#include <regex>
#include <map>
#include <queue>
using namespace std;
class Func {
public:
    string eval();
    map<string, int> trans;
    map<int, string> arguments;
    string body;
    string name;
    int arguments_size;

};


#endif //EXPCALC_FUNC_H
