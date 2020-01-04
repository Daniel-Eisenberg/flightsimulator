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
    // Initiate the array of the XML values with the simulator variable values.
    std::vector<std::string> simArray  = {
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

public:
    static DatabaseManager& get();
    std::queue<std::string>* getSimCommandsQ();
    void updateDataFromSim(std::vector<double> dataFromSim);
    void addToSimCommandsQ(std::string command);
    void putToVariablesMap(std::string varName, Variable* variable);
    Variable* getFromVariablesMap(std::string varName, int scope) throw();
    bool isVariableExist(std::string varName);
    void putToFunctionMap(std::string funcName, CreateFunctionCommand* functionCommand);
    CreateFunctionCommand getFromFunctionMap(std::string funcName);
    bool isFunctionExist(std::string funcName);
    double getFromSimVariablesMap(std::string varName);
    void initSimVariablesMap();
    void clearVariablesScope(int scope);
};


#endif //EX3_DATABASEMANAGER_H
