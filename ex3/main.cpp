#include "Variable.h"
#include "Lexer.h"
#include "Parser.h"
#include "Command.h"



int main(int argc, char *argv[]) {


    std::string filename = argv[argc - 1];
    filename = "fly.txt";
    vector<string> a;
    try {
        a = Lexer::lexerCode(filename);
    } catch (const char* e){
        return -20;
    }


    for (string &x : a) {
        cout << x << endl;
    }

    Parser::parser(&a, 0, false, 0);
    std::unique_lock<std::mutex> ul(Command::lock);
    Command::killClientThread(0);
    Command::cv.wait(ul, [] {return !Command::getKillClientThread();});
    Command::killServerThread(0);
    Command::cv.wait(ul, [] {return !Command::getKillServerThread();});

    Parser::clearMap();

    return 0;
}

