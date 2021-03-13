//
// Created by zcz on 2021/3/11.
//

#include "Helper.h"
#include <iostream>
using namespace std;

void Helper::printServerType(ServerType &obj) {
    cout << obj.serverTypeName << "," << obj.cpus << "," << obj.memory << "," << obj.hardCost << "," << obj.energyCost
         << endl;
}

void Helper::printServerTypeBypercent(ServerType &obj) {
    cout << obj.serverTypeName << "," << obj.cpus << "," << obj.memory << "," << obj.hardCost << "," << obj.energyCost
            << "," << int(obj.percent)<< endl;
}

void Helper::printVMType(VMType &obj) {
    cout << obj.vmTypeName << "," << obj.cpus << "," << obj.memory << "," << obj.isDouble << endl;
}

void Helper::printRequest(Request &obj) {
    if(obj.requestType == "add")
        cout << obj.requestType << "," << obj.vmTypeName << "," << obj.vmId << endl;
    else
        cout << obj.requestType  << "," << obj.vmId << endl;
}

void Helper::printPurchaser(Purchaser &obj) {
    cout << obj.serverType << "," << obj.num << endl;
}

void Helper::printMigration(Migration &obj) {
    if (obj.node == '0') cout << obj.id << "," << obj.targetId << endl;
    else cout << obj.id << "," << obj.targetId << "," << obj.node << endl;
}

void Helper::printDeployment(Deployment &obj) {
    if (obj.node == '0') cout << obj.id << endl;
    else cout << obj.id << "," << obj.node << endl;
}

void Helper::printServer(Server &obj){
    cout << obj.id << "," << obj.serverTypeName  << ",(" << obj.A.first <<","<<obj.A.second << ")," << "(" << obj.B.first <<","<<obj.B.second << ")"<< endl;
}