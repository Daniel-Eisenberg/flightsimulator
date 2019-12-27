//
// Created by Yuval Grinberg on 18/11/2019.
//

#ifndef EX1_INTERPRETER_H
#define EX1_INTERPRETER_H
#include <map>
#include <queue>
#include "Expression.h"
#include "Token.h"


class Interpreter {
    std::map<string, string> variables_map;
public:
    Interpreter();
    virtual ~Interpreter();
    virtual Expression* interpret(string arg);
    Expression* reversePolishNotation(queue<Token> shunting_yard);
    queue<Token> shuntingYard(string arg);
    void setVariables(string arg);
    double getVariable(string arg);
    vector<string> split(const string& s, char delimiter);
};
#endif //EX1_INTERPRETER_H
