//
// Created by csh on 2021/6/4.
//

/*
 * Var x = 4
 * Var y = x + 4
 * Func f(z) = z + 1 + z / 2
 * Func g(a, b) = x / a + y / b
 * g(f(1),f(2))/f(1)
 */

#include "Calculator.h"
#include <iostream>
#include <regex>
#include <string>

using namespace std;

extern vector<pair<regex, string>> replace_list;

const regex CREATEVAR("Var\\s+[a-zA-Z]+\\s*=\\s*(.*?)");
const regex ASSIGNVAL("([a-zA-Z]+)\\s*=\\s*(.*?)");
const regex CREATEFUNC("Func\\s+(.*?)\\((.*?)\\)\\s*=\\s*(.*?)");
const regex DIGIT("[0-9|.]+");
const regex VARIABLE("[a-zA-Z]+");

bool Calculator::check(string line) {
    if (count(line.begin(), line.end(), '(') != count(line.begin(), line.end(), ')')) {
        cout << "\033[31mERROR: PARENTHESIS DON'T MATCH\n\033[0m";
        return false;
    }


    return true;
}

void Calculator::doSth() {
    cout << "ExpCal > ";
    string line;
    getline(cin, line);
    if (!check(line))
        return;
    switch (analyseLine(line)) {
        case CREATE_VARIABLE:
            create_variable(line);
            break;
        case ASSIGN_VALUE:
            assign_value(line);
            break;
        case CREATE_FUNCTION:
            create_function(line);
            break;
        case SHOW_VALUE:
            show_value(line);
            break;
        default:
            cout << "\033[31mERROR: UNKNOWN EXPRESSION\n\033[0m";
            break;
    }
}


int Calculator::analyseLine(string &line) {
    if (regex_match(line, CREATEVAR))
        return ACTION::CREATE_VARIABLE;
    else if (regex_match(line, ASSIGNVAL))
        return ACTION::ASSIGN_VALUE;
    else if (regex_match(line, CREATEFUNC))
        return ACTION::CREATE_FUNCTION;
    else
        return ACTION::SHOW_VALUE;
}

void Calculator::create_variable(string &s) {
    string line = s;
    line.erase(0, 3);
    int index = 0;
    if (!line.empty())
        while ((index = line.find(' ', index)) != string::npos)
            line.erase(index, 1);
    for (int i = 0; i < line.length(); i++)
        if (line[i] == '=')
            line[i] = ' ';
    istringstream is(line);
    string Var, name, val;
    is >> name >> val;
    for (auto i : vars)
        if (i->getVarName() == name) {
            cout << "\033[31mERROR: VARIABLE " + i->getVarName() + " EXISTED\n\033[0m";
            return;
        }

    Variable *var = new Variable(name, val);
    if (regex_match(val, regex("[0-9|.]+")) && count(val.begin(), val.end(), '.') <= 1)
        var->isConst = true;
    else
        var->isConst = false;
    vars.push_back(var);
}

