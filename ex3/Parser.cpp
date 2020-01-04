//
// Created by Yuval Grinberg on 04/01/2020.
//

#include "Parser.h"

using namespace std;

static unordered_map<string, Command*> command_map;


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

/**
 *
 * @param params list of all the parameters created by the lexer
 * @param index the index to start the parsing from
 * @param isScoped a boolean that sets the scoped mode
 * @param scope the current scope to run the code in (used for variables visibility)
 */
void Parser::parser(vector<string> *params, unsigned index, bool isScoped, int scope) {
    setMap();
    int stopScope;
    if (isScoped)
        stopScope = index + Command::findClosingBracket(params, index - 2) + 1;

    while (index < params -> size()) {
        if (!isScoped) { // Run the regular parse (main parser)
            string current_command = params -> at(index);
            cout << "Command=" << current_command <<" Index=" << index << endl;
            index += command_map.at(current_command)->execute(params, index, scope);
            index++;
        } else if (index < stopScope && params->at(index) != "}") { // Run the scoped parsing
            string current_command = params->at(index);
            cout << "Scoped- Command=" << current_command << " Index=" << index << endl;
            index += command_map.at(current_command)->execute(params, index, scope);
            index++;
        } else
            break;
    }
    // Clear the variables from the scope we are leaving so they won't interrupt new variables
    DatabaseManager::get().clearVariablesScope(scope);
}