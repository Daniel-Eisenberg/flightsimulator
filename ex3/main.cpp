#include "Variable.h"
#include "Lexer.h"
#include "Parser.h"
#include "Command.h"
#include "Tcp_Server.h"
#include "Client_Side.h"



int main(int argc, char *argv[]) {


    std::string filename = argv[argc - 1];
    filename = "fly.txt";
    vector<string> a;
    try {
        a = Lexer::lexerCode(filename);
    } catch (const char* e){
        return -20;
    }

    
    Parser::parser(&a, 0, false, 0);
    std::unique_lock<std::mutex> ul(Command::lock);
    Client_Side::killClientThread(0);
    Command::cv.wait(ul, [] {return !Client_Side::getKillClientThread();});
    Tcp_Server::killServerThread(0);
    Command::cv.wait(ul, [] {return !Tcp_Server::getKillServerThread();});
    Parser::clearMap();

    return 0;
}

