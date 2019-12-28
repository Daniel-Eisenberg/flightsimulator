//
// Created by Yuval Grinberg on 23/12/2019.
//

#include "Variable.h"

Variable::Variable(std::string sim, bool shouldUpdateSim) {
    this->sim = sim;
    this->shouldUpdateSim = shouldUpdateSim;
    if (sim != "")
        this->value = getValueFromServer(sim);
    this->valueInit = true;
}

void Variable::setValue(double value) {
    this->value = value;
    if (this->shouldUpdateSim && sim != "")
        updateValueToServer(value, this->sim);
    this->valueInit = true;
}

double Variable::getValue() {
    if (valueInit)
        return value;
    else
        throw "Uninitilization error!";
}

std::string Variable::getSim() {
    return sim;
}

bool Variable::getShouldUpdateSim() {
    return shouldUpdateSim;
}

double Variable::getValueFromServer(std::string sim) {
    return 0;
}

void Variable::updateValueToServer(double value, std::string sim) {

}
