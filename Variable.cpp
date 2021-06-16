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


extern vector<pair<regex, string>> replace_list;
vector<Func*> Calculator::funcs;
vector<Variable*> Calculator::vars;
const regex DIGIT("[0-9|.]+");
const regex VARIABLE("[a-zA-Z]+");

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
        if(!c.Cac())
            return "";
        return to_string(c.getAns());
    }
    string temp = val;
    string target;
    for (auto i : replace_list) temp = regex_replace(temp, i.first, i.second);
    istringstream is(temp);
    vector<string> token;
    string t("");
    while (is >> t)
        token.push_back(t);
    if (token.size() == 1) {
        if (regex_match(token[0], regex("[0-9]+")))
            return token[0];
        Variable *tem = nullptr;
        for (int j = 0; j < Calculator::vars.size(); j++)
            if (token[0] == Calculator::vars[j]->getVarName()) {
                tem = Calculator::vars[j];
                break;
            }
        if (!tem) {
            cout << "There is no variable called " << token[0] << endl;
            return "";
        } else {
            string ans = tem->eval();
            if (ans.empty())
                cout << "表达式有误\n";
            else
                cout << ans << endl;
        }
        return "";
    }
    token.push_back("");
    for (int i = 0; i < token.size(); i++) {
        if(token[i] == ",") continue;
        if(token[i].empty())
            break;
        if(!regex_match(token[i], DIGIT)) {
            if(token[i + 1] == "(") {
                Func *f = nullptr;
                for(auto func : Calculator::funcs)
                    if(token[i] == func->name) {
                        f = func;
                        break;
                    }
                i++;
                int left = 1, right = 0;
                string args;
                while(left != right) {
                    if(token[i + 1] == "(") left++;
                    else if(token[i + 1] == ")") right++;
                    if(right == left)
                        break;
                    args += token[i + 1];
                    i++;
                }
                target += f->eval(args);
                i++;
            }
            else {
                if(regex_match(token[i], VARIABLE)) {
                    Variable * var;
                    for(auto v : Calculator::vars)
                        if(v->getVarName() == token[i]) {
                            var = v;
                            break;
                        }
                    target += var->eval();
                }
                else
                    target += token[i];
            }
        }
        else
            target += token[i];
    }
    c.setExp(const_cast<char *>((target + "=").c_str()));
    if(!c.Cac())
        return "";
    return to_string(c.getAns());
}

Variable::Variable(string name, string val) {
    this->varName = name;
    this->val = val;
}
