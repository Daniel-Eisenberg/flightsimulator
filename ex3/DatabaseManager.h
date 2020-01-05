//
// Created by Yuval Grinberg on 30/12/2019.
//

#ifndef EX3_DATABASEMANAGER_H
#define EX3_DATABASEMANAGER_H


#include <string>
#include <queue>
#include <map>
#include "Variable.h"
#include "Command.h"

/**
 * A singleton object that will hold all the maps and data shared by all the other objects.
 */
class DatabaseManager {
    DatabaseManager();
    std::queue<std::string> *simCommandsQ;
    std::map<std::string, Variable*> *variablesMap;
    std::map<std::string, double> *simVariablesMap;
    std::map<std::string, CreateFunctionCommand*> *functionMap;
    static DatabaseManager *instance;
    std::vector<std::string> simArray  = {};
public:
    static DatabaseManager& get();
    ~DatabaseManager();
    std::queue<std::string>* getSimCommandsQ();
    void updateDataFromSim(std::vector<double> dataFromSim);
    void addToSimCommandsQ(std::string command);
    void putToVariablesMap(std::string varName, Variable* variable);
    Variable* getFromVariablesMap(std::string varName, int scope) noexcept(false);
    bool isVariableExist(std::string varName);
    void putToFunctionMap(std::string funcName, CreateFunctionCommand* functionCommand);
    CreateFunctionCommand getFromFunctionMap(std::string funcName);
    bool isFunctionExist(std::string funcName);
    double getFromSimVariablesMap(std::string varName);
    void initSimVariablesMap();
    void clearVariablesScope(int scope);
};


#endif //EX3_DATABASEMANAGER_H
