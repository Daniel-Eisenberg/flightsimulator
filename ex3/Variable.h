//
// Created by Yuval Grinberg on 23/12/2019.
//

#ifndef EX3_VARIABLE_H
#define EX3_VARIABLE_H


#include <string>

class Variable {
    int value;
    std::string sim;
    bool shouldUpdateSim;
public:
    void setValue(int value);
    int getValue();
    void setSim(std::string sim);
    std::string getSim();
    void setShouldUpdateSim(bool val);
    bool getShouldUpdateSim();
};


#endif //EX3_VARIABLE_H
