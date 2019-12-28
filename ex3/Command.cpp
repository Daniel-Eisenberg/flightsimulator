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

using namespace std;
// Local static class methods
// ---------------------------

static std::map<std::string, Variable*> variables_map;

int parseMathExp(std::vector<std::string> *list, int i) {
    string mathExp;
    double value;
    if (list->at(i) == "$") // if by mistake we got the first $
        i++;
    while (list->at(i) != "$") {
        if (isalpha(list->at(i)[0]))
            mathExp += to_string(variables_map[list->at(i)]->getValue());
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
bool static evaluateLogicalExp(std::vector<std::string> *list, int i) {
    int leftStart = i;
    int leftEnd = i + Command::findStopSign(list, leftStart + 1, "$");
    string defType = list->at(leftEnd);
    int rightStart = leftEnd + 1;

    int leftExp = parseMathExp(list, leftStart);
    int rightExp = parseMathExp(list, rightStart);
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

int Command::execute(std::vector<std::string> *list, int i) {
}

int OpenServerCommand::execute(std::vector<std::string> *list, int i) {
    Tcp_Server server;
//    server.create_socket();
    return args;
}

int ConnectCommand::execute(std::vector<std::string> *list, int i)  {
    //connect to the server;
    return args;
}

int DefineVarCommand::execute(std::vector<std::string> *list, int i)  {
    Variable *var = nullptr;
    string varName = list->at(i + 1);
    string defType = list->at(i + 2);
    if (defType == "<-") {// Define a variable that gets data from simulator
        string sim = list->at(i + 3);
        var = new Variable(sim, false);
        variables_map[varName] = var;
        args = 4;
    } else if (defType == "->") {// Define a variable that sets data to the simulator
        string sim = list->at(i + 3);
        var = new Variable(sim, true);
        variables_map[varName] = var;
        args = 4;
    } else if (defType == "=") {// Define a variable that sets data to the simulator
        if (list->at(i + 3) == "$") { // Calc and set a math expression
            double value = parseMathExp(list, i + 4);
            var = new Variable("", false);
            var->setValue(value);
            variables_map[varName] = var;
            args = 2 + findStopSign(list, i + 4, "$");
        } else
            cout << "ERROR no math exp $ after = tag.";
    }

    return args;
}


int SetVarCommand::execute(std::vector<std::string> *list, int i)  {
    string varName = list->at(i + 1);

    if (list->at(i + 3) == "$") { // Calc and set a math expression
        int value = parseMathExp(list, i + 4);
        variables_map[varName]->setValue(value);
        args = 2 + findStopSign(list, i + 4, "$");
    } else
        cout << "ERROR no math exp $ after = tag.";

    return args;
}

int WhileLoopCommand::execute(std::vector<std::string> *list, int i)  {
    args = 1 + findStopSign(list, i + 1, "}");
    int cmd = findStopSign(list, i + 1, "{");
    int logicalExpIndex = i + 1;
    while(evaluateLogicalExp(list, logicalExpIndex) && list->at(i) != "}") {
        ex3::parser(list, i + cmd, true);
    }
    return args;
}

int IfCommand::execute(std::vector<std::string> *list, int i)  {
    args = 1 + findStopSign(list, i + 1, "}");
    int cmd = findStopSign(list, i + 1, "{");
    int logicalExpIndex = i + 1;
    if(evaluateLogicalExp(list, logicalExpIndex)) {
        ex3::parser(list, i + cmd, true);
    }
    return args;
}

int PrintCommand::execute(std::vector<std::string> *list, int i)  {
    if (variables_map.count(list->at(i + 1)) > 0)
        cout << variables_map[list->at(i + 1)]->getValue() << endl;
    else
        cout << list->at(i + 1) << endl;
    return args;
}

int SleepCommand::execute(std::vector<std::string> *list, int i)  {
    sleep(stod(list->at(i + 1)));
    return args;
}