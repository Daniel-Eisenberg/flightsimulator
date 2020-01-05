//
// Created by Daniel Eisenberg on 20/12/2019.
//

#include <string>
#include <vector>
#include "Variable.h"
#include "Tcp_Server.h"
#include "Command.h"
#include "Lexer.h"
#include "DatabaseManager.h"

void cleanString(char* str, int i);
/**
 * create server socket that get requests from the simulator. the requests are values that gets updated in a data base.
 * @param port od the sever
 * @return a number different then 0 if there is a failure of any sort
 */
int Tcp_Server::createAndRunServer(int port) {

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
    if (::bind(socket1, (struct sockaddr *) &address, sizeof(address)) == -1) {
        std::cerr << "could not bind the socket to an ip" << std::endl;
        return -2;
    }

    //socket listen to port
    if (listen(socket1, 1) == -1) {
        std::cerr << "Error during listening" << std::endl;
        return -3;
    }


    //accept client
        int client_socket = accept(socket1, (struct sockaddr *) &address, (socklen_t *) &address);
        if (client_socket == -1) {
            std::cerr << "Error accepting client" << std::endl;
            return -4;
        }

    // loop condition updated by the main function

    char message[1024] = {0};
    while (!Tcp_Server::getKillServerThread()) {

        read(client_socket, message, 1024);
        int i;
        string s;
        for (i = 0; i < 1024; i++) {
            if (message[i] == '\0')
                break;
            s += message[i];
        }
        vector<string> values = Lexer::split(s, ",");
        vector<double> double_values;
        for (string x : values) {
            if(x != "")
                double_values.push_back(stod(x));
        }
        //sends values only when it gets all the values of the simulator
        if (double_values.size() == 36)
            DatabaseManager::get().updateDataFromSim(double_values);
        //release the main thread
        cleanString(message, i);
        if (Tcp_Server::getServerFlag()) {
            Tcp_Server::setServerFlag(1);
            Command::cv.notify_all();
        }
    }
    close(socket1);
    //update the main thread that the thread is finished
    Tcp_Server::killServerThread(1);
    Command::cv.notify_all();
    return 0;
}

/**
 * realse the block call of the open data server command.
 * @param i when i equals 0 we release the block call
 */
void Tcp_Server::setServerFlag(int i) {
    if (!i) {
        server_flag = true;
    } else {
        server_flag = false;
    }
}
/**
 * Kill the server thread.
 * @param i when i equals 0 we kill the thread
 */
void Tcp_Server::killServerThread(int i) {
    if (!i)
        kill_server_thread = true;
    else
        kill_server_thread = false;
}

/**
 * @return the boolean var in charge of killing the thread.
 */
bool Tcp_Server::getKillServerThread() {
    return kill_server_thread;
}

/**
 * @return the flag.
 */
bool Tcp_Server::getServerFlag() {
    return server_flag;
}

/**
 * clean a given string
 * @param str the string
 * @param i the end of the string
 */
void cleanString(char* str, int i) {
    for (int j = 0; j < i; j++) {
        str[j] = '\0';
    }
}