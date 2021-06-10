//
// Created by csh on 2021/6/4.
//

#ifndef EXPCALC_VARIABLE_H
#define EXPCALC_VARIABLE_H
#include <string>

using namespace std;
class Variable {
public:
    string eval();
    bool isConst;
    Variable(string name, string val);

    const string &getVarName() const;

    void setVarName(const string &varName);

    const string &getVal() const;

    void setVal(const string &val);
private:
    string varName;
    string val;
};


#endif //EXPCALC_VARIABLE_H
