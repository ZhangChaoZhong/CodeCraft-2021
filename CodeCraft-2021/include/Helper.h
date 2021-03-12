//
// Created by zcz on 2021/3/11.
//

#ifndef CODECRAFT_2021_HELPER_H
#define CODECRAFT_2021_HELPER_H

#include "common.h"
class Helper {
public:
    void printServerType(ServerType &obj);
    void printVMType(VMType &obj);
    void printRequest(Request &obj);
    void printPurchaser(Purchaser &obj);
    void printMigration(Migration &obj);
    void printDeployment(Deployment &obj);
};


#endif //CODECRAFT_2021_HELPER_H
