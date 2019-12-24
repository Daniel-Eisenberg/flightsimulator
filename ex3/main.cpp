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

std::vector<std::string> split(std::string str,std::string sep){
    char* cstr=const_cast<char*>(str.c_str());
    char* current;
    std::vector<std::string> arr;
    current=strtok(cstr,sep.c_str());
    while(current!=NULL){
        arr.push_back(current);
        current=strtok(NULL,sep.c_str());
    }
    return arr;
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

string removeclosing(string str) {
    vector<string> s = split(str, '(');
    vector<string> temp = split(s.at(1), ')');
    return temp.at(0);
}

string find_operator(string str) {
    if (str.find("!=")) {
        return "!=";
    } else if (str.find("==")) {
        return "==";
    } else if (str.find(">=")) {
        return ">=";
    } else if (str.find("<=")) {
        return "<=";
    } else if (str.find("<")) {
        return "<";
    } else if (str.find("<")) {
        return "<";
    } else return "error";
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
            params.push_back("var");
            unsigned i;
            for (i = 3; i < line.length(); i++) {
                if (line.at(i) == '-' || line.at(i) == '<' || line.at(i) == '=') {
                    break;
                }
            }
            params.push_back(line.substr(3, (3 - i)));
            if ((line.find("->") != std::string::npos)) {
                params.push_back("->");
                params.push_back("sim");
                unsigned long start_pos = line.find('(');
                unsigned long end_pos = line.find(')');
                params.push_back(line.substr(start_pos + 1, end_pos - 1));


            } else if (line.find("<-") != std::string::npos) {
                params.push_back("<-");
                params.push_back("sim");
                unsigned long start_pos = line.find('(');
                unsigned long end_pos = line.find(')');
                params.push_back(line.substr(start_pos + 1, end_pos - 1));
            } else {
                params.push_back("=");
                check_equal_with_yuval;
            }
        } else if (line.find('=') != string::npos) {
            vector<string> temp = split(line, '=');
            params.push_back(temp.at(0));
            params.push_back("=");
            check_equal_with_yuval;
        } else if (line.rfind("while", 0) == 0 || line.rfind("if", 0) == 0) {
            string str = find_operator(line);
            vector<string> param = split(line, str);
            params.push_back(param.at(0));
            params.push_back(str);
            string boolean_string = param.at(1);
            check_equal_with_yuval;
            params.push_back("{");
        } else if (line.rfind("Print", 0) == 0) {
            vector<string> param = split(line, '(');
            params.push_back(param.at(0));
            params.push_back(removeclosing(param.at(1)));
        } else if (line.rfind("Sleep", 0) == 0) {
            vector<string> param = split(line, '(');
            params.push_back(param.at(0));
            params.push_back(removeclosing(param.at(1)));
        } else if (line.rfind("}", 0) == 0) {
            params.push_back("}");
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

