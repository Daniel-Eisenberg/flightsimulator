#include <iostream>
#include <string>
#include "Tcp_Server.h"
#include "Client_Side.h"

using namespace std;

void lexerCode(string filename) {
//iterate all line;
//split by?
}

int main(int argc, char *argv[]) {

    Tcp_Server server;
    server.create_socket();
    std::string filename = *argv;
    lexerCode(filename);

    return 0;
}

