//
// Created by Yuval Grinberg on 23/12/2019.
//

#ifndef EX3_VARIABLE_H
#define EX3_VARIABLE_H


#include <string>

class Variable {
    double value = 0;
    bool valueInit = false;
    std::string sim = "";
    bool shouldUpdateSim = false;
    int scope;
public:
    Variable(std::string sim, bool shouldUpdateSim);
    double getValue();
    void setValue(double value);
    std::string getSim();
    bool getShouldUpdateSim();
    double getValueFromServer(std::string sim);
    void updateValueToServer(double value, std::string sim);
};


#endif //EX3_VARIABLE_H
