//
// Created by csh on 2021/6/4.
//

#ifndef EXPCALC_CALC_H
#define EXPCALC_CALC_H


#include<cstdio>
#include<cstring>
#include<iostream>
using namespace std;

const int MAXN = 200;

class Calc{
private:
    char Exp[MAXN];         //表达式
public:
    const char *getExp() const;

    double getAns() const;

    void setAns(double ans);

    void setExp(char* exp);

private:
    int NextB[MAXN];        //匹配括号位置
    double Ans;             //求值结果
    void DelandLower(char *str);        //删除空字符 转化为小写
    bool Check(char *str,int & len);
    bool CheckCh(const char *str,int pos);  //检查字符
    bool Is_Num(char c);        //是否为数字
    bool Operat(char c);        //是否为运算符
    bool CheckError(const char *str,int len);
    bool CrectB(const char *str);       //检查括号匹配
    bool Equal(double a,double b);      //判断浮点数相等
    int Prio(double x);             //符号优先级判断
    double hash(char c);            //符号到浮点型映射
    double GetV(const char *str,int st,int ed);     //区间求值
public:
    void Input(){cin>>Exp;}
    void Output(){printf("%.2f\n",Ans);}
    bool Cac();
};


#endif //EXPCALC_CALC_H
