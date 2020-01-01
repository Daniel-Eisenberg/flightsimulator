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
    this->value = getValueFromServer(sim);

    if (this->scope != scope)
        throw "Variable is out of scope!";

    return value;
}

double Variable::getValueFromServer(std::string sim) {
    DatabaseManager databaseManager = DatabaseManager::get();
    return 0;
}

void Variable::updateValueToServer(double value, std::string sim) {

}

int Variable::getScope() {
    return this->scope;
}
