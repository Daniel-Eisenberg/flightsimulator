//
// Created by Daniel Eisenberg on 27/12/2019.
//

#ifndef EX3_LEXER_H
#define EX3_LEXER_H


#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include "Command.h"
#include "Parser.h"
#include "DatabaseManager.h"
#include "Tcp_Server.h"
#include "Client_Side.h"

using namespace std;

class Lexer {
public:
    static vector<string> split(const string& s, char delimiter);
    static std::vector<std::string> split(std::string str,std::string delimiter);
    static vector<string> lexerCode(std::string filename);
};

#endif //EX3_LEXER_H
