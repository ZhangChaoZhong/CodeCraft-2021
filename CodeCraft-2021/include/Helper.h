//
// Created by zcz on 2021/3/11.
//

#ifndef CODECRAFT_2021_HELPER_H
#define CODECRAFT_2021_HELPER_H

#include "common.h"
class Helper {
public:
    void printServerType(ServerType &obj);
    void printServer(Server &obj);
    void printVMType(VMType &obj);
    void printRequest(Request &obj);
    void printPurchaser(Purchaser &obj);
    void printMigration(Migration &obj);
    void printDeployment(Deployment &obj);
    void generateServer(std::string &serverType,std::string &cpuCores,std::string &memorySize,std::string &serverCost,std::string &powerCost);
};


#endif //CODECRAFT_2021_HELPER_H
