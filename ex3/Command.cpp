//
// Created by Yuval Grinberg on 23/12/2019.
//

#include "Command.h"
#include "Tcp_Server.h"


// Local static class methods
// ---------------------------

int parseMathExp(std::vector<std::string> *list, int i) {
    string mathExp = "";
    int value;
    while (list.at(i) != "$") {
        if (isalpha(list.at(i)))
            mathExp += variables_map[list.at(i)]->value;
        else
            mathExp += list.at(i);
        i++;
    }
    Interpreter *inter = new Interpreter();
    Expression *exp = inter->interpret(mathExp);
    value = int(exp->calculate());

    return value;
}

// Evaluates an expression to an int
int static evaluateExp(std::vector<std::string> *list, int i) {
    string firstElement = list.at(i);
    if (firstElement == "$")
        return parseMathExp(*list, i + 1);
    else if (isalpha(firstElement))
        return variables_map[list.at(i)]->value;
    else if (isnumber(firstElement))
        return firstElement;

    return 0;
}

// Finds the next occurence of a string in the array
int static findStopSign(std::vector<std::string> *list, int i, string sign) {
    args = 1;
    while (list.at(i) != sign) {
        i++;
        args++;
    }
    args++;
}

// Return boolean evaluation of a logical expression
bool static evaluateLogicalExp(std::vector<std::string> *list, int i) {
    string defType = list.at(i + 2);
    int rightStart = i + 5;
    if ((i + 3) == "$")
        rightStart = findStopSign(*list, i+3, "$") + 1;

    int leftExp = evaluateExp(*list, i+3);
    int rightExp = evaluateExp(*list, rightStart);
    switch (defType) {
        case "==":
            return leftExp == rightExp;
        case "!=":
            return leftExp != rightExp;
        case "<=":
            return leftExp <= rightExp;
        case ">=":
            return leftExp >= rightExp;
        case ">":
            return leftExp > rightExp;
        case "<":
            return leftExp < rightExp;
        default:
            return false;
    }
}



// Subclasses methods
// ---------------------------

int OpenServerCommand::execute(std::vector<std::string> *list, int i) {
    Tcp_Server server;
    server.create_socket();
    return args;
}

int ConnectCommand::execute(std::vector<std::string> *list, int i)  {
    //connect to the server;
    return args;
}

int DefineVarCommand::execute(std::vector<std::string> *list, int i)  {
    Variable var;
    string varName = list.at(i + 1);
    string defType = list.at(i + 2);

    switch (defType) {
        case "<-": // Define a variable that gets data from simulator
            var = new Variable(list.at(i + 4), list.at(i + 3), false);
            variables_map[varName] = var;
            args = 4;
        case "->": // Define a variable that sets data to the simulator
            var = new Variable(list.at(i + 4), list.at(i + 3), true);
            variables_map[varName] = var;
            args = 4;
        case "=":
            if (list.at(i + 3) == "$") { // Calc and set a math expression
                int value = evaluateExp(*list, i + 4);
                variables_map[varName]->setValue(value);
                args = findStopSign(*list, i + 4, "$");
            } else
                cout << "ERROR no math exp $ after = tag."
    }
    return args;
}



//int Command::setVarLogic(std::vector<std::string> *list, int i, std::string varName) {
//    int value;
//    if (isalpha(list.at(i)) && list.at(i + 1) == "$") {
//        variables_map[varName] = variables_map[list.at(i)];
//        value = variables_map[list.at(i)]->value;
//    } else {
//        value = parseMathExp(*list, i);
//        variables_map[varName].setValue(value);
//    }
//    return value;
//}


int SetVarCommand::execute(std::vector<std::string> *list, int i)  {
    Variable var;
    string varName = list.at(i + 1);

    if (list.at(i + 3) == "$") { // Calc and set a math expression
        int value = evaluateExp(*list, i + 4);
        variables_map[varName]->setValue(value);
        args = findStopSign(*list, i + 4, "$");
    } else
        cout << "ERROR no math exp $ after = tag."

    return args;
}

int WhileLoopCommand::execute(std::vector<std::string> *list, int i)  {
    args = findStopSign(*list, i + 1, "}");
    int logicalExpIndex = i + 1;
    while(evaluateLogicalExp(*list, logicalExpIndex) && list.at(i) != "}") {
        parser(*list, i + 1, true);
//        while (list.at(i) != "}") {
//            i += parser(*list);
//        }
    }
    return args;
}

int IfCommand::execute(std::vector<std::string> *list, int i)  {
    args = findStopSign(*list, i + 1, "}");
    int logicalExpIndex = i + 1;
    if(evaluateLogicalExp(*list, logicalExpIndex)) {
        parser(*list, i + 1, true);
//        while (list.at(i) != "}") {
//            i += parser(*list);
//        }
    }
    return args;
}

int PrintCommand::execute(std::vector<std::string> *list, int i)  {
    print(list.at(i + 1));
    return args;
}

int SleepCommand::execute(std::vector<std::string> *list, int i)  {
    thread.sleep(int(list.at(i + 1)));
    return args;
}