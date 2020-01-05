//
// Created by Daniel Eisenberg on 21/12/2019.
//

#ifndef EX3_CLIENT_SIDE_H
#define EX3_CLIENT_SIDE_H
#include <sys/socket.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include<arpa/inet.h>

static bool client_flag = true;
static bool kill_client_thread = false;

class Client_Side {
public:
    static int createAndRunClient(const char* ip, const char *port);
    static void setClientFlag(int i);
    static bool getClientFlag();
    static void killClientThread(int i);
    static bool getKillClientThread();
};


#endif //EX3_CLIENT_SIDE_H
