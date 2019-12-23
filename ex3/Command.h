//
// Created by Yuval Grinberg on 23/12/2019.
//

#ifndef EX3_COMMAND_H
#define EX3_COMMAND_H

#include <list>
#include <map>
#include "Variable.h"

class Command {
protected:
    std::map<std::string, Variable*> variables_map;
public:
    int virtual execute(std::vector<std::string> *list, int index);
};

class OpenServerCommand : public Command {
    int args = 1;
public:
    int virtual execute(std::vector<std::string> *list, int index);
};

class ConnectCommand : public Command {
    int args = 2;
public:
    int virtual execute(std::vector<std::string> *list, int index);
};

class DefineVarCommand : public Command {
    int args = 4;
public:
    int virtual execute(std::vector<std::string> *list, int index);
};

class SetVarCommand : public Command {
    int args;
public:
    int virtual execute(std::vector<std::string> *list, int index);
};

class WhileLoopCommand : public Command {
    int args;
public:
    int virtual execute(std::vector<std::string> *list, int index);
};

class PrintCommand : public Command {
    int args = 1;
public:
    int virtual execute(std::vector<std::string> *list, int index);
};

class IfCommand : public Command {
    int args = 1;
public:
    int virtual execute(std::vector<std::string> *list, int index);
};


#endif //EX3_COMMAND_H
