//
// Created by Daniel Eisenberg on 21/12/2019.
//

#include <arpa/inet.h>
#include "Client_Side.h"
#include <vector>
#include "DatabaseManager.h"
#include "Command.h"
#include <queue>
//hi
std::string Message_to_server(std::vector<std::string> &values) {
    std::string message;
    for (const std::string &i : values) {
        message = i + ", ";
    }
    message += '\0';
    return message;
}


 int Client_Side::create(const char* ip, const char* port) {

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        std::cerr << "cuold not create client socket" << std::endl;
        return -1;
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(5402);
    int is_connect = connect(client_socket, (struct sockaddr*)&address, sizeof(address));
    if (is_connect == -1) {
        std:: cerr << "could not connect to server" << std:: endl;
        return -2;
    } else {
        std:: cout <<"client connect to server" << std::endl;
    }
    sleep(10);
    while (Command::getKillClientThread()) {
        std::queue<std::string> queue = *DatabaseManager::get().getSimCommandsQ();
        std::string s;
        for (int i = 0; i < queue.size(); i++) {
            const char* message = queue.back().c_str();
            int sent = send(client_socket, message, strlen(message), 0);
            if (sent == -1) {
                std:: cerr << "error sending message" << std::endl;
                return -3;
            } else {
                std::cout  << "message sent" << std:: endl;
                queue.pop();
            }
            char buffer[1024] = {0};
            int valread = read(client_socket, buffer, 1024);
            std::cout << buffer << std::endl;
            Command::setFlag(1);
            Command::cv.notify_one();
        }
    }
     close(client_socket);
    Command::killClientThread(1);
    Command::cv.notify_one();
}