//
// Created by Yuval Grinberg on 18/11/2019.
//

#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include "Interpreter.h"
#include "MathParser.h"
#include "Token.h"
#include "DatabaseManager.h"
#include <sstream>

/**
 * Get variable if it exists in the cache map.
 * @param arg
 * @return
 */
double Interpreter::getVariable(string arg){
    if (variables_map.count(arg) < 1)
        throw "Variable was not defined with a value";

    if (isdigit(variables_map.find(arg)->second[0]))
        return stod(variables_map.find(arg)->second);
    else
        throw "Variable has invalid value";
};


/**
 * Set the variables if they have digit values.
 * @param arg
 */
void Interpreter::setVariables(string arg){
    vector<string> tokens = split(arg, ';');

    for (string token : tokens) {
        vector<string> variable = split(token, '=');

        if (!isdigit(variable[1][0]))
            throw "Variable has invalid value";

        variables_map.erase(variable[0]);
        variables_map[variable[0]] = variable[1];
    }
};

/**
 * Split a string according to a chosen delimiter.
 * @param s
 * @param delimiter
 * @return
 */
vector<string> Interpreter::split(const string& s, char delimiter)
{
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

Expression* Interpreter::interpret(string arg) {
    return reversePolishNotation(shuntingYard(arg));
}

/**
 * Return the shunting yard result queue;
 * @param arg the math expression to parse
 * @return
 */
queue<Token> Interpreter::shuntingYard(string arg) {
    stack<Token> operators;
    queue<Token> shunting_yard;
    Token *token;

    bool expecting_op = false;
    for (string::size_type i = 0; i < arg.length(); i++){
        string val = "";
        if (isdigit(arg[i])) {
            if (expecting_op)
                throw "Expecting (, ), -, +, *, /";

            val = arg[i];
            while (i < arg.length() && (isdigit(arg[i + 1]) || isalpha(arg[i + 1]) || arg[i + 1] == '.')) {
                if (arg[i + 1] == '.' && !isdigit(arg[i + 2]))
                    throw "Bad double entered";
                if (isalpha(arg[i + 1]))
                    throw "Number before variable";
                val += arg[++i];
            }
            token = new Token(val);
            shunting_yard.push(*token);
            expecting_op = true;
        } else if (isalpha(arg[i])) {
//            if (expecting_op)
//                throw "Excpecting (, ), -, +, *, /";
            val = arg[i];
            while (i < arg.length() && (isdigit(arg[i + 1]) || isalpha(arg[i + 1]) || arg[i + 1] == '.')) {
                val += arg[++i];
            }
            token = new Token(val);
            shunting_yard.push(*token);
            expecting_op = true;
        } else if (arg[i] == '-' || arg[i] == '+' || arg[i] == '*' || arg[i] == '/') {
            if ((i < arg.length()) && (arg[i+1] == '-' || arg[i+1] == '+' || arg[i+1] == '*' || arg[i+1] == '/'))
                throw "Bad character after operator";

            val = arg[i];
            token = new Token(val);
            while (!operators.empty() && token->isPrecedent(operators.top()) && operators.top().getValue() != "(") {
                shunting_yard.push(operators.top());
                operators.pop();
            }

            if (val == "-" && (isdigit(arg[i + 1]) || arg[i + 1] == '(')) {
                token = new Token('#' + val);
            }
            operators.push(*token);

            expecting_op = false;
        } else if (arg[i] == '(') {
            token = new Token("(");
            operators.push(*token);
            expecting_op = false;
        } else if (arg[i] == ')') {
            while (operators.top().getValue() != "(") {
                shunting_yard.push(operators.top());
                operators.pop();
            }

            if (operators.top().getValue() == "(") {
                operators.pop();
            } else if (operators.empty()) {
                throw "Expecting parenthesis";
            }

            expecting_op = false;
        } else {
            throw "Wrong character in math expression";
        }
    }

    while (!operators.empty()) {
        shunting_yard.push(operators.top());
        operators.pop();
    }

    if (token != nullptr)
        delete token;

    return shunting_yard;
}

/**
 * Use reverse polish Notation algorithm to parse the data into an Expression object.
 * @param shunting_yard
 * @return
 */
Expression* Interpreter::reversePolishNotation(queue<Token> shunting_yard) {
    Expression* e1 = nullptr; Expression* e2 = nullptr; Expression* result = nullptr;
    stack<Expression*> expressions;

    while (!shunting_yard.empty()) {
        if (shunting_yard.front().isNumber()) {
            expressions.push(new Value(stod(shunting_yard.front().getValue())));
            shunting_yard.pop();
        } else if (shunting_yard.front().isVariable()) {
            string varName = shunting_yard.front().getValue();
            double varValue = 0;
            if (isalpha(varName[0]) && DatabaseManager::get().isVariableExist(varName)) {
                varValue = DatabaseManager::get().getFromVariablesMap(varName, 0)->getValue();
            } else {
                varValue = getVariable(varName);
            }
            expressions.push(new Var(varName, varValue));
            shunting_yard.pop();
        } else {
            e1 = expressions.top();
            expressions.pop();

            // Pop e2 only for binary operators
            if (!expressions.empty() && shunting_yard.front().getValue()[0] != '#') {
                e2 = expressions.top();
                expressions.pop();
            } else {
                e2 = nullptr;
            }

            // Choose right expression from operator
            char op = shunting_yard.front().getValue()[0];
            if (op == '+') {
                if (e2 == nullptr)
                    expressions.push(new UPlus(e1));
                else
                    expressions.push(new Plus(e1, e2));
            } else if (op == '-') {
                if (e2 == nullptr)
                    expressions.push(new UMinus(e1));
                else
                    expressions.push(new Minus(e2, e1));
            } else if (op == '/') {
                expressions.push(new Div(e2, e1));
            } else if (op == '*') {
                expressions.push(new Mul(e1, e2));
            } else if (op == '#') {
                // For the case of unary operators
                if (shunting_yard.front().getValue() == "#-")
                    expressions.push(new UMinus(e1));
                else
                    expressions.push(new UPlus(e1));
            }

            shunting_yard.pop();
        }
    }

    return expressions.top();
}

Interpreter::~Interpreter(){
}

Interpreter::Interpreter(){
}
