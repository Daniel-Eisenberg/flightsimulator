//
// Created by Daniel Eisenberg on 20/12/2019.
//

#include "Tcp_Server.h"
#include "Command.h"
#include <string>
#include <vector>
#include "ex3.h"
extern bool flag;

std::vector<std::string> split(std::string str,std::string delimiter){

    std::vector<std::string> arr;
    size_t pos = 0;
    std::string token;
    while ((pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, pos);
        if (token != "")
            arr.push_back(token);
        str.erase(0, pos + delimiter.length());
    }
    arr.push_back(str);
    return arr;
}




char* getline(int fd){
    int index=0, check;
    char c;
    char* line = (char*)malloc(10 * sizeof(char));
    check = read(fd, &c, 1);

    while(c != '\n' && check == 1){
        if((index+1) % 10 == 0){
            line = (char*)realloc(line, index+11);
        }
        line[index] = c;
        check = read(fd, &c, 1);
        index++;

    }
    if((index+1) % 10 == 0){
        line = (char*)realloc(line, index+11);
    }
    line[index] = '\0';
    return line;
}



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
            char* line = getline(client_socket);
            vector<string> values = split(line, " ");
            //add the values to the map
            sleep(5);
            if (flag)
                flag = false;
        }


}