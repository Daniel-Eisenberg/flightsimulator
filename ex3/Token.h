//
// Created by Yuval Grinberg on 18/11/2019.
//

#ifndef EX1_TOKEN_H
#define EX1_TOKEN_H

class Token{
    bool isNum = false;
    bool isOper = false;
    bool isVar = false;
    std::string value = "";
public:
    Token(std::string arg);
    ~Token(){};
    std::string getValue();
    bool isOperator();
    bool isNumber();
    bool isVariable();
    bool isPrecedent(Token t);
};
#endif //EX1_TOKEN_H
