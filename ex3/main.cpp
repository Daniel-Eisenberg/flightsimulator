#include <iostream>
#include <string>
#include "Tcp_Server.h"
#include "Client_Side.h"
#include <iostream>
#include <fstream>
#include<sstream>
#include <vector>

using namespace std;

vector<string> split(const string& s, char delimiter)
{
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}
void removeSpaces(char *str) {
    // To keep track of non-space character count
    int count = 0;

    // Traverse the given string. If current character
    // is not space, then place it at index 'count++'
    for (int i = 0; str[i]; i++)
        if (str[i] != ' ')
            str[count++] = str[i]; // here count is
    // incremented
    str[count] = '\0';
}
void lexerCode(std::string filename) {

    vector<string> params;
    vector<string> temp;
    fstream file;
    file.open(filename);
    string line;
    while (getline(file, line)) {
        char string1[1024];
        strcpy(string1, line.c_str());
        removeSpaces(string1);
        line = string1;
        if (line.rfind("openDataServer", 0) == 0) {
            temp = split(line, '(');
            params.push_back(temp.at(0));
            temp = split(temp.at(1), ')');
            params.push_back(temp.at(0));
        } else if (line.rfind("connectControlClient", 0) == 0) {
            temp = split(line, '(');
            params.push_back(temp.at(0));
            temp = split(temp.at(1), ',');
            params.push_back(temp.at(0));
            temp = split(temp.at(1), ')');
            params.push_back(temp.at(0));
        } else if (line.rfind("var", 0) == 0) {
            if ((line.find("->") != std::string::npos) || (line.find("<-") != std::string::npos)) {
                params.push_back("var");
                int i = 0;
                while (string1[i] != '\0') {


                    i++;
                }
            }
        }
    }

}

int main(int argc, char *argv[]) {

    Tcp_Server server;
    server.create_socket();
    std::string filename = *argv;
    lexerCode(filename);

    return 0;
}

