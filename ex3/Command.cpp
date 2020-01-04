//
// Created by Yuval Grinberg on 23/12/2019.
//

#include "Command.h"
#include "Tcp_Server.h"
#include <vector>
#include "MathParser.h"
#include "Lexer.h"
#include "Expression.h"
#include "DatabaseManager.h"
#include <iostream>
#include <thread>

using namespace std;
// Local static class methods
// ----------------------------

bool flag = true;
extern bool thread2 = true;
extern bool thread3 = true;
extern bool signal1 = true;
extern bool signal2 = true;

/**
 * Parse a mathematical expression into a value
 * Taking variables in account
 * $ is a self-implemented mark on the beginning and end of a math expression
 * @param list of parameters
 * @param i index of current location
 * @param scope the scope we are running in
 * @return
 */
double parseMathExp(std::vector<std::string> *list, int i, int scope) {
    string mathExp;
    double value;
    if (list->at(i) == "$") // if by mistake we got the first $
        i++;
    while (list->at(i) != "$") {
        if (isalpha(list->at(i)[0]))
            try {
                mathExp += to_string(DatabaseManager::get().getFromVariablesMap(list->at(i), scope).getValue());
            } catch (char *e) {
                cout << "Error parsing variable to math expression: " << e;
            }
        else
            mathExp += list->at(i);
        i++;
    }
    Interpreter* inter = new Interpreter();
    Expression *exp = inter->interpret(mathExp);
    value = exp->calculate();

    return value;
}

/**
 * Finds the next occurrence of a string in the params array.
 * @param list of parameters
 * @param i index of current location
 * @param sign to search
 * @return
 */
int Command::findSign(std::vector<std::string> *list, int i, const string& sign) {
    int args = 1;
    while (list->at(i) != sign) {
        i++;
        args++;
    }
    args++;

    return args;
}

/**
 * Finds the closing parenthesis taking in account nested expressions
 * @param list of parameters
 * @param i index of current location
 * @return
 */
