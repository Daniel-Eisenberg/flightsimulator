//
// Created by Yuval Grinberg on 23/12/2019.
//

#include "Variable.h"
#include "DatabaseManager.h"

/**
 * Construct new variable, if we got sim value try to get it from the server.
 * @param sim the location in of the variable in the simulator
 * @param shouldUpdateSim set true if ->
 * @param scope the scope of the variable (inside a while / if / function would upper the value)
 */
Variable::Variable(std::string sim, bool shouldUpdateSim, int scope) {
    this->sim = sim;
    this->shouldUpdateSim = shouldUpdateSim;
    if (sim != "")
        this->value = getValueFromServer(sim);
    this->scope = scope;
}

/**
 * Set to value to to member variable, if in -> mode then also update the simulator
 * @param value the value to set
 */
void Variable::setValue(double val) {
    this->value = val;
    if (this->shouldUpdateSim && sim != "")
        updateValueToServer(value, this->sim);
}

/**
 * If the sim is not empty get the newest value from the simulator before
 * returning the value, else just return the value inside the object.
 * If the parser scope is smaller then this scope dont return the variable (out of scope error)
 * @param scope the scope the parser is running to check if this is an allowed query
 * @return the value if exists
 */
double Variable::getValue() {
    if (sim != "")
        this->value = getValueFromServer(sim);

    return value;
}

/**
 * Get the updated value from the simulator
 * @param sim the location in of the variable in the simulator
 * @return the updated value
 */
double Variable::getValueFromServer(std::string sim) {
    return DatabaseManager::get().getFromSimVariablesMap(sim);
}

/**
 * Update the value in the simulator by adding it to the simulator command queue
 * @param value the value to update
 * @param sim the location in of the variable in the simulator
 */
void Variable::updateValueToServer(double value, std::string sim) {
    std::string command = "set " + sim + " " + std::to_string(value);
    DatabaseManager::get().addToSimCommandsQ(command);
}

/**
 * @return the variable's scope
 */
int Variable::getScope() {
    return this->scope;
}
