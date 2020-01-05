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
    functionMap = new map<std::string, CreateFunctionCommand*>;
    initSimVariablesMap();
}

/**
 * Iterate all the simulator variable names from the header array and create a map
 */
void DatabaseManager::initSimVariablesMap() {
    // Initiate the array of the XML values with the simulator variable values.
    simArray = {
            "/instrumentation/airspeed-indicator/indicated-speed-kt",
            "/sim/time/warp",
            "/controls/switches/magnetos",
            "/instrumentation/heading-indicator/offset-deg",
            "/instrumentation/altimeter/indicated-altitude-ft",
            "/instrumentation/altimeter/pressure-alt-ft",
            "/instrumentation/attitude-indicator/indicated-pitch-deg",
            "/instrumentation/attitude-indicator/indicated-roll-deg",
            "/instrumentation/attitude-indicator/internal-pitch-deg",
            "/instrumentation/attitude-indicator/internal-roll-deg",
            "/instrumentation/encoder/indicated-altitude-ft",
            "/instrumentation/encoder/pressure-alt-ft",
            "/instrumentation/gps/indicated-altitude-ft",
            "/instrumentation/gps/indicated-ground-speed-kt",
            "/instrumentation/gps/indicated-vertical-speed",
            "/instrumentation/heading-indicator/indicated-heading-deg",
            "/instrumentation/magnetic-compass/indicated-heading-deg",
            "/instrumentation/slip-skid-ball/indicated-slip-skid",
            "/instrumentation/turn-indicator/indicated-turn-rate",
            "/instrumentation/vertical-speed-indicator/indicated-speed-fpm",
            "/controls/flight/aileron",
            "/controls/flight/elevator",
            "/controls/flight/rudder",
            "/controls/flight/flaps",
            "/controls/engines/engine/throttle",
            "/controls/engines/current-engine/throttle",
            "/controls/switches/master-avionics",
            "/controls/switches/starter",
            "/engines/active-engine/auto-start",
            "/controls/flight/speedbrake",
            "/sim/model/c172p/brake-parking",
            "/controls/engines/engine/primer",
            "/controls/engines/current-engine/mixture",
            "/controls/switches/master-bat",
            "/controls/switches/master-alt",
            "/engines/engine/rpm"
    };

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
        std::cout << "updateDataFromSim sim=" << sim << " value=" << value <<endl;
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
Variable* DatabaseManager::getFromVariablesMap(std::string varName, int scope) {
    if (!isVariableExist(varName))
        throw "Variable " + varName +  " does not exists!";
    else if (scope < (*variablesMap)[varName]->getScope())
        throw "Variable " + varName +  " is out of scope!";
    return (*variablesMap)[varName];
}

/**
 * Check if a variable exists
 * @param varName
 * @return
 */
bool DatabaseManager::isVariableExist(std::string varName) {
    return variablesMap->count(varName) > 0;
}

/**
 * @param varName
 * @return the most updated value of the variable from the simulator
 */
double DatabaseManager::getFromSimVariablesMap(std::string varName) {
    return (*simVariablesMap)[varName];
}


/**
 * Clear the variables scope that we leave (at the end of a method / while / if command)
 * @param scope
 */
void DatabaseManager::clearVariablesScope(int scope) {
    for (auto &k : *variablesMap) {
        if (k.second->getScope() >= scope) {
            delete k.second;
            (*variablesMap).erase(k.first);
        }
    }
}

void DatabaseManager::putToFunctionMap(std::string funcName, CreateFunctionCommand *functionCommand) {
    (*functionMap)[funcName] = functionCommand;
}

CreateFunctionCommand DatabaseManager::getFromFunctionMap(std::string funcName) {
    return *(*functionMap)[funcName];
}

bool DatabaseManager::isFunctionExist(std::string funcName) {
    return functionMap->count(funcName) > 0;
}

/**
 * @return the singleton instace of the object *thread safe
 */
DatabaseManager& DatabaseManager::get() {
    if (!instance)
        instance = new DatabaseManager();
    return *instance;
}

DatabaseManager::~DatabaseManager() {
    for (const auto &k  : *variablesMap) {
        delete k.second;
    }

    for (const auto &k : *functionMap) {
        delete k.second;
    }

    delete simCommandsQ;
    delete variablesMap;
    delete simVariablesMap;
    delete functionMap;
    delete instance;
}
