//
// Created by Yuval Grinberg on 30/12/2019.
//

#ifndef EX3_DATABASEMANAGER_H
#define EX3_DATABASEMANAGER_H


#include <string>
#include <queue>
#include <map>
#include "Variable.h"


class DatabaseManager {
    std::queue<std::string> *simCommandsQ;
    std::map<std::string, Variable*> *variablesMap;
    std::map<std::string, double> *simVariablesMap;
    std::vector<std::string> *simArray;
    static DatabaseManager *databaseManager;
    DatabaseManager();
public:
    static DatabaseManager& get();
    std::queue<std::string> getSimCommandsQ();
    void updateDataFromSim(std::vector<double> dataFromSim);
    void addToSimCommandsQ(std::string command);
    void putToVariablesMap(std::string varName, Variable* variable);
    Variable getFromVariablesMap(std::string varName);
    double getFromSimVariablesMap(std::string varName);
    void initSimVariablesMap();
};


#endif //EX3_DATABASEMANAGER_H
