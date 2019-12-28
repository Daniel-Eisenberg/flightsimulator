//
// Created by Yuval Grinberg on 23/12/2019.
//

#include "Variable.h"

Variable::Variable(std::string sim, bool shouldUpdateSim, int scope) {
    this->sim = sim;
    this->shouldUpdateSim = shouldUpdateSim;
    if (sim != "")
        this->value = getValueFromServer(sim);
    this->valueInit = true;
    this->scope = scope;
}

void Variable::setValue(double value) {
    this->value = value;
    if (this->shouldUpdateSim && sim != "")
        updateValueToServer(value, this->sim);
    this->valueInit = true;
}

double Variable::getValue(int scope) {
    if (valueInit)
        return value;
    if (this->scope != scope)
        throw "Variable is out of scope!";
    else
        throw "Uninitilization error!";
}

double Variable::getValueFromServer(std::string sim) {
    return 0;
}

void Variable::updateValueToServer(double value, std::string sim) {

}

int Variable::getScope() {
    return this->scope;
}
