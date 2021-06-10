//
// Created by csh on 2021/6/4.
//

#include <sstream>
#include "Variable.h"
#include "Calc.h"
#include "Func.h"
#include <regex>
#include "Calculator.h"
using namespace std;

vector<Func*> Calculator::funcs;
vector<Variable*> Calculator::vars;

const string &Variable::getVarName() const {
    return varName;
}

void Variable::setVarName(const string &varName) {
    Variable::varName = varName;
}

const string &Variable::getVal() const {
    return val;
}

void Variable::setVal(const string &val) {
    Variable::val = val;
}

string Variable::eval() {
    Calc c;
    if(isConst) {
        c.setExp(const_cast<char *>((val + "=").c_str()));
        c.Cac();
        return to_string(c.getAns());
    }
    string temp = val;
    string target = val;
    for(int i = 0; i < temp.size(); i++)
        if(temp[i] == '+' || temp[i] == '-' || temp[i] == '*' || temp[i] == '/' || temp[i] == '(' || temp[i] == ')')
            temp[i] = ' ';
    istringstream is(temp);
    vector<string> token;
    string t("");
    while(is >> t)
        token.push_back(t);
    for(int i = 0; i < token.size(); i ++) {
        regex r("^[0-9|.]+$");
        if(!regex_match(token[i], r)) {
            Variable *var = nullptr;
            for(int j = 0; j < Calculator::vars.size(); j++)
                if(token[i] == Calculator::vars[j]->getVarName()) {
                    var = Calculator::vars[j];
                    break;
                }
            if(var) {
                string varVal = var->eval();
                int pos = target.find(var->getVarName());
                target.replace(pos, var->getVarName().length(), varVal);
                continue;
            }
            Func* f = nullptr;
            for(int j = 0; j < Calculator::funcs.size(); j++)
                if(Calculator::funcs[j]->name == token[i]) {
                    f = Calculator::funcs[j];
                    break;
                }
            if(f) {
                string sub = f->name + "(";
                for(int k = 0; k < f->arguments_size; k++) {
                    f->arguments[k] = token[i + k + 1];
                    sub += token[i + k + 1] + ",";
                    i++;
                }
                string func_val = f->eval();
                sub.pop_back();
                sub += ")";
                int pos = target.find(sub);
                target.replace(pos, sub.length(), func_val);
                continue;
            }
        }
    }
    c.setExp(const_cast<char *>((target + "=").c_str()));
    c.Cac();
    return to_string(c.getAns());
}

Variable::Variable(string name, string val) {
    this->varName = name;
    this->val = val;
}
