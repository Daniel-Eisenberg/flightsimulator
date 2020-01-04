//
// Created by Yuval Grinberg on 23/12/2019.
//

#ifndef EX3_COMMAND_H
#define EX3_COMMAND_H
#include <list>
#include <map>
#include <vector>
#include "Variable.h"

extern bool flag;
extern bool thread2;
extern bool thread3;
extern bool signal1;
extern bool signal2;
class Command {
public:
    int virtual execute(std::vector<std::string> *list, int index, int scope) {};
    int static findSign(std::vector<std::string> *list, int i, const std::string &sign);
    int static findClosingBracket(std::vector<std::string> *list, int i);
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

class CreateFunctionCommand : public Command {
    int args = 0;
    int methodBeginIndex = 0;
    std::vector<std::string> varNames = {};
public:
    static std::map<std::string, CreateFunctionCommand*> functionMap;
    int getBeginIndex();
    std::vector<std::string> getVarNamesVector();
    int virtual execute(std::vector<std::string> *list, int index, int scope);
};

class RunFunctionCommand : public Command {
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
