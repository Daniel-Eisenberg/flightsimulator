//
// Created by Yuval Grinberg on 18/11/2019.
//

#include <map>
#include <stack>
#include <string>
#include "Token.h"
#include "DatabaseManager.h"

using namespace std;

Token::Token(string arg) {
    this->value = arg;
    if (isdigit(arg[0])) {
        isNum = true;
    } else if (isalpha(arg[0])) {
        isVar = true;
    } else if (arg[0] == '*' || arg[0] == '/' || arg[0] == '+' || arg[0]== '-' || arg[0]== '#') {
        isOper = true;
    }
}

string Token::getValue() {
    return this->value;
}

bool Token::isOperator() {
    return this->isOper;
}

bool Token::isNumber() {
    return this->isNum;
}

bool Token::isVariable() {
    return this->isVar;
}

bool Token::isPrecedent(Token t) {
    if (this->value == "*" && t.value == "/") {
        return true;
    } else if (t.value == "*" && this->value == "/") {
        return true;
    } else if (this->value == "+") {
        return true;
    } else if (this->value == "-") {
        return true;
    }
    return false;
}