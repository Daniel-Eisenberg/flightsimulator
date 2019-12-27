//
// Created by Yuval Grinberg on 18/11/2019.
//


#ifndef EX1_EX1_H
#define EX1_EX1_H
using namespace std;

#include <string>
#include "Expression.h"
#include "Interpreter.h"



class Value : public Expression {
    double value = 0;
public:
    Value(double val);
    ~Value(){};
    double calculate();
};

class Variable : public Expression {
    string name = "";
    double value = 0;
public:
    Variable(string name, double value);
    ~Variable(){};
    double calculate();
    string getName();
    Variable& clone();
    Variable& operator+(Variable *v);
    Variable& operator-(Variable *v);
    Variable& operator++();
    Variable& operator--();
    Variable& operator++(int);
    Variable& operator--(int);
    Variable& operator+=(Variable *v);
    Variable& operator-=(Variable *v);
};

class BinaryOperator : public Expression {
protected:
    Expression *left = nullptr, *right = nullptr;
public:
    BinaryOperator(Expression *left, Expression *right);
    ~BinaryOperator();
};

class Plus : public BinaryOperator {
public:
    Plus(Expression *left, Expression *right) : BinaryOperator(left, right){};
    ~Plus(){};
    double calculate();
};

class Minus : public BinaryOperator {
public:
    Minus(Expression *left, Expression *right) : BinaryOperator(left, right){};
    ~Minus(){};
    double calculate();
};

class Mul : public BinaryOperator {
public:
    Mul(Expression *left, Expression *right) : BinaryOperator(left, right){};
    ~Mul(){};
    double calculate() ;
};

class Div : public BinaryOperator {
public:
    Div(Expression *left, Expression *right) : BinaryOperator(left, right){};
    ~Div(){};
    double calculate() ;
};

class UnaryOperator : public Expression {
protected:
    Expression *exp = nullptr;
public:
    UnaryOperator(Expression *exp);
    ~UnaryOperator();
    double calculate() ;

};

class UPlus : public UnaryOperator {
public:
    UPlus(Expression *exp) : UnaryOperator(exp){};
    ~UPlus(){};
    double calculate() ;
};

class UMinus : public UnaryOperator {
public:
    UMinus(Expression *exp) : UnaryOperator(exp){};
    ~UMinus(){};
    double calculate() ;
};

#endif //EX1_EX1_H
