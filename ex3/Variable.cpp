//
// Created by Yuval Grinberg on 23/12/2019.
//

#include "Variable.h"

Variable::Variable(int value, std::string sim, bool shouldUpdateSim) {
    this->value = value;
    this->sim = sim;
    this->shouldUpdateSim = shouldUpdateSim;
}

void Variable::setValue(int value) {
    this->value = value;
}

int Variable::getValue() {
    return value;
}

std::string Variable::getSim() {
    return sim;
}

bool Variable::getShouldUpdateSim() {
    return shouldUpdateSim;
}