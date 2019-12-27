//
// Created by Yuval Grinberg on 23/12/2019.
//

#ifndef EX3_VARIABLE_H
#define EX3_VARIABLE_H


#include <string>

class Variable {
    double value = 0;
    std::string sim = "";
    bool shouldUpdateSim = false;
    int scope;
public:
    Variable(double value, std::string sim, bool shouldUpdateSim);
    int getValue();
    void setValue(int value);
    std::string getSim();
    bool getShouldUpdateSim();
};


#endif //EX3_VARIABLE_H
