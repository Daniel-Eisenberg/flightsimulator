//
// Created by Daniel Eisenberg on 21/12/2019.
//

#include <arpa/inet.h>
#include "Client_Side.h"


int Client_Side::create() {

    char message[] = "message";
    while (true) {
        int client_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (client_socket == -1) {
            std::cerr << "cuold not create client socket" << std::endl;
            return -1;
        }

        sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = inet_addr("127.0.0.1");
        address.sin_port = htons(PORT);
        int is_connect = connect(client_socket, (struct sockaddr*)&address, sizeof(address));
        if (is_connect == -1) {
            std:: cerr << "could not connect to server" << std:: endl;
            return -2;
        } else {
            std:: cout <<"client connect to server" << std::endl;
        }
        int sent = send(client_socket, message, strlen(message), 0);
        if (sent == -1) {
            std:: cout << "error sending message" << std::endl;
        } else {
            std::cout  << "message sent" << std:: endl;
        }

        char buffer[1024] = {0};
        int valread = read(client_socket, buffer, 1024);
        std::cout << buffer << std::endl;
        close(client_socket);
    }
}