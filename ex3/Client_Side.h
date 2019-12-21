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
#define PORT 5402

class Client_Side {
public:
    int create();
};


#endif //EX3_CLIENT_SIDE_H
