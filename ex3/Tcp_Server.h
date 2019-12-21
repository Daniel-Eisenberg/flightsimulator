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
#define PORT 5402

class Tcp_Server {

public:
// create socket
    int create_socket();
};


#endif //EX3_TCP_SERVER_H
