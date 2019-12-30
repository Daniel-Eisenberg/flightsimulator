//
// Created by Yuval Grinberg on 30/12/2019.
//

#ifndef EX3_DATABASEMANAGER_H
#define EX3_DATABASEMANAGER_H


#include <string>
#include <queue>


class DatabaseManager {
public:
    std::queue<std::string> getSimCommandsQ();
    void updateDataFromSim(std::vector<double> dataFromSim);
};


#endif //EX3_DATABASEMANAGER_H
