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

/**
 * Initiate the object by creating the member variable objects and creating the simulator variables map
 */
DatabaseManager::DatabaseManager() {
    simCommandsQ = new queue<string>;
    variablesMap = new map<std::string, Variable*>;
    simVariablesMap = new map<std::string, double>;
    initSimVariablesMap();
}

/**
 * Iterate all the simulator variable names from the header array and create
 * a map
 */
void DatabaseManager::initSimVariablesMap() {
    for (string sim : simArray) {
        (*simVariablesMap)[sim] = 0;
    }
}

/**
 * @return a reference to the simulator command queue which holds all the update commands we
 * want to send to the simulator, after every command use we will dequeue the command
 */
std::queue<std::string>* DatabaseManager::getSimCommandsQ() {
    return simCommandsQ;
}

/**
 * Take the vector of data we got from the simulator and update the simulator variables map with it.
 * @param dataFromSim raw data from simulator
 */
void DatabaseManager::updateDataFromSim(std::vector<double> dataFromSim) {
    for (int i = 0; i < dataFromSim.size(); i++) {
        string sim = simArray.at(i);
        double value = dataFromSim[i];
        std::cout << "sim=" << sim << " value=" << value <<endl;
        (*simVariablesMap)[sim] = value;
    }
}

/**
 * Add command to the commands queue, command format is:
 * "set /location value"
 * Commands sent to this method are not verified and expected to be in the correct format
 * @param command
 */
void DatabaseManager::addToSimCommandsQ(std::string command) {
    simCommandsQ->push(command);
}

/**
 * Add a value to the variables map
 * @param varName var name
 * @param variable object ref
 */
void DatabaseManager::putToVariablesMap(std::string varName, Variable *variable) {
    (*variablesMap)[varName] = variable;
}

/**
 * @param varName
 * @return the variable of the current name, if out of scope / non exists return the NOT_FOUND object
 */
Variable DatabaseManager::getFromVariablesMap(std::string varName, int scope) {
    if (isVariableExist(varName, scope))
        return *(*variablesMap)[varName];
    return *Variable::NOT_FOUND_VAR;
}

/**
 * Check if a variable exists
 * @param varName
 * @return
 */
bool DatabaseManager::isVariableExist(std::string varName, int scope) {
    bool isExists = variablesMap->count(varName) > 0;
    bool isInScope;
    if (isExists)
        isInScope = scope >= (*variablesMap)[varName]->getScope();
    return isExists && isInScope;
}

/**
 * @param varName
 * @return the most updated value of the variable from the simulator
 */
double DatabaseManager::getFromSimVariablesMap(std::string varName) {
    return (*simVariablesMap)[varName];
}

/**
 * @return the singleton instace of the object *thread safe
 */
DatabaseManager& DatabaseManager::get() {
    if (!instance)
        instance = new DatabaseManager();
    return *instance;
}

/**
 * Clear the variables scope that we leave (at the end of a method / while / if command)
 * @param scope
 */
void DatabaseManager::clearVariablesScope(int scope) {
    for (auto&& [key, value] : *variablesMap) {
        if (value->getScope() >= scope)
            (*variablesMap).erase(key);
    }
}
