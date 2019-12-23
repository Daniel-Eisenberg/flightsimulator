//
// Created by Yuval Grinberg on 23/12/2019.
//

#include "Command.h"
#include "Tcp_Server.h"


int OpenServerCommand::execute(std::vector<std::string> *list, int i) {
    Tcp_Server server;
    server.create_socket();
    return args;
}

int ConnectCommand::execute(std::vector<std::string> *list, int i)  {
    //connect to the server;
    return args;
}

int DefineVarCommand::execute(std::vector<std::string> *list, int i)  {
    if (list[i + 2] == "->" || list[i + 2] == "<-")
        i = i;

    else
        variables_map[list[i+1]] = list[i+1];


    //connect to the server;
    return args;
}

int SetVarCommand::execute(std::vector<std::string> *list, int i)  {
    //connect to the server;
    return args;
}

int WhileLoopCommand::execute(std::vector<std::string> *list, int i)  {
    //connect to the server;
    return args;
}

int PrintCommand::execute(std::vector<std::string> *list, int i)  {
    //connect to the server;
    return args;
}