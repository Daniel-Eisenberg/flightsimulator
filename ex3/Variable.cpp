//
// Created by Yuval Grinberg on 23/12/2019.
//

#include "Variable.h"
#include "DatabaseManager.h"

Variable::Variable(std::string sim, bool shouldUpdateSim, int scope) {
    this->sim = sim;
    this->shouldUpdateSim = shouldUpdateSim;
    if (sim != "")
        this->value = getValueFromServer(sim);
    this->scope = scope;
}

void Variable::setValue(double value) {
    this->value = value;
    if (this->shouldUpdateSim && sim != "")
        updateValueToServer(value, this->sim);
}

double Variable::getValue(int scope) {
    if (sim != "")
        this->value = getValueFromServer(sim);

    if (this->scope != scope)
        throw "Variable is out of scope!";

    return value;
}

double Variable::getValueFromServer(std::string sim) {
    return DatabaseManager::get().getFromSimVariablesMap(sim);
}

void Variable::updateValueToServer(double value, std::string sim) {
    std::string command = "set " + sim + " value";
    DatabaseManager::get().addToSimCommandsQ(command);
}

int Variable::getScope() {
    return this->scope;
}
