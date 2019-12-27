#include "ex3.h"

int main(int argc, char *argv[]) {


    std::string filename = argv[argc - 1];
    vector<string> a;
    try {
        a = lexerCode(filename);
    } catch (const char* e){
        return -20;
    }


    for (string &x : a) {
        cout << x << ", " << endl;
    }


    return 0;
}

