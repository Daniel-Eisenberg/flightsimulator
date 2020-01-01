//
// Created by Yuval Grinberg on 30/12/2019.
//

#include "DatabaseManager.h"
#include <queue>
#include <map>
#include "Variable.h"

using namespace std;

DatabaseManager::DatabaseManager() {
    simCommandsQ = new queue<string>;
    variablesMap = new map<std::string, Variable*>;
    simVariablesMap = new map<std::string, double>;
    initSimVariablesMap();
}

void DatabaseManager::initSimVariablesMap() {
    *simArray = {
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

    for (string sim : *simArray) {
        (*simVariablesMap)[sim] = 0;
    }
}

std::queue<std::string> DatabaseManager::getSimCommandsQ() {
    return *simCommandsQ;
}

void DatabaseManager::updateDataFromSim(std::vector<double> dataFromSim) {
    for (int i = 0; i <= dataFromSim.size(); i++) {
        string sim = simArray->at(i);
        double value = dataFromSim[i];
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
    static DatabaseManager databaseManager;
    return databaseManager;
}

bool DatabaseManager::isVariableExist(std::string varName) {
    return variablesMap->count(varName) > 0;
}

// Clear the variables scope that we leave (finishing a method etc.)
void DatabaseManager::clearVariablesScope(int scope) {
    for (auto&& [varName, variable] : *variablesMap) {
        if (variable->getScope() >= scope)
            (*variablesMap).erase(varName);
    }
}
