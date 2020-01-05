//
// Created by Daniel Eisenberg on 20/12/2019.
//

#ifndef EX3_TCP_SERVER_H
#define EX3_TCP_SERVER_H
#include <sys/socket.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>

static bool server_flag = true;
static bool kill_server_thread = false;

class Tcp_Server {

public:
    static int createAndRunServer(int port);
    static void setServerFlag(int i);
    static bool getServerFlag();
    static void killServerThread(int i);
    static bool getKillServerThread();

};


#endif //EX3_TCP_SERVER_H
