//
// Created by Yuval Grinberg on 23/12/2019.
//

#include "Variable.h"

void Variable::setValue(int value) {
    this->value = value;
}
int Variable::getValue() {
    return value;
}
void Variable::setSim(std::string sim) {
    this->sim = sim;
}
std::string Variable::getSim() {
    return sim;
}
void Variable::setShouldUpdateSim(bool val) {
    this->shouldUpdateSim = val;
}
bool Variable::getShouldUpdateSim() {
    return shouldUpdateSim;
}