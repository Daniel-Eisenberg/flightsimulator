//
// Created by Yuval Grinberg on 18/11/2019.
//


#include <string>
#include "Expression.h"
#include "ex1.h"

Value::Value(double val){
    value = val;
}

double Value::calculate() {
    return this->value;
}

Variable::Variable(string name, double value) {
    this->value = value;
    this->name = name;
}

double Variable::calculate() {
    return this->value;
}

string Variable::getName() {
    return this->name;
}

Variable& Variable::clone() {
    Variable *variableCpy = new Variable(this->name, this->value);
    return *variableCpy;
}

Variable& Variable::operator+(Variable *v) {
    this->value = this->value + v->value;
    return *this;
}

Variable& Variable::operator-(Variable *v) {
    this->value = this->value - v->value;
    return *this;
}

Variable& Variable::operator++() {
    ++this->value;
    return *this;
}

Variable& Variable::operator--() {
    --this->value;
    return *this;
}

Variable& Variable::operator++(int) {
    this->value++;
    return *this;
}

Variable& Variable::operator--(int) {
    this->value--;
    return *this;
}

Variable& Variable::operator+=(Variable *v) {
    this->value += v->value;
    return *this;
}

Variable& Variable::operator-=(Variable *v) {
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