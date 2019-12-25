//
// Created by Yuval Grinberg on 23/12/2019.
//

#include "Command.h"
#include "Tcp_Server.h"


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
                calcAndSetMath(*list, i + 4);
                args = findStopSign(*list, i+3, "$");
            } else
                cout << "ERROR no math exp $ after = tag."
    }
    return args;
}

int Command::parseMath(std::vector<std::string> *list, int i, std::string varName, bool isLogicExp) {
    string mathExp = "";
    int value;
    if (isalpha(list.at(i)) && list.at(i + 1) == "$") {
        variables_map[varName] = variables_map[list.at(i)];
        value = variables_map[list.at(i)]->value;
    } else {
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
        variables_map[varName].setValue(value);
    }

    return value;
}

int evaluateExp(std::vector<std::string> *list, int i) {
    string firstElement = list.at(i);
    if (firstElement == "$")
        return parseMath(*list, i + 1);
    else if (isalpha(firstElement))
        return variables_map[list.at(i)]->value;
    else if (isnumber(firstElement))
        return firstElement;

    return 0;
}

int findStopSign(std::vector<std::string> *list, int i, string sign) {
    args = i;
    while (list.at(i) != sign) {
        args++;
    }
    args++;
}

bool evaluateLogicalExp(std::vector<std::string> *list, int i) {
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

int SetVarCommand::execute(std::vector<std::string> *list, int i)  {
    Variable var;
    string varName = list.at(i + 1);

    if (list.at(i + 3) == "$") { // Calc and set a math expression
        calcAndSetMath(*list, i + 4, varName);
        args = findStopSign(*list, i+3, "$");
    }

    return args;
}

int WhileLoopCommand::execute(std::vector<std::string> *list, int i)  {
    args = findStopSign(*list, i+1, "}");
    while(evaluateLogicalExp(*list, i + 1) && list.at(i) != "}") {
        parser();
        i++;
    }
    return args;
}

int IfCommand::execute(std::vector<std::string> *list, int i)  {
    args = findStopSign(*list, i+1, "}");
    if(evaluateLogicalExp(*list, i + 1)) {
        while (list.at(i) != "}") {
            parser();
            i++;
        }
    }
    return args;
}

int PrintCommand::execute(std::vector<std::string> *list, int i)  {
    print(list.at(i + 1));
    return args;
}

int SleepCommand::execute(std::vector<std::string> *list, int i)  {
    sleep(int(list.at(i + 1)));
    return args;
}