//
// Created by Daniel Eisenberg on 21/12/2019.
//

#include <arpa/inet.h>
#include "Client_Side.h"
#include <vector>
#include "DatabaseManager.h"
#include "Command.h"
#include <queue>

/**
 * create a socket that acts as a client. The client sends updates to the simulator.
 * the function finishes when the main update the boolean value that keeps the loop running.
 * @param ip the ip of the socket
 * @param port the port of the socket
 * @return a number different then 0 if there is a failure of any sort
 */
 int Client_Side::createAndRunClient(const char* ip, const char* port) {

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        std::cerr << "Could not create client socket" << std::endl;
        return -1;
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip);
    address.sin_port = htons(*port);
    int is_connect = connect(client_socket, (struct sockaddr*)&address, sizeof(address));
    if (is_connect == -1) {
        std:: cerr << "Could not connect to server" << std:: endl;
        return -2;
    } else {
        std:: cout <<"Client connect to server" << std::endl;
    }
    sleep(10);
    // loop condition updated by the main function
    while (!Client_Side::getKillClientThread()) {
        std::queue<std::string>* command_queue = DatabaseManager::get().getSimCommandsQ();
        //pass commands to the simulator
        while (!command_queue->empty()){
            const char* message = command_queue->front().c_str();
            int sent = send(client_socket, message, strlen(message), 0);
            if (sent == -1) {
                std:: cerr << "Error sending message" << std::endl;
                return -3;
            } else {
                command_queue->pop();
            }
        }
        //release the main thread
        if (Client_Side::getClientFlag()) {
            Client_Side::setClientFlag(1);
            CommandUtil::cv.notify_all();
        }
    }
    close(client_socket);
    //update the main thread that the thread is finished
    Client_Side::killClientThread(1);
    CommandUtil::cv.notify_all();
    return 0;
}
/**
 * release the block call of the connect control client command.
 * @param i when i equals 0 we release the block call
 */
void Client_Side::setClientFlag(int i) {
    if (!i) {
        client_flag = true;
    } else {
        client_flag = false;
    }
}
/**
 * Kill the client thread.
 * @param i when i equals 0 we kill the thread
 */
void Client_Side::killClientThread(int i) {
    if (!i)
        kill_client_thread = true;
    else
        kill_client_thread = false;
}
/**
 * @return the boolean var in charge of killing the thread.
 */
bool Client_Side::getKillClientThread() {
    return kill_client_thread;
}
/**
 * @return the flag.
 */
bool Client_Side::getClientFlag() {
    return client_flag;
}