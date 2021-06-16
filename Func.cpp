//
// Created by csh on 2021/6/4.
//

/*
 * Func f(x) = x + 2
 * Func g(x,y) = f(y) - x
 * g(3,5)
 *
 * Func f(x,y) = x + y
 * Func g(x,y,z) = f(x,y) - z
 * g(1,2,4)
 *
 * Func h(x,y,z,w) = g(y,z,w) * x
 * h(4,2,3,4)
 *
 * Func f(x, y) = x + y
 * Func g(x) = x + 1
 *
 * */

#include "Func.h"
#include "Calculator.h"
vector<pair<regex, string>> replace_list{
        make_pair(regex("\\("), " ( "),
        make_pair(regex("\\)"), " ) "),
        make_pair(regex("\\+"), " + "),
//        make_pair(regex("&"), " & "),
//        make_pair(regex("\\|"), " | "),
//        make_pair(regex("\\^"), " ^ "),
        make_pair(regex("-"), " - "),
        make_pair(regex("\\*"), " * "),
//        make_pair(regex("%"), " % "),
        make_pair(regex("/"), " / "),
        make_pair(regex(","), " , "),
//        make_pair(regex("="), " = "),
};
//string expr_with_end(queue<string> &tokens,string &end,int rank);
//string get_atom(queue<string> &tokens,bool begin);


//string expr_with_end(queue<string> &tokens,string &end,int rank){
//    string now=get_atom(tokens,rank==0);
//    while(1){
//        string str=tokens.front();
//        if(str==","||str==")"||str==""){
//            end=str;
//            break;
//        }
//        else if(Calculator::operatorRank.find(str)!=Calculator::operatorRank.end()){
//            int new_rank=Calculator::operatorRank.find(str)->second;
//            if(new_rank<=rank)break;
//            tokens.pop();
//            now=new ECall(new EVar(str),{now,expr_with_end(tokens,end,new_rank)});
//        }
//    }
//    return now;
//}
//
//string get_atom(queue<string> &tokens,bool begin) {
//    string now=tokens.front();
//    tokens.pop();
//    if(regex_match(now,tint))return new EData(stoi(now));
//    else if(regex_match(now,tfloat))return new EData(stof(now));
//    else if(regex_match(now,var)){
//        EVar *func=new EVar(now);
//        vector<EObject*> args;
//        string nexts=tokens.front();
//        if(nexts=="("){
//            tokens.pop();
//            string end;
//            while(1){
//                args.push_back(expr_with_end(tokens,end,0));
//                tokens.pop();
//                if(end==",")continue;
//                else if(end==")")break;
//                else if(end=="")throw unexpected_token;
//            }
//        }
//        return new ECall(func,args);
//    }else if(now=="("){
//        string end;
//        EObject *res=expr_with_end(tokens,end,0);
//        tokens.pop();
//        if(end==")")return res;
//        else throw unexpected_token;
//    }else if(begin&&now=="+"){
//        return get_atom(tokens, false);
//    }else if(begin&&now=="-"){
//        EObject *res=get_atom(tokens, false);
//        return new ECall(new EVar("m-"),{res});
//    }else if(now=="!"||now=="~"){
//        EObject *res=get_atom(tokens, false);
//        return new ECall(new EVar(now),{res});
//    }else throw unexpected_token;
//}

const regex DIGIT("[0-9|.]+");
const regex VARIABLE("[a-zA-Z]+");

string Func::eval(string args) {
    Calc c;
    for(auto i : replace_list) args = regex_replace(args, i.first, i.second);
    istringstream iss(args);
    string t;
    vector<string> argus;
    while(iss>>t)
        argus.push_back(t);
    argus.push_back("");
    int index = 0;
    string arg;
    for(int i = 0; i < argus.size(); i++) {
        if(argus[i] == ",") {
            c.setExp(const_cast<char *>((arg + "=").c_str()));
            try{
                c.Cac(); 
            } catch(string msg) {
                throw msg;
            }
            arguments[index++] = to_string(c.getAns());
            arg = "";
            continue;
        }
        if(index == arguments_size)
            break;
        if(argus[i].empty()) {
            c.setExp(const_cast<char *>((arg + "=").c_str()));
            try{
                c.Cac(); 
            } catch(string msg) {
                throw msg;
            }
            arguments[index++] = to_string(c.getAns());
            arg = "";
            break;
        }
        if(regex_match(argus[i], VARIABLE)) {
            if(argus[i + 1] == "(") {
                Func *f = nullptr;
                for(auto func : Calculator::funcs)
                    if(argus[i] == func->name) {
                        f = func;
                        break;
                    }
                if(!f)
                    throw string("ERROR: FUNCTION " + argus[i] + " DOESN'T EXIST");
                i++;
                int left = 1, right = 0;
                string _args;
                while(left != right) {
                    if(argus[i + 1] == "(") left++;
                    else if(argus[i + 1] == ")") right++;
                    if(left == right)
                        break;
                    _args += argus[i + 1];
                    i++;
                }
                try {
                    arg += f->eval(_args);
                } catch(string msg) {
                    throw msg;
                }
                i++;
            }
            else {
                if(regex_match(argus[i], VARIABLE)) {
                    Variable * var;
                    for(auto v : Calculator::vars)
                        if(v->getVarName() == argus[i]) {
                            var = v;
                            break;
                        }
                    if(!var)
                        throw string("ERROR: VARIABLE " + argus[i] + " DOESN'T EXIST");
                    try {
                        arg += var->eval();
                    } catch(string msg) {
                        throw msg;
                    }
                }
                else
                    arg += argus[i];
            }
        } else
            arg += argus[i];
    }
    string temp = body;
    string target;
//    for(const auto& i : replace_list) temp = regex_replace(temp, i.first, i.second);
    for(int i = 0; i< replace_list.size(); i++)
        temp = regex_replace(temp, replace_list[i].first, replace_list[i].second);
    istringstream is(temp);
    vector<string> tokens;
    while (is >> t)
        tokens.push_back(t);
    tokens.push_back("");
    for(int i = 0; i < tokens.size(); i++) {
        if (tokens[i].empty())
            break;
        else if(tokens[i] == "+" || tokens[i] == "-" || tokens[i] == "*" || tokens[i] == "/" || (regex_match(tokens[i], regex("[0-9|.]+")) && count(tokens[i].begin(), tokens[i].end(), '.') <= 1) || tokens[i] == "(" || tokens[i] == ")")
            target += tokens[i];
        else if(regex_match(tokens[i], regex("[a-zA-Z]+")) && tokens[i + 1] == "(") {
            Func *f = nullptr;
            for(auto func : Calculator::funcs)
                if(func->name == tokens[i]) {
                    f = func;
                    break;
                }
            if(!f)
                throw string("ERROR: FUNCTION " + tokens[i] + " DOESN'T EXIST");
            i++;
            string _args;
            int left = 1, right = 0;
            while(left != right) {
                if(tokens[i + 1] == "(") left++;
                else if(tokens[i + 1] == ")") right++;
                if(left == right) break;
                if(trans.find(tokens[i + 1]) != trans.end()) {
                    _args += arguments[trans[tokens[i + 1]]];
                    i++;
                    continue;
                }
                _args += tokens[i + 1];
                i++;
            }
            i++;
            try{
                target += f->eval(_args);
            } catch(string msg) {
                throw msg;
            }
        }
        else if(regex_match(tokens[i], regex("[a-zA-Z]+")) && tokens[i + 1] != "(") {
            if(trans.find(tokens[i]) == trans.end()) {
                Variable *variable = nullptr;
                for(auto var : Calculator::vars)
                    if(var->getVarName() == tokens[i]) {
                        variable = var;
                        break;
                    }
                if(!variable)
                    throw string("ERROR: VARIABLE " + tokens[i] + " DOESN'T EXIST");
                try{
                    target += variable->eval();
                } catch(string msg) {
                    throw msg;
                }
            }
            else
                target += arguments[trans[tokens[i]]];
        }

    }

    for (int i = 0; i < arguments_size; i++)
        arguments[i] = "";
    c.setExp(const_cast<char *>((target + "=").c_str()));
    try {
        c.Cac();
    } catch(string msg) {
        throw msg;
    }
    return to_string(c.getAns());
}

