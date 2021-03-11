//
// Created by zcz on 2021/3/11.
//

#include "Helper.h"
#include <iostream>
using namespace std;

void Helper::printServer(Server &obj) {
    cout << obj.serverType << "," << obj.cpus << "," << obj.memory << "," << obj.hardCost << "," << obj.energyCost
         << "," << obj.id << endl;
}

void Helper::printSale(Sale &obj) {
    cout << obj.serverType << "," << obj.cpus << "," << obj.memory << "," << obj.isDouble << endl;
}

void Helper::printRequest(Request &obj) {
    cout << obj.requestType << "," << obj.serverType << "," << obj.id << endl;
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