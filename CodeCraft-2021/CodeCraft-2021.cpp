#include <iostream>
#include "Solution.h"
using namespace std;

int main() {
    // TODO:read standard input
    // TODO:process
    // TODO:write standard output
    // TODO:fflush(stdout);

    //string txtPath = "training-1.txt";
    //Solution solution(txtPath);
    Solution solution;
    solution.run();
    return 0;
}

//void testTime(){
//    clock_t start = clock();
//    //do something
//
//    clock_t ends = clock();
//    cout <<"Running Time : "<<(double)(ends - start)/ CLOCKS_PER_SEC << endl;
//}
//
//int main(){
//    //测试输出
//    Helper helper;
//    Server server = {"NV603", 92, 324, 53800, 500};
//    helper.printServer(server);
//
//    Sale sale={"c3.large.4", 2, 8, 0};
//    helper.printSale(sale);
//
//    Request request = {"add", "c3.large.4", 5};
//    helper.printRequest(request);
//
//    Purchaser purchaser = {"NV603", 1};
//    helper.printPurchaser(purchaser);
//
//    Migration migration={1,3,'A'};
//    helper.printMigration(migration);
//
//    Deployment deployment ={1, 'B'};
//    helper.printDeployment(deployment);
//    return 0;
//}