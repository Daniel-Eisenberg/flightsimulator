//
// Created by Yuval Grinberg on 23/12/2019.
//

#ifndef EX3_COMMAND_H
#define EX3_COMMAND_H

#include <list>
#include <map>
#include "Variable.h"

class Command {
public:
    int virtual execute(std::vector<std::string> *list, int index, int scope);
    int static findStopSign(std::vector<std::string> *list, int i, const std::string &sign);
    static void clearVariablesScope(int scope);
};

class OpenServerCommand : public Command {
    int args = 1;
public:
    int virtual execute(std::vector<std::string> *list, int index, int scope);
};

class ConnectCommand : public Command {
    int args = 2;
public:
    int virtual execute(std::vector<std::string> *list, int index, int scope);
};

class DefineVarCommand : public Command {
    int args = 4;
public:
    int virtual execute(std::vector<std::string> *list, int index, int scope);
};

class SetVarCommand : public Command {
    int args;
public:
    int virtual execute(std::vector<std::string> *list, int index, int scope);
};

class WhileLoopCommand : public Command {
    int args = 0;
public:
    int virtual execute(std::vector<std::string> *list, int index, int scope);
};

class IfCommand : public Command {
    int args = 0;
public:
    int virtual execute(std::vector<std::string> *list, int index, int scope);
};

class FunctionCommand : public Command {
    int args = 0;
public:
    int virtual execute(std::vector<std::string> *list, int index, int scope);
};

class PrintCommand : public Command {
    int args = 1;
public:
    int virtual execute(std::vector<std::string> *list, int index, int scope);
};

class SleepCommand : public Command {
    int args = 1;
public:
    int virtual execute(std::vector<std::string> *list, int index, int scope);
};


#endif //EX3_COMMAND_H
