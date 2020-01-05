//
// Created by Daniel Eisenberg on 27/12/2019.
//

#include "Lexer.h"

/**
 * split a string by delimeter (char).
 * @param str the string to split
 * @param delimiter
 * @return a vector containing the parts of the splited string
 */
std::vector<std::string> Lexer::split(const string& str, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(str);
    static unordered_map<string, Command> command_map;
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

/**
 * split a string by delimeter (string).
 * @param str the string to split
 * @param delimiter
 * @return a vector containing the parts of the splited string
 */
std::vector<std::string> Lexer::split(std::string str, std::string delimiter){

    std::vector<std::string> arr;
    size_t pos = 0;
    std::string token;
    while ((pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, pos);
        if (token != "")
            arr.push_back(token);
        str.erase(0, pos + delimiter.length());
    }
    arr.push_back(str);
    return arr;
}

/**
 * erase double quotes from a given string.
 * @param s the string
 */
void eraseDoubleQuotes(string &s) {
    size_t pos = 0;
    while ((pos = s.find('\"')) != std::string::npos) {
        s.erase(pos, 1);
    }
}

/**
 * remove spaces from a given string.
 * @param str the string
 */
void removeSpaces(char *str) {
    // To keep track of non-space character count
    int count = 0;
    // Traverse the given string. If current character
    // is not space, then place it at index 'count++'
    for (int i = 0; str[i]; i++)
        if (str[i] != ' ')
            str[count++] = str[i];
    str[count] = '\0';
}

/**
 * remove tabs from a given string.
 * @param str the string
 */
void removeTabs(char *str) {
    // To keep track of non-tab character count
    int count = 0;
    // Traverse the given string. If current character
    // is not tab, then place it at index 'count++'
    for (int i = 0; str[i]; i++)
        if (str[i] != '\t')
            str[count++] = str[i];
    str[count] = '\0';
}

/**
 * detect what kind of operator is in an expression.
 * @param str the expression
 * @return the operator
 */
string findOperator(string str) {
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

/**
 * the lexer method. open a file containing code and split the code to tokens.
 * @param filename the file the function reads from
 * @return a vector containing of all the tokens
 */
vector<string> Lexer::lexerCode(std::string filename) {

    unordered_map<string, bool> functions;
    vector<string> params;
    vector<string> temp;
    fstream file;
    file.open(filename, std::fstream::in);
    if (!file.is_open()) {
        throw "didnt read file";
    }
    string line, line_with_spaces;
    //the file is being read line by line
    while (getline(file, line)) {
        //remove tabs and spaces
        char string1[1024];
        strcpy(string1, line.c_str());
        removeTabs(string1);
        line_with_spaces = string1;
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
            eraseDoubleQuotes(temp.at(0));
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
                params.push_back(line.substr(start_pos + 2, end_pos - start_pos - 3));
            } else if (line.find("<-") != std::string::npos) {
                params.push_back("<-");
                params.push_back("sim");
                unsigned long start_pos = line.find('(');
                unsigned long end_pos = line.find(')');
                params.push_back(line.substr(start_pos + 2, end_pos  - start_pos - 3));
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
            string str = findOperator(line);
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
            while (line_with_spaces.rfind(" ", 0) == 0) {
                line_with_spaces.erase(0, 1);
            }
            vector<string> param = split(line_with_spaces, '(');
            params.push_back(param.at(0));
            params.push_back(split(param.at(1), ')').at(0));
        } else if (line.rfind("Sleep", 0) == 0) {
            vector<string> param = split(line, '(');
            params.push_back(param.at(0));
            params.push_back(split(param.at(1), ')').at(0));
        } else if (line.rfind("}", 0) == 0) {
            params.push_back("}");
        } else if (line.find('{') != string::npos) {
            vector<string> param = split(line, '(');
            params.push_back("creatFunc");
            params.push_back(param.at(0));
            functions[param.at(0)] = true;
            param = split(param.at(1), ')');
            if (param.at(0).rfind("var", 0) == 0) {
                param = split(param.at(0), "var");
                for (string x : param) {
                    params.push_back("var");
                    params.push_back(x);
                }
            }
            params.push_back("{");
        } else if (line.find('(') != string::npos) {
            vector<string> param = split(line, '(');
            if (functions.count(param.at(0)) > 0) {
                params.push_back("runFunc");
                params.push_back(param.at(0));
                params.push_back("(");
                param = split(param.at(1), ')');
                param =  split(param.at(0), ',');
                for (string x : param) {
                    params.push_back(x);
                }
                params.push_back(")");
            }

        }
    }
    file.close();
    return params;
}

