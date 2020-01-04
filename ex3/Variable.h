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
    int scope = 0;
    double getValueFromServer(std::string sim);
    void updateValueToServer(double value, std::string sim);
public:
    Variable(std::string sim, bool shouldUpdateSim, int scope);
    double getValue();
    int getScope();
    void setValue(double value);
    ~Variable();
};


#endif //EX3_VARIABLE_H
