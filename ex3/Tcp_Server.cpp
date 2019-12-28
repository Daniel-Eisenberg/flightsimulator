//
// Created by Daniel Eisenberg on 20/12/2019.
//

#include "Tcp_Server.h"

int Tcp_Server::create_socket(int port) {

        int socket1 = socket(AF_INET, SOCK_STREAM, 0);
        if (socket1 == -1) {
            std::cerr << "could not create server socket" << std::endl;
            return -1;
        }

        sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);


        // bind
        if (bind(socket1, (struct sockaddr *) &address, sizeof(address)) == -1) {
            std::cerr << "could not bind the socket to an ip" << std::endl;
            return -2;
        }

        //socket listen to port
        if (listen(socket1, 1) == -1) {
            std::cerr << "Error during listening" << std::endl;
            return -3;
        }

        while (true) {
            //accept client
            int client_socket = accept(socket1, (struct sockaddr *) &address, (socklen_t *) &address);
            if (client_socket == -1) {
                std::cerr << "Error accepting client" << std::endl;
                return -4;
            }

            char buffer[1024] = {0};
            int read_client = read(client_socket, buffer, 1024);
            std::cout << buffer << std::endl;
            char *hello = "I can hear you \n";
            send(client_socket, hello, strlen(hello), 0);
            std::cout << "message sent\n" << std::endl;
        }


}