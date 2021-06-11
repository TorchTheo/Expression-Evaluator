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

const regex CREATEVAR("Var\\s+[a-zA-Z]+\\s*=\\s*(.*?)");
const regex ASSIGNVAL("([a-zA-Z]+)\\s*=\\s*(.*?)");
const regex CREATEFUNC("Func\\s+(.*?)\\((.*?)\\)\\s*=\\s*(.*?)");
const regex DIGIT("[0-9|.]+");

bool Calculator::check(string line) {
    if(count(line.begin(), line.end(), '(') != count(line.begin(), line.end(), ')')) {
        cout<<"括号数量不匹配\n";
        return false;
    }



    return true;
}

void Calculator::doSth() {
    cout<<"ExpCal > ";
    string line;
    getline(cin, line);
    if(!check(line))
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
            cout<<"未知表达式\n";
            break;
    }
}



int Calculator::analyseLine(string &line) {
    if(regex_match(line, CREATEVAR))
        return ACTION::CREATE_VARIABLE;
    else if(regex_match(line, ASSIGNVAL))
        return ACTION::ASSIGN_VALUE;
    else if(regex_match(line, CREATEFUNC))
        return ACTION::CREATE_FUNCTION;
    else
        return ACTION::SHOW_VALUE;
}

void Calculator::create_variable(string &s) {
    string line = s;
    line.erase(0, 3);
    int index = 0;
    if(!line.empty())
        while((index = line.find(' ', index)) != string::npos)
            line.erase(index, 1);
    for(int i = 0; i < line.length(); i++)
        if(line[i] == '=')
            line[i] = ' ';
    istringstream is(line);
    string Var, name, val;
    is >> name >> val;
    for(auto i : vars)
        if(i->getVarName() == name) {
            cout<<"Existed"<<endl;
            return;
        }

    Variable* var = new Variable(name, val);
    if(regex_match(val, regex("[0-9|.]+")) && count(val.begin(), val.end(), '.') <= 1)
        var->isConst = true;
    else
        var->isConst = false;
    vars.push_back(var);
}

void Calculator::show_value(string &line) {
    string val = line;
    string temp = val;
    string target = val;
    for(int i = 0; i < temp.size(); i++)
        if(temp[i] == '+' || temp[i] == '-' || temp[i] == '*' || temp[i] == '/' || temp[i] == '(' || temp[i] == ')' || temp[i] == ',')
            temp[i] = ' ';
    istringstream is(temp);
    vector<string> token;
    string t("");
    while(is >> t)
        token.push_back(t);
    if(token.size() == 1) {
        if(regex_match(token[0], regex("[0-9]+"))) {
            cout << token[0] << endl;
            return;
        }
        Variable *tem = nullptr;
        for(int j = 0; j < Calculator::vars.size(); j++)
            if(token[0] == Calculator::vars[j]->getVarName()) {
                tem = Calculator::vars[j];
                break;
            }
        if(!tem) {
            cout<<"There is no variable called " << token[0] << endl;
            return;
        }
        else {
            string ans = tem->eval();
            if(ans.empty())
                cout<<"表达式有误\n";
            else
                cout<<ans<<endl;
        }
        return;
    }
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
                if(varVal.empty()) {
                    cout<<"表达式有误\n";
                    return;
                }
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
                }
                i += f->arguments_size;
                string func_val = f->eval();
                if(func_val.empty()) {
                    cout<<"表达式有误\n";
                    return;
                }
                sub.pop_back();
                sub += ")";
                int pos = target.find(sub);
                target.replace(pos, sub.length(), func_val);
                continue;
            }
        }
    }
    Calc c;
    c.setExp(const_cast<char *>((target + "=").c_str()));
    if(!c.Cac()) {
        cout<<"表达式错误\n";
        return;
    }
    cout<<c.getAns()<<endl;
}

void Calculator::assign_value(string &line) {
    string temp = line;
    smatch m;
    regex_match(temp, m, ASSIGNVAL);
    Variable *variable = nullptr;
    for(auto i : vars)
        if(i->getVarName() == m[1]) {
            variable = i;
            break;
        }
    if(!variable) {
        cout<<"NO SUCH VARIABLE"<<endl;
        return;
    }
    variable->setVal(m[2]);
    string newVal = m[2];
    if(regex_match(newVal, DIGIT) && count(newVal.begin(), newVal.end(), '.') <= 1)
        variable->isConst = true;
    else
        variable->isConst = false;
}

//Calculator::Calculator() {
//    operatorRank.insert(make_pair("?",1));
//    operatorRank.insert(make_pair(":",2));
//    operatorRank.insert(make_pair("||",3));
//    operatorRank.insert(make_pair("&&",4));
//    operatorRank.insert(make_pair("|",5));
//    operatorRank.insert(make_pair("^",6));
//    operatorRank.insert(make_pair("&",7));
//    operatorRank.insert(make_pair("==",8));
//    operatorRank.insert(make_pair("!=",8));
//    operatorRank.insert(make_pair("<",9));
//    operatorRank.insert(make_pair("<=",9));
//    operatorRank.insert(make_pair(">",9));
//    operatorRank.insert(make_pair(">=",9));
//    operatorRank.insert(make_pair("<<",10));
//    operatorRank.insert(make_pair(">>",10));
//    operatorRank.insert(make_pair("+",11));
//    operatorRank.insert(make_pair("-",11));
//    operatorRank.insert(make_pair("*",12));
//    operatorRank.insert(make_pair("/",12));
//    operatorRank.insert(make_pair("%",12));
//    operatorRank.insert(make_pair("**",13));
//    operatorRank.insert(make_pair("!",14));
//    operatorRank.insert(make_pair("~",14));
//}

void Calculator::create_function(string &line) {
    smatch m;
    regex_match(line, m, CREATEFUNC);
    vector<string>splitArgs;
    string arguments = m[2];
    string body = m[3];
    string name = m[1];
    Func *f = nullptr;
    for(auto func : funcs)
        if(func->name == name) {
            f = func;
            break;
        }
    if(f) {
        cout<<"EXIST\n";
        return;
    }
    f = new Func();
    f->name = name;
    f->body = body;
    for(int i = 0; i < arguments.size(); i++)
        if(arguments[i] == ',')
            arguments[i] = ' ';
    istringstream is(arguments);
    string t;
    while(is >> t)
        splitArgs.push_back(t);
    f->arguments_size = splitArgs.size();
    for(int i = 0; i < splitArgs.size(); i++)
        f->trans.insert(make_pair(splitArgs[i], i));
    funcs.push_back(f);
}

