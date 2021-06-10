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
        make_pair(regex(","), " "),
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

string Func::eval() {
    string temp = body;
//    for(int i = 0; i < temp.size(); i++)
//        if(temp[i] == ',')
//            temp[i] = ' ';
    string target;
//    for(const auto& i : replace_list) temp = regex_replace(temp, i.first, i.second);
    for(int i = 0; i< replace_list.size(); i++)
        temp = regex_replace(temp, replace_list[i].first, replace_list[i].second);
    istringstream is(temp);
    vector<string> tokens;
    string t;
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
            for(int j = i + 2; j < i + 2 + f->arguments_size; j++) {
                if(regex_match(tokens[j], regex("[0-9|.]+")) && count(tokens[i].begin(), tokens[i].end(), '.'))
                    f->arguments[j - i - 2] = tokens[j];
                else {
                    if(trans.find(tokens[j]) == trans.end()) {
                        Variable *variable = nullptr;
                        for(auto var : Calculator::vars)
                            if(var->getVarName() == tokens[j]) {
                                variable = var;
                                break;
                            }
                        f->arguments[j - i - 2] = variable->eval();
                    }
                    else
                        f->arguments[j - i - 2] = arguments[trans[tokens[j]]];
                }
            }

            i += f->arguments_size + 2;
            target += f->eval();
        }
        else if(regex_match(tokens[i], regex("[a-zA-Z]+")) && tokens[i + 1] != "(") {
            if(trans.find(tokens[i]) == trans.end()) {
                Variable *variable = nullptr;
                for(auto var : Calculator::vars)
                    if(var->getVarName() == tokens[i]) {
                        variable = var;
                        break;
                    }
                target += variable->eval();
            }
            else
                target += arguments[trans[tokens[i]]];
        }

    }

    for (int i = 0; i < arguments_size; i++)
        arguments[i] = "";
    Calc c;
    c.setExp(const_cast<char *>((target + "=").c_str()));
    c.Cac();
    return to_string(c.getAns());
}

