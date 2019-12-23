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
                string mathExp = parseMath(*list, i + 4);
                Interpreter *inter = new Interpreter();
                Expression *exp = inter->interpret(mathExp);
                int value = int(exp->calculate());
            }
            else { // Point a new variable to an existing one
                variables_map[varName] = variables_map[list.at(i + 3)];
                args = 3;
            }
    }
    return args;
}

string parseMath(std::vector<std::string> *list, int i) {
    string mathExp = "";
    while (list.at(i) != "$") {
        if (isalpha(list.at(i)))
            mathExp += variables_map[list.at(i)]->value;
        else
            mathExp += list.at(i);
        i++;
    }
    return mathExp;
}

bool estimateLogicalExp(std::vector<std::string> *list, int i) {
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
                string mathExp = parseMath(*list, i + 4);
                Interpreter *inter = new Interpreter();
                Expression *exp = inter->interpret(mathExp);
                int value = int(exp->calculate());
            } else { // Point a new variable to an existing one
                variables_map[varName] = variables_map[list.at(i + 3)];
                args = 3;
            }
    }
}

int SetVarCommand::execute(std::vector<std::string> *list, int i)  {
    Variable var;
    string varName = list.at(i + 1);
    if (list.at(i + 3) == "$") { // Calc and set a math expression
        string mathExp = parseMath(*list, i + 4);
        Interpreter *inter = new Interpreter();
        Expression *exp = inter->interpret(mathExp);
        int value = int(exp->calculate());
        variables_map[varName]->setValue(value);
    }
    else { // Point a new variable to an existing one
        variables_map[varName] = variables_map[list.at(i + 3)];
        args = 3;
    }
    return args;
}

int WhileLoopCommand::execute(std::vector<std::string> *list, int i)  {
    //connect to the server;
    return args;
}

int IfCommand::execute(std::vector<std::string> *list, int i)  {
    //connect to the server;
    return args;
}

int PrintCommand::execute(std::vector<std::string> *list, int i)  {
    print(list.at(i + 1));
    return args;
}