//
// Created by Yuval Grinberg on 30/12/2019.
//

#include "DatabaseManager.h"
#include <queue>
#include <map>
#include <iostream>
#include "Variable.h"

using namespace std;

DatabaseManager* DatabaseManager::instance = NULL;

DatabaseManager::DatabaseManager() {
    simCommandsQ = new queue<string>;
    variablesMap = new map<std::string, Variable*>;
    simVariablesMap = new map<std::string, double>;
    initSimVariablesMap();
}

void DatabaseManager::initSimVariablesMap() {
    for (string sim : simArray) {
        (*simVariablesMap)[sim] = 0;
    }
}

std::queue<std::string>* DatabaseManager::getSimCommandsQ() {
    return simCommandsQ;
}

void DatabaseManager::updateDataFromSim(std::vector<double> dataFromSim) {
    for (int i = 0; i < dataFromSim.size(); i++) {
        string sim = simArray.at(i);
        double value = dataFromSim[i];
        std::cout << "sim=" << sim << " value=" << value <<endl;
        (*simVariablesMap)[sim] = value;
    }
}

void DatabaseManager::addToSimCommandsQ(std::string command) {
    simCommandsQ->push(command);
}

void DatabaseManager::putToVariablesMap(std::string varName, Variable *variable) {
    (*variablesMap)[varName] = variable;
}

Variable DatabaseManager::getFromVariablesMap(std::string varName) {
    return *(*variablesMap)[varName];
}

double DatabaseManager::getFromSimVariablesMap(std::string varName) {
    return (*simVariablesMap)[varName];
}

DatabaseManager& DatabaseManager::get() {
    if (!instance)
        instance = new DatabaseManager();
    return *instance;
}

bool DatabaseManager::isVariableExist(std::string varName) {
    return variablesMap->count(varName) > 0;
}

// Clear the variables scope that we leave (finishing a method etc.)
void DatabaseManager::clearVariablesScope(int scope) {
    for (auto&& [key, value] : *variablesMap) {
        if (value->getScope() >= scope)
            (*variablesMap).erase(key);
    }
}