void Calculator::show_value(string &line) {
    string val = line;
    string temp = val;
    string target;
    for (auto i : replace_list) temp = regex_replace(temp, i.first, i.second);
    istringstream is(temp);
    vector<string> token;
    string t("");
    while (is >> t)
        token.push_back(t);
    if (token.size() == 1) {
        if (regex_match(token[0], regex("[0-9]+"))) {
            cout << token[0] << endl;
            return;
        }
        Variable *tem = nullptr;
        for (int j = 0; j < Calculator::vars.size(); j++)
            if (token[0] == Calculator::vars[j]->getVarName()) {
                tem = Calculator::vars[j];
                break;
            }
        if (!tem) {
            cout << "\033[31mERROR: VARIABLE " + token[0] + " DOESN'T EXIST\n\033[0m";
            // cout << "There is no variable called " << token[0] << endl;
            return;
        } else {
            string ans;
            try
            {
                ans = tem->eval();
            }
            catch(string msg)
            {
                cout<<"\033[31m" + msg + "\n\033[0m";
                return;
            }
            
            
            if (ans.empty())
                cout << "\033[31mERROR: WRONG EXPRESSION\n\033[0m";
            else
                cout << ans << endl;
        }
        return;
    }
    token.push_back("");
    for (int i = 0; i < token.size(); i++) {
        if (token[i] == ",") continue;
        if (token[i].empty())
            break;
        if (!regex_match(token[i], DIGIT)) {
            if (token[i + 1] == "(" && regex_match(token[i], VARIABLE)) {
                Func *f = nullptr;
                for (auto func : funcs)
                    if (token[i] == func->name) {
                        f = func;
                        break;
                    }
                if(!f) {
                    cout<<"\033[31mERROR: FUNCTION " + token[i] + " DOESN'T EXIST\n\033[0m";
                    return;
                }
                i++;
                int left = 1, right = 0;
                string args;
                while (left != right) {
                    if (token[i + 1] == "(") left++;
                    else if (token[i + 1] == ")") right++;
                    if(left == right) break;
                    args += token[i + 1];
                    i++;
                }
                try{
                    target += f->eval(args);
                } catch(string msg) {
                    cout<<"\033[31m" + msg + "\n\033[0m";
                    return;
                }
                i++;
            } else {
                if (regex_match(token[i], VARIABLE)) {
                    Variable *var;
                    for (auto v : vars)
                        if (v->getVarName() == token[i]) {
                            var = v;
                            break;
                        }
                    if(!var) {
                        cout<<"\033[31mERROR: VARIABLE " + token[i] + " DOESN'T EXIST\n\033[0m";
                        return;
                    }
                    try{
                        target += var->eval();
                    } catch(string msg) {
                        cout<<"\033[31m" + msg + "\n\033[0m";
                        return;
                    }
                } else
                    target += token[i];
            }
        } else
            target += token[i];
    }
    Calc c;
    c.setExp(const_cast<char *>((target + "=").c_str()));
    try {
        c.Cac();
    } catch(string msg) {
        cout<<"\033[31m" + msg + "\n\033[0m";
        return;
    }
    cout << c.getAns() << endl;
}

void Calculator::assign_value(string &line) {
    string temp = line;
    smatch m;
    regex_match(temp, m, ASSIGNVAL);
    Variable *variable = nullptr;
    for (auto i : vars)
        if (i->getVarName() == m[1]) {
            variable = i;
            break;
        }
    if (!variable) {
        string _varname = m[1];
        cout << "\033[31mERROR: VARIABLE " + _varname + " DOESN'T EXIST\n\033[0m";
        return;
    }
    variable->setVal(m[2]);
    string newVal = m[2];
    if (regex_match(newVal, DIGIT) && count(newVal.begin(), newVal.end(), '.') <= 1)
        variable->isConst = true;
    else
        variable->isConst = false;
}

void Calculator::create_function(string &line) {
    smatch m;
    regex_match(line, m, CREATEFUNC);
    vector<string> splitArgs;
    string arguments = m[2];
    string body = m[3];
    string name = m[1];
    Func *f = nullptr;
    for (auto func : funcs)
        if (func->name == name) {
            f = func;
            break;
        }
    if (f) {
        cout << "\033[31mERROR: FUNCTION " + name + " EXISTED\n\033[0m";
        return;
    }
    f = new Func();
    f->name = name;
    f->body = body;
    for (int i = 0; i < arguments.size(); i++)
        if (arguments[i] == ',')
            arguments[i] = ' ';
    istringstream is(arguments);
    string t;
    while (is >> t)
        splitArgs.push_back(t);
    f->arguments_size = splitArgs.size();
    for (int i = 0; i < splitArgs.size(); i++)
        f->trans.insert(make_pair(splitArgs[i], i));
    funcs.push_back(f);
}

