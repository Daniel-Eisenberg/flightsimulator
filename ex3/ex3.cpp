//
// Created by Daniel Eisenberg on 27/12/2019.
//

#include "ex3.h"


static unordered_map<string, Command*> command_map;

vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);

    static unordered_map<string, Command> command_map;
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

void removeTabs(char *str) {
    // To keep track of non-tab character count
    int count = 0;

    // Traverse the given string. If current character
    // is not tab, then place it at index 'count++'
    for (int i = 0; str[i]; i++)
        if (str[i] != '\t')
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
    if (str.find("!=") != std::string::npos) {
        return "!=";
    } else if (str.find("==") != std::string::npos) {
        return "==";
    } else if (str.find(">=") != std::string::npos) {
        return ">=";
    } else if (str.find("<=") != std::string::npos) {
        return "<=";
    } else if (str.find("<") != std::string::npos) {
        return "<";
    } else if (str.find("<") != std::string::npos) {
        return "<";
    } else return "error";
}


vector<string> ex3::lexerCode(std::string filename) {

    vector<string> params;
    vector<string> temp;
    fstream file;
    file.open(filename, std::fstream::in);
    if (!file.is_open()) {
        throw "didnt read file";
    }
    string line, debug_line;
    while (getline(file, line)) {
        char string1[1024];
        strcpy(string1, line.c_str());
        removeTabs(string1);
        debug_line = string1;
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
                char a = line.at(i);
                if (line.at(i) == '-' || line.at(i) == '<' || line.at(i) == '=') {
                    break;
                }
            }
            params.push_back(line.substr(3, (i - 3)));
            if ((line.find("->") != std::string::npos)) {
                params.push_back("->");
                params.push_back("sim");
                unsigned long start_pos = line.find('(');
                unsigned long end_pos = line.find(')');
                params.push_back(line.substr(start_pos + 1, end_pos - start_pos - 1));
            } else if (line.find("<-") != std::string::npos) {
                params.push_back("<-");
                params.push_back("sim");
                unsigned long start_pos = line.find('(');
                unsigned long end_pos = line.find(')');
                params.push_back(line.substr(start_pos + 1, end_pos  - start_pos - 1));
            } else {
                params.push_back("=");
                vector<string> temp = split(line, '=');
                params.push_back("$");
                params.push_back(temp.at(1));
                params.push_back("$");
            }
        } else if (line.rfind("while", 0) == 0 || line.rfind("if", 0) == 0) {
            string condition;
            if (line.rfind("while", 0) == 0) {
                condition = "while";
            } else {
                condition = "if";
            }
            params.push_back(condition);
            string str = find_operator(line);
            vector<string> param = split(line, condition);
            param = split(param.at(0), str);
            params.push_back("$");
            params.push_back(param.at(0));
            params.push_back("$");
            params.push_back(str);
            params.push_back("$");
            params.push_back(split(param.at(1), '{').at(0));
            params.push_back("$");
            params.push_back("{");
        } else if (line.find('=') != string::npos) {
            params.push_back("#");
            vector<string> temp = split(line, '=');
            params.push_back(temp.at(0));
            params.push_back("=");
            params.push_back("$");
            params.push_back(temp.at(1));
            params.push_back("$");
        }  else if (line.rfind("Print", 0) == 0) {
            vector<string> param = split(debug_line, '(');
            params.push_back(param.at(0));
            params.push_back(split(param.at(1), ')').at(0));
        } else if (line.rfind("Sleep", 0) == 0) {
            vector<string> param = split(line, '(');
            params.push_back(param.at(0));
            params.push_back(split(param.at(1), ')').at(0));
        } else if (line.rfind("}", 0) == 0) {
            params.push_back("}");
        }
    }
    file.close();
    return params;
}

void setMap() {

    OpenServerCommand *c1 = new OpenServerCommand();
    ConnectCommand *c2 = new ConnectCommand();
    DefineVarCommand *c3 = new DefineVarCommand();
    SetVarCommand *c4 = new SetVarCommand();
    WhileLoopCommand *c5 = new WhileLoopCommand();
    IfCommand *c6 = new IfCommand();
    PrintCommand *c7 = new PrintCommand();
    SleepCommand *c8 = new SleepCommand();
    command_map.insert(
            {{"openDataServer", c1},
            {"connectControlClient", c2},
            {"var", c3},
            {"#", c4},
            {"while", c5},
            {"if", c6},
            {"Print", c7},
            {"Sleep", c8}}
    );
}

void ex3::parser(vector<string> *params, unsigned index, bool isScoped, int scope) {
    setMap();
    int stopScope = index + Command::findStopSign(params, index, "}") - 2;


    while (index < params -> size()) {
        if (!isScoped) {
            string current_command = params -> at(index);
            cout << "Command=" << current_command <<" Index=" << index << endl;
            index += command_map.at(current_command)->execute(params, index, scope);
            index++;
        } else if (index < stopScope) {
            string current_command = params->at(index);
            cout << "Scoped- Command=" << current_command << " Index=" << index << endl;
            index += command_map.at(current_command)->execute(params, index, scope);
            index++;
        } else
            break;
    }
    Command::clearVariablesScope(scope);
}