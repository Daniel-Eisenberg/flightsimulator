//
// Created by Daniel Eisenberg on 27/12/2019.
//

#ifndef EX3_EX3_H
#define EX3_EX3_H


#include <iostream>
#include <string>
#include "Tcp_Server.h"
#include "Client_Side.h"
#include <iostream>
#include <fstream>
#include<sstream>
#include <vector>
#include<unordered_map>
#include "Command.h"
using namespace std;

class ex3 {
public:
    OpenServerCommand c;
    vector<string> lexerCode(std::string filename);
    static void parser(vector<string> *params, unsigned index, bool scope);
    static unordered_map<string, Command> command_map;

};

#endif //EX3_EX3_H
