#include "Lexer.h"

int main(int argc, char *argv[]) {


    std::string filename = argv[argc - 1];
    //filename = "fly.txt";
    vector<string> a;
    try {
        a = Lexer::lexerCode(filename);
    } catch (const char* e){
        return -20;
    }


//    for (string &x : a) {
//        cout << x << ", " << endl;
//    }

    Parser::parser(&a, 0, false, 0);
    thread2 = false;
    thread3 = false;
//    while(signal1 || signal2){}

    return 0;
}

