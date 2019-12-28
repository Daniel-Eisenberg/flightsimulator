//
// Created by Yuval Grinberg on 23/12/2019.
//

#include "Command.h"
#include "Tcp_Server.h"
#include <vector>
#include "ex1.h"
#include "ex3.h"
#include "Expression.h"
#include <iostream>
#include <thread>

using namespace std;
// Local static class methods
// ---------------------------

static std::map<std::string, Variable*> variables_map;
std::mutex mServer;

int parseMathExp(std::vector<std::string> *list, int i, int scope) {
    string mathExp;
    double value;
    if (list->at(i) == "$") // if by mistake we got the first $
        i++;
    while (list->at(i) != "$") {
        if (isalpha(list->at(i)[0]))
            mathExp += to_string(variables_map[list->at(i)]->getValue(scope));
        else
            mathExp += list->at(i);
        i++;
    }
    Interpreter* inter = new Interpreter();
    Expression *exp = inter->interpret(mathExp);
    value = exp->calculate();

    return value;
}

// Finds the next occurrence of a string in the array
int Command::findStopSign(std::vector<std::string> *list, int i, const string& sign) {
    int args = 1;
    while (list->at(i) != sign) {
        i++;
        args++;
    }
    args++;

    return args;
}

// Return boolean evaluation of a logical expression
bool static evaluateLogicalExp(std::vector<std::string> *list, int i, int scope) {
    int leftStart = i;
    int leftEnd = i + Command::findStopSign(list, leftStart + 1, "$");
    string defType = list->at(leftEnd);
    int rightStart = leftEnd + 1;

    int leftExp = parseMathExp(list, leftStart, scope);
    int rightExp = parseMathExp(list, rightStart, scope);
    if (defType == "==")
            return leftExp == rightExp;
    else if (defType == "!=")
            return leftExp != rightExp;
    else if (defType == "<=")
            return leftExp <= rightExp;
    else if (defType == ">=")
            return leftExp >= rightExp;
    else if (defType == ">")
            return leftExp > rightExp;
    else if (defType == "<")
            return leftExp < rightExp;

    return false;
}



// Subclasses methods
// ---------------------------

int Command::execute(std::vector<std::string> *list, int i, int scope) {
}

// Clear the variables scope that we leave (finishing a method etc.)
void Command::clearVariablesScope(int scope) {
    for (auto&& [key, value] : variables_map) {
        if (value->getScope() >= scope)
            variables_map.erase(key);
    }
}

int OpenServerCommand::execute(std::vector<std::string> *list, int i, int scope) {
    Tcp_Server server;
    string port = list->at(i + 1);
    std::thread serverThread(server.create_socket(stoi(port)));
    return args;
}

int ConnectCommand::execute(std::vector<std::string> *list, int i, int scope)  {
    std::thread connectionThread();
    return args;
}

int DefineVarCommand::execute(std::vector<std::string> *list, int i, int scope)  {
    Variable *var = nullptr;
    string varName = list->at(i + 1);
    string defType = list->at(i + 2);
    if (defType == "<-") {// Define a variable that gets data from simulator
        string sim = list->at(i + 3);
        var = new Variable(sim, false, scope);
        variables_map[varName] = var;
        args = 4;
    } else if (defType == "->") {// Define a variable that sets data to the simulator
        string sim = list->at(i + 3);
        var = new Variable(sim, true, scope);
        variables_map[varName] = var;
        args = 4;
    } else if (defType == "=") {// Define a variable that sets data to the simulator
        if (list->at(i + 3) == "$") { // Calc and set a math expression
            double value = parseMathExp(list, i + 4, scope);
            var = new Variable("", false, scope);
            var->setValue(value);
            variables_map[varName] = var;
            args = 2 + findStopSign(list, i + 4, "$");
        } else
            cout << "ERROR no math exp $ after = tag.";
    }

    return args;
}


int SetVarCommand::execute(std::vector<std::string> *list, int i, int scope)  {
    string varName = list->at(i + 1);

    if (list->at(i + 3) == "$") { // Calc and set a math expression
        int value = parseMathExp(list, i + 4, scope);
        variables_map[varName]->setValue(value);
        args = 2 + findStopSign(list, i + 4, "$");
    } else
        cout << "ERROR no math exp $ after = tag.";

    return args;
}

int WhileLoopCommand::execute(std::vector<std::string> *list, int i, int scope)  {
    args = 1 + findStopSign(list, i + 1, "}");
    int cmd = findStopSign(list, i + 1, "{");
    int logicalExpIndex = i + 1;
    while(evaluateLogicalExp(list, logicalExpIndex, scope) && list->at(i) != "}") {
        ex3::parser(list, i + cmd, true, scope + 1);
    }
    return args;
}

int IfCommand::execute(std::vector<std::string> *list, int i, int scope)  {
    args = 1 + findStopSign(list, i + 1, "}");
    int cmd = findStopSign(list, i + 1, "{");
    int logicalExpIndex = i + 1;
    if(evaluateLogicalExp(list, logicalExpIndex, scope)) {
        ex3::parser(list, i + cmd, true, scope + 1);
    }
    return args;
}

int FunctionCommand::execute(std::vector<std::string> *list, int i, int scope)  {
    args = 1 + findStopSign(list, i + 1, "}");
    return args;
}

int PrintCommand::execute(std::vector<std::string> *list, int i, int scope)  {
    if (variables_map.count(list->at(i + 1)) > 0)
        cout << variables_map[list->at(i + 1)]->getValue(scope) << endl;
    else
        cout << list->at(i + 1) << endl;
    return args;
}

int SleepCommand::execute(std::vector<std::string> *list, int i, int scope)  {
    sleep(stod(list->at(i + 1)));
    return args;
}