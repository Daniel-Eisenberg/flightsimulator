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
std::mutex CommandUtil::lock;
std::condition_variable CommandUtil::cv;


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
        string data = list->at(i);
        if (isalpha(data[0]) && DatabaseManager::get().isVariableExist(data))
            try {
                mathExp += to_string(DatabaseManager::get().getFromVariablesMap(list->at(i), scope)->getValue());
            } catch (char *e) {
                cout << "Error parsing variable to math expression: " << e;
            }
        else
            mathExp += data;
        i++;
    }
    Interpreter* inter = new Interpreter();
    Expression *exp = inter->interpret(mathExp);
    value = exp->calculate();

    delete inter;
    delete exp;

    return value;
}

/**
 * Finds the next occurrence of a string in the params array.
 * @param list of parameters
 * @param i index of current location
 * @param sign to search
 * @return
 */
int CommandUtil::findSign(std::vector<std::string> *list, int i, const string& sign) {
    int args = 1;
    while (list->at(i) != sign) {
        i++;
        args++;
        unsigned long iLong = i;
        if (iLong == list->size()) {
            cout << "Out of bound exception i=" << i;
            break;
        }
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
int CommandUtil::findClosingBracket(std::vector<std::string> *list, int i) {
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
    int leftEnd = i + CommandUtil::findSign(list, leftStart + 1, "$");
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
/**
 * Initiate the server connection
 * @param list of parameters
 * @param i index of current location
 * @param scope the scope we are running in
 * @return
 */
int OpenServerCommand::execute(std::vector<std::string> *list, int i, int scope) {
    int port = stoi(list->at(i + 1));
    std::thread serverThread(&Tcp_Server::createAndRunServer, port);
    std::unique_lock<std::mutex> ul(CommandUtil::lock);
    CommandUtil::cv.wait(ul, [] {return !Tcp_Server::getServerFlag();});
    serverThread.detach();
    (void)scope; // Suppress unused warning
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
    std::thread connectionThread(&Client_Side::createAndRunClient, ip, port);
    std::unique_lock<std::mutex> ul(CommandUtil::lock);
    CommandUtil::cv.wait(ul, [] {return !Client_Side::getClientFlag();});
    connectionThread.detach();
    (void)scope; // Suppress unused warning
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
            args = 2 + CommandUtil::findSign(list, i + 4, "$");
        } else
            cout << "Error: no math exp $ after = tag.";
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
        double value = parseMathExp(list, i + 4, scope);
        try {
            DatabaseManager::get().getFromVariablesMap(varName, scope)->setValue(value);
        } catch (char *e) {
            cout << "Error setting variable: " << e;
        }
        args = 2 + CommandUtil::findSign(list, i + 4, "$");
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
    int openParen = CommandUtil::findSign(list, i, "{") - 2;
    args = CommandUtil::findClosingBracket(list, i + 1) - 1;
    int logicalExpIndex = i + 1;
    while(evaluateLogicalExp(list, logicalExpIndex, scope) && list->at(i) != "}") {
        // Call the parser in scoped mode
        Parser::parser(list, i + openParen + 1, true, scope + 1);
        sleep(1);
    }

    // Clear the variables from the scope we are leaving so they won't interrupt new variables
    DatabaseManager::get().clearVariablesScope(scope + 1);

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
    int openParen = CommandUtil::findSign(list, i, "{") - 2;
    args = CommandUtil::findClosingBracket(list, i + 1) - 1;
    int logicalExpIndex = i + 1;
    if(evaluateLogicalExp(list, logicalExpIndex, scope)) {
        Parser::parser(list, i + openParen + 1, true, scope + 1);
    }

    // Clear the variables from the scope we are leaving so they won't interrupt new variables
    DatabaseManager::get().clearVariablesScope(scope + 1);

    return args;
}

/**
 * Create a function with the relevant variables for future use
 * Add the function the function command map
 * Add the variable names to a vector that will help initialize the argument variables
 * Save the start of the function code to help run the code
 * @param list of parameters
 * @param i index of current location
 * @param scope the scope we are running in
 * @return number of arguments that the parser should skip
 */
int CreateFunctionCommand::execute(std::vector<std::string> *list, int i, int scope)  {
    string funcName = list->at(i + 1);
    int openParen = CommandUtil::findSign(list, i, "{") - 2;
    methodBeginIndex = i + openParen + 1;
    args = CommandUtil::findClosingBracket(list, i + 1) - 1;

    i += 2;
    while (list->at(i) != "{") { // For functions with variables
        if (list->at(i) != "var")
            varNames.push_back(list->at(i));
        i++;
    }

    // Add the function to the function command map
    DatabaseManager::get().putToFunctionMap(funcName, this);

    (void)scope; // Suppress unused warning
    return args;
}

// Marks the begining of the function code for the parser when running
int CreateFunctionCommand::getBeginIndex() {
    return methodBeginIndex;
}

// The argument vars that should be included in the function
std::vector<std::string> CreateFunctionCommand::getVarNamesVector() {
    return varNames;
}

/**
 * Load the function from the function command map by name
 * Get the variable to
 * @param list of parameters
 * @param i index of current location
 * @param scope the scope we are running in
 * @return number of arguments that the parser should skip
 */
int RunFunctionCommand::execute(std::vector<std::string> *list, int i, int scope)  {
    string funcName = list->at(i + 1);
    unsigned long openParen = CommandUtil::findSign(list, i, "(") - 2;
    unsigned long closeParen = CommandUtil::findSign(list, i, ")") - 2;
    args += closeParen;

    // Get the function objects and its members
    CreateFunctionCommand func = DatabaseManager::get().getFromFunctionMap(funcName);
    int methodBeginIndex = func.getBeginIndex();
    std::vector<std::string> varNames = func.getVarNamesVector();

    if (closeParen - openParen - 1 != varNames.size()) {
        cout << "Error running the function: " + funcName + ", missing arguments.";
        return args;
    }

    if (varNames.size() > 0) {
        i += openParen + 1;
        for (string varName : varNames) {
            double value = 0;
            if (isalpha(list->at(i)[0]))
                try {
                    value = DatabaseManager::get().getFromVariablesMap(list->at(i), scope)->getValue();
                } catch (char *e) {
                    cout << "Error loading a variable to function arguments: " << e;
                }
            else
                value = stod(list->at(i));
            Variable* var = new Variable("", false, scope + 1);
            var->setValue(value);
            DatabaseManager::get().putToVariablesMap(varName, var);
            i++;
        }
    }
    Parser::parser(list, methodBeginIndex, true, scope + 1);
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
    if (data[0] != '\"' && DatabaseManager::get().isVariableExist(data))
        try {
            cout << DatabaseManager::get().getFromVariablesMap(data, scope)->getValue() << endl;
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
    sleep(stoi(data));

    (void)scope; // Suppress unused warning
    return args;
}
