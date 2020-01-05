//
// Created by Yuval Grinberg on 04/01/2020.
//

#ifndef EX3_PARSER_H
#define EX3_PARSER_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include "Command.h"
#include "DatabaseManager.h"


using namespace std;

class Parser {
public:
    static void clearMap();
    static void parser(vector<string> *params, unsigned index, bool isScoped, int scope);
};


#endif //EX3_PARSER_H