int Command::findClosingBracket(std::vector<std::string> *list, int i) {
    int innerParenthesis = 1;
    int openParen = findSign(list, i, "{") - 2;
    int args = 1 + openParen;
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

/**
 * Return boolean after evaluating a logical expression, used for If and While command
 * @param list of parameters
 * @param i index of current location
 * @param scope the scope we are running in
 * @return
 */
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

/**
 * Initiate the server connection
 * @param list of parameters
 * @param i index of current location
 * @param scope the scope we are running in
 * @return
 */
int OpenServerCommand::execute(std::vector<std::string> *list, int i, int scope) {
    int port = stoi(list->at(i + 1));
    std::thread serverThread(&Tcp_Server::create_socket, port);
    while(flag){}
    serverThread.detach();
    return args;
}

/**
 * Initate a connection as a client
 * @param list of parameters
 * @param i index of current location
 * @param scope the scope we are running in
 * @return number of arguments that the parser should skip
 */
int ConnectCommand::execute(std::vector<std::string> *list, int i, int scope)  {
    const char* ip = list->at(i + 1).c_str();
    const char* port = list->at(i + 2).c_str();
    flag = true;
    std::thread connectionThread(&Client_Side::create, ip, port);
    while(flag){}
    connectionThread.detach();
    return args;
}

/**
 * Defining a new variable into the variables map
 * We will set the variable differently according to the set type (<-,->,=)
 * @param list of parameters
 * @param i index of current location
 * @param scope the scope we are running in
 * @return number of arguments that the parser should skip
 */
int DefineVarCommand::execute(std::vector<std::string> *list, int i, int scope)  {
    Variable *var = nullptr;
    string varName = list->at(i + 1);
    string defType = list->at(i + 2);
    if (defType == "<-") { // Define a variable that gets data from simulator
        string sim = list->at(i + 4);
        var = new Variable(sim, false, scope);
        DatabaseManager::get().putToVariablesMap(varName, var);
        args = 4;
    } else if (defType == "->") { // Define a variable that sets data to the simulator
        string sim = list->at(i + 4);
        var = new Variable(sim, true, scope);
        DatabaseManager::get().putToVariablesMap(varName, var);
        args = 4;
    } else if (defType == "=") { // Define a variable without any connection to the simulator
        if (list->at(i + 3) == "$") { // Calculate and set a math expression
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

/**
 * Setting a value to an existing variable
 * @param list of parameters
 * @param i index of current location
 * @param scope the scope we are running in
 * @return number of arguments that the parser should skip
 */
int SetVarCommand::execute(std::vector<std::string> *list, int i, int scope)  {
    string varName = list->at(i + 1);

    if (list->at(i + 3) == "$") { // Calculate and set a math expression
        int value = parseMathExp(list, i + 4, scope);
        try {
            DatabaseManager::get().getFromVariablesMap(varName, scope).setValue(value);
        } catch (char *e) {
            cout << "Error setting variable: " << e;
        }
        args = 2 + findSign(list, i + 4, "$");
    } else
        cout << "Error: no math expression $ after = tag.";

    return args;
}

/**
 * Iterating a while a logical expression is true
 * Use the parser in scoped mode to run the commands inside the loop scope
 * Add one to the scope so the variable's would not be accesable from upper scopes
 * @param list of parameters
 * @param i index of current location
 * @param scope the scope we are running in
 * @return number of arguments that the parser should skip
 */
int WhileLoopCommand::execute(std::vector<std::string> *list, int i, int scope)  {
    int openParen = findSign(list, i, "{") - 2;
    args = 1 + findClosingBracket(list, i + 1);
    int logicalExpIndex = i + 1;
    while(evaluateLogicalExp(list, logicalExpIndex, scope) && list->at(i) != "}") {
        // Call the parser in scoped mode
        Parser::parser(list, i + openParen + 1, true, scope + 1);
        sleep(1);
    }
    return args;
}

/**
 * Run a piece of code if a logical expression is true
 * Use the parser in scoped mode to run the commands inside the if scope
 * Add one to the scope so the variable's would not be accesable from upper scopes
 * @param list of parameters
 * @param i index of current location
 * @param scope the scope we are running in
 * @return number of arguments that the parser should skip
 */
int IfCommand::execute(std::vector<std::string> *list, int i, int scope)  {
    int openParen = findSign(list, i, "{") - 2;
    args = 1 + findClosingBracket(list, i + openParen + 1);
    int logicalExpIndex = i + 1;
    if(evaluateLogicalExp(list, logicalExpIndex, scope)) {
        Parser::parser(list, i + openParen, true, scope + 1);
    }
    return args;
}

/**
 * Create a function with the relevant variables for future use
 * Add the function the function command map
 * @param list of parameters
 * @param i index of current location
 * @param scope the scope we are running in
 * @return number of arguments that the parser should skip
 */
int FunctionCommand::execute(std::vector<std::string> *list, int i, int scope)  {
    int openParen = findSign(list, i, "{") - 2;
    args = 1 + findClosingBracket(list, i + openParen + 1);
    Parser::parser(list, i + openParen, true, scope + 1);
    return args;
}

/**
 * Print a message to the screen,
 * If the message is a variable, print the value, otherwise print the string as found by the lexer.
 * @param list of parameters
 * @param i index of current location
 * @param scope the scope we are running in
 * @return number of arguments that the parser should skip
 */
int PrintCommand::execute(std::vector<std::string> *list, int i, int scope)  {
    string data = list->at(i + 1);
    if (DatabaseManager::get().isVariableExist(data, scope) && data[0] != '\"')
        try {
            cout << DatabaseManager::get().getFromVariablesMap(data, scope).getValue() << endl;
        } catch (char *e) {
            cout << "Error printing variable: " << e;
        }
    else
        cout << data << endl;
    return args;
}

/**
 * Set the thread to sleep by milliseconds.
 * @param list of parameters
 * @param i index of current location
 * @param scope the scope we are running in
 * @return number of arguments that the parser should skip
 */
int SleepCommand::execute(std::vector<std::string> *list, int i, int scope)  {
    string data = list->at(i + 1);
    sleep(5);
    return args;
}