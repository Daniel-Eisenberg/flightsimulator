//
// Created by Yuval Grinberg on 18/11/2019.
//


#include <string>
#include "Expression.h"
#include "MathParser.h"

Value::Value(double val){
    value = val;
}

double Value::calculate() {
    return this->value;
}

Var::Var(string name, double value) {
    this->value = value;
    this->name = name;
}

double Var::calculate() {
    return this->value;
}

string Var::getName() {
    return this->name;
}

Var& Var::clone() {
    Var *variableCpy = new Var(this->name, this->value);
    return *variableCpy;
}

Var& Var::operator+(Var *v) {
    this->value = this->value + v->value;
    return *this;
}

Var& Var::operator-(Var *v) {
    this->value = this->value - v->value;
    return *this;
}

Var& Var::operator++() {
    ++this->value;
    return *this;
}

Var& Var::operator--() {
    --this->value;
    return *this;
}

Var& Var::operator++(int) {
    this->value++;
    return *this;
}

Var& Var::operator--(int) {
    this->value--;
    return *this;
}

Var& Var::operator+=(Var *v) {
    this->value += v->value;
    return *this;
}

Var& Var::operator-=(Var *v) {
    this->value -= v->value;
    return *this;
}

BinaryOperator::BinaryOperator(Expression *left, Expression *right) {
    this->right = right;
    this->left = left;
}

BinaryOperator::~BinaryOperator() {
    delete this->right;
    delete this->left;
}

double Plus::calculate() {
    return this->left->calculate() + this->right->calculate();
}

double Minus::calculate() {
    return this->left->calculate() - this->right->calculate();
}

double Mul::calculate() {
    return this->left->calculate() * this->right->calculate();
}

double Div::calculate() {
    return this->left->calculate() / this->right->calculate();
}

UnaryOperator::UnaryOperator(Expression *exp) {
    this->exp = exp;
}

UnaryOperator::~UnaryOperator() {
    delete this->exp;
}


double UnaryOperator::calculate() {
    return this->exp->calculate();
}

double UPlus::calculate() {
    return this->exp->calculate();
}

double UMinus::calculate() {
    return -this->exp->calculate();
}