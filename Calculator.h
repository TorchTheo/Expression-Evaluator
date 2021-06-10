//
// Created by csh on 2021/6/4.
//

#ifndef EXPCALC_CALCULATOR_H
#define EXPCALC_CALCULATOR_H

#include "Func.h"
#include "Variable.h"
#include "Calc.h"
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace std;

enum ACTION {
    CREATE_VARIABLE, SHOW_VALUE, ASSIGN_VALUE, CREATE_FUNCTION
};

class Calculator {
public:
//    Calculator();
    static vector<Func*> funcs;
    static vector<Variable*> vars;
//    static map<string, int> operatorRank;
    void doSth();
    int analyseLine(string& line);
    void create_variable(string& line);
    void show_value(string &line);
    void assign_value(string &line);
    void create_function(string &line);

};

#endif //EXPCALC_CALCULATOR_H
