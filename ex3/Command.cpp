//
// Created by Yuval Grinberg on 23/12/2019.
//

#include "Command.h"
#include "Tcp_Server.h"
#include <vector>
#include "ex1.h"
#include "ex3.h"
#include "Expression.h"
#include "DatabaseManager.h"
#include <iostream>
#include <thread>

using namespace std;
// Local static class methods
// ----------------------------

//static std::map<std::string, Variable*> variables_map;
std::mutex mServer;
bool flag = true;


int parseMathExp(std::vector<std::string> *list, int i, int scope) {
    string mathExp;
    double value;
    if (list->at(i) == "$") // if by mistake we got the first $
        i++;
    while (list->at(i) != "$") {
        if (isalpha(list->at(i)[0]))
            mathExp += to_string(DatabaseManager::get().getFromVariablesMap(list->at(i)).getValue(scope));
        else
            mathExp += list->at(i);
        i++;
    }
    Interpreter* inter = new Interpreter();
    Expression *exp = inter->interpret(mathExp);
    value = exp->calculate();

    return value;
}

// Finds the next occurrence of a string in the array.
int Command::findSign(std::vector<std::string> *list, int i, const string& sign) {
    int args = 1;
    while (list->at(i) != sign) {
        i++;
        args++;
    }
    args++;

    return args;
}

// Finds the next occurrence of a string in the array
int Command::findClosingBracket(std::vector<std::string> *list, int i) {
    int args = 1; int innerParenthesis = 1;
    int openParen = findSign(list, i, "{");
    i = i + openParen + 1;
    while (innerParenthesis > 0) {
        if (list->at(i) == "{")
            innerParenthesis++;
        if (list->at(i) == "}")
            innerParenthesis--;
        i++;
        args++;
    }
    args++;

    return args;
}

// Return boolean evaluation of a logical expression
bool static evaluateLogicalExp(std::vector<std::string> *list, int i, int scope) {
    int leftStart = i;
    int leftEnd = i + Command::findSign(list, leftStart + 1, "$");
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

int OpenServerCommand::execute(std::vector<std::string> *list, int i, int scope) {
    Tcp_Server *server = new Tcp_Server();
    int port = stoi(list->at(i + 1));
    std::thread serverThread(&Tcp_Server::create_socket, port);
    while(flag){}
    delete server;
    return args;
}

int ConnectCommand::execute(std::vector<std::string> *list, int i, int scope)  {
    Client_Side *client = new Client_Side();
    const char* ip = list->at(i + 1).c_str();
    const char* port = list->at(i + 2).c_str();
    std::thread connectionThread(&Client_Side::create, ip, port);
    return args;
}

int DefineVarCommand::execute(std::vector<std::string> *list, int i, int scope)  {
    Variable *var = nullptr;
    string varName = list->at(i + 1);
    string defType = list->at(i + 2);
    if (defType == "<-") {// Define a variable that gets data from simulator
        string sim = list->at(i + 3);
        var = new Variable(sim, false, scope);
        DatabaseManager::get().putToVariablesMap(varName, var);
        args = 4;
    } else if (defType == "->") {// Define a variable that sets data to the simulator
        string sim = list->at(i + 3);
        var = new Variable(sim, true, scope);
        DatabaseManager::get().putToVariablesMap(varName, var);
        args = 4;
    } else if (defType == "=") {// Define a variable that sets data to the simulator
        if (list->at(i + 3) == "$") { // Calc and set a math expression
            double value = parseMathExp(list, i + 4, scope);
            var = new Variable("", false, scope);
            var->setValue(value);
            DatabaseManager::get().putToVariablesMap(varName, var);
            args = 2 + findSign(list, i + 4, "$");
        } else
            cout << "ERROR no math exp $ after = tag.";
    }

    return args;
}


int SetVarCommand::execute(std::vector<std::string> *list, int i, int scope)  {
    string varName = list->at(i + 1);

    if (list->at(i + 3) == "$") { // Calc and set a math expression
        int value = parseMathExp(list, i + 4, scope);
        DatabaseManager::get().getFromVariablesMap(varName).setValue(value);
        args = 2 + findSign(list, i + 4, "$");
    } else
        cout << "ERROR no math exp $ after = tag.";

    return args;
}

int WhileLoopCommand::execute(std::vector<std::string> *list, int i, int scope)  {
    int openParen = findSign(list, i, "{");
    args = 1 + findClosingBracket(list, i + openParen + 1);
    int logicalExpIndex = i + 1;
    while(evaluateLogicalExp(list, logicalExpIndex, scope) && list->at(i) != "}") {
        ex3::parser(list, i + openParen, true, scope + 1);
    }
    return args;
}

int IfCommand::execute(std::vector<std::string> *list, int i, int scope)  {
    int openParen = findSign(list, i, "{");
    args = 1 + findClosingBracket(list, i + openParen + 1);
    int logicalExpIndex = i + 1;
    if(evaluateLogicalExp(list, logicalExpIndex, scope)) {
        ex3::parser(list, i + openParen, true, scope + 1);
    }
    return args;
}

int FunctionCommand::execute(std::vector<std::string> *list, int i, int scope)  {
    int openParen = findSign(list, i, "{");
    args = 1 + findClosingBracket(list, i + openParen + 1);
    ex3::parser(list, i + openParen, true, scope + 1);
    return args;
}

int PrintCommand::execute(std::vector<std::string> *list, int i, int scope)  {
    string data = list->at(i + 1);
    if (DatabaseManager::get().isVariableExist(data) && data[0] != '\"')
        cout << DatabaseManager::get().getFromVariablesMap(data).getValue(scope) << endl;
    else
        cout << data << endl;
    return args;
}

int SleepCommand::execute(std::vector<std::string> *list, int i, int scope)  {
    string data = list->at(i + 1);
    sleep(stod(data));
    return args;
}