//
// Created by zcz on 2021/3/11.
//

#include "Solution.h"

#include <algorithm>
#include <string>
#include <iostream>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
using namespace std;

Solution::Solution(string &filename){
    this->mFilename = filename;
    this->mDays=0;
    this->sizeFlag = 0;
}

void Solution::input() {
    //服务器类型,请求类型
    string serverTypeName,VMTypeName,requestType;
    //cpu核数,内存,硬件成本，能耗成本，是否双结点部署
    int cpus = 0, memory = 0, hardCost = 0, energyCost = 0,isDouble = 0,vmId=0;

    int fd = open(this->mFilename.c_str(), O_RDONLY);
    int buf_len = 40 * 1024 * 1024;  // 定义buffer大小40M
    char *buf = (char *) mmap(nullptr, buf_len, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);

    char *currentChar = buf;
    int currentNum = 0;
    string curStr;

    /**
     * 购买服务器序列 每种类型买100
     *
     * */
    do {
        currentNum = 10 * currentNum + *currentChar - '0'; // 每次取出数字第一位
        currentChar++;
    } while (*currentChar != '\n'); // 前两个数字以逗号结尾
    this->mServerTypeNum = currentNum;   //服务器类型数量
    currentNum = 0, currentChar++; //跳过换行

    //cout << serverTypeNum << endl;
//    this->mServerTypeByCpu = new ServerType[serverTypeNum];
//    this->mServerTypeByMemory = new ServerType[serverTypeNum];
    for(int i=0;i<this->mServerTypeNum;i++){
        curStr = "";
        // 读取 serverTypeName
        do {
            if(*currentChar != '(') curStr += *currentChar; // 每次取出数字第一位
            currentChar++;
        } while (*currentChar != ','); // 前两个数字以逗号结尾
        serverTypeName = curStr;
        currentNum = 0, currentChar+=2;

        // 读取 cpus
        do {
            currentNum = 10 * currentNum + *currentChar - '0'; // 每次取出数字第一位
            currentChar++;
        } while (*currentChar != ','); // 前两个数字以逗号结尾
        cpus = currentNum;
        currentNum = 0, currentChar+=2; //跳过逗号和空格

        // 读取 memory
        do {
            currentNum = 10 * currentNum + *currentChar - '0';
            currentChar++;
        } while (*currentChar != ',');
        memory = currentNum;
        currentNum = 0, currentChar+=2;

        // 读取 hardCost
        do {
            currentNum = 10 * currentNum + *currentChar - '0';
            currentChar++;
        } while (*currentChar != ',');
        hardCost = currentNum;
        currentNum = 0, currentChar+=2;

        // 读取 energyCost
        do {
            if(*currentChar != ')')
                currentNum = 10 * currentNum + *currentChar - '0';
            currentChar++;
        } while (*currentChar != '\n');
        energyCost = currentNum;
        currentNum = 0, currentChar++;

        ServerType t = {serverTypeName,cpus,memory,hardCost,energyCost};
        this->mServerTypeByCpu[i]=t;
        this->mServerTypeByMemory[i]=t;
        this->mServerTypeMap[serverTypeName] = t;
        //this->mServer.push_back({serverTypeName,this->mServerId++});
        // cout << serverType << "," << cpus << "," << memory << "," << hardCost << "," << energyCost << endl;
    }

    sort(this->mServerTypeByCpu,this->mServerTypeByCpu+this->mServerTypeNum,serverTypeCmpCpu);           //按cpu排序
    sort(this->mServerTypeByMemory,this->mServerTypeByMemory+this->mServerTypeNum,serverTypeCmpMemory);  //按内存排序

    //cout<<"服务器\n";
    //for(int i=0;i<serverTypeNum;i++) {               //加入哈希表
    //     this->mHelper.printServerType(this->mServerTypeByMemory[i]);
    //}

    /**
     * 售卖序列
     *
     * */
    int saleNum;    //售卖数量
    do {
        currentNum = 10 * currentNum + *currentChar - '0'; // 每次取出数字第一位
        currentChar++;
    } while (*currentChar != '\n'); // 前两个数字以逗号结尾
    saleNum = currentNum;
    currentNum = 0, currentChar++; //跳过换行
    //cout << saleNum << endl;

    for(int i=0;i<saleNum;i++){
        curStr = "";
        // 读取 VMTypeName
        do {
            if(*currentChar != '(') curStr += *currentChar; // 每次取出数字第一位
            currentChar++;
        } while (*currentChar != ','); // 前两个数字以逗号结尾
        VMTypeName = curStr;
        currentNum = 0, currentChar+=2;

        // 读取 cpus
        do {
            currentNum = 10 * currentNum + *currentChar - '0'; // 每次取出数字第一位
            currentChar++;
        } while (*currentChar != ','); // 前两个数字以逗号结尾
        cpus = currentNum;
        currentNum = 0, currentChar+=2; //跳过逗号和空格

        // 读取 memory
        do {
            currentNum = 10 * currentNum + *currentChar - '0';
            currentChar++;
        } while (*currentChar != ',');
        memory = currentNum;
        currentNum = 0, currentChar+=2;

        // 读取 isDouble
        do {
            currentNum = 10 * currentNum + *currentChar - '0';
            currentChar++;
        } while (*currentChar != ')');
        isDouble = currentNum;
        currentNum = 0, currentChar+=2;

        VMType t={VMTypeName,cpus,memory,isDouble};
        this->mVMTypeByCpu[i] = t;
        this->mVMTypeByMemory[i] = t;
        this->mMVTypeMap[VMTypeName] = t;

        //this->mVM.push_back({VMTypeName,this->mVMId++});
        //cout << serverType<<" "<<cpus << " " << memory <<" " << isDouble <<endl;
    }


    sort(this->mVMTypeByCpu,this->mVMTypeByCpu+saleNum,vmTypeCmpCpu);           //按cpu排序
    sort(this->mVMTypeByMemory,this->mVMTypeByMemory+saleNum,vmTypeCmpMemory);  //按内存排序

    //cout<<"虚拟机\n";
    //for(int i=0;i<saleNum;i++)
    //    this->mHelper.printVMType(this->mVMTypeByCpu[i]);

    /**
     * 请求序列
     *
     * */

    //请求天数
    do {
        currentNum = 10 * currentNum + *currentChar - '0'; // 每次取出数字第一位
        currentChar++;
    } while (*currentChar != '\n'); // 前两个数字以逗号结尾
    this->mDays = currentNum;
    currentNum = 0, currentChar++; //跳过换行
    //cout << days << endl;

    for (int i = 0; i < this->mDays; ++i) {
        int requestNum;  //当天请求数
        do {
            currentNum = 10 * currentNum + *currentChar - '0'; // 每次取出数字第一位
            currentChar++;
        } while (*currentChar != '\n'); // 前两个数字以逗号结尾
        requestNum = currentNum;
        currentNum = 0, currentChar++; //跳过换行
        //cout << requestNum << endl;

        vector<Request> vec;
        vec.erase(vec.begin(),vec.end());   //先清空容器

        for(int j=0;j<requestNum;j++){
            curStr = "";
            // 读取 requestType
            do {
                if(*currentChar != '(') curStr += *currentChar; // 每次取出数字第一位
                currentChar++;
            } while (*currentChar != ','); // 前两个数字以逗号结尾
            requestType = curStr;
            currentNum = 0, currentChar+=2;

            if(requestType == "add"){
                curStr = "";
                // 读取 VMTypeName
                do {
                    curStr += *currentChar; // 每次取出数字第一位
                    currentChar++;
                } while (*currentChar != ','); // 前两个数字以逗号结尾
                VMTypeName = curStr;
                currentNum = 0, currentChar+=2;

                this->vmToVMType[vmId] = VMTypeName;
            }else{
                VMTypeName = "";
            }

            // 读取服务器 serverId
            do {
                currentNum = 10 * currentNum + *currentChar - '0';
                currentChar++;
            } while (*currentChar != ')');
            vmId = currentNum;
            currentNum = 0, currentChar+=2;

            Request req = {requestType,VMTypeName,vmId};
            vec.push_back(req);
            //cout << requestType << " " << id <<endl;
        }
        this->mRequest.push_back(vec);
    }
    //cout << "input over" << endl;
}

void Solution::run() {
    this->input();
    this->judge();
    //this->printTest();
}

void Solution::write_answer() {

}


void Solution::printTest(){

    for(auto it:this->mServerTypeMap) this->mHelper.printServerType(it.second);
//    cout << this->mServerTypeMap.size() << endl;
//    for(auto it:this->mServer)  this->mHelper.printServerType(this->mServerTypeMap[it.serverTypeName]);
//    cout << this->mMVTypeMap.size() << endl;
//    for(auto it:this->mVM) this->mHelper.printVMType(this->mMVTypeMap[it.vmTypeName]);
//    cout << this->mDays << endl;
//    for (int i = 0; i < this->mDays; ++i) {
//        cout << this->mRequest[i].size() << endl;
//        for (int j = 0; j < this->mRequest[i].size(); ++j) {
//            this->mHelper.printRequest(this->mRequest[i][j]);
//        }
//    }
}

void Solution::judge(){
    int total=0;
    for (int i = 0; i < this->mDays; ++i) {
        total+=this->mRequest[i].size();
    }
    //cout << total << endl;
    int num =  total/179957.0*12000;  //12000     179957
    //cout << num << endl;
    int aveNum = num/this->mServerTypeNum;
    //long long int sum=0;
    int curId=0;    //当前服务器id
    for (int i = 0; i < this->mDays; ++i) {

        if(i == 0){
            cout << "(purchase, " << this->mServerTypeNum << ")"<< endl;
            for (int j = 0; j < this->mServerTypeNum; ++j) {    //先全部买
                string name = this->mServerTypeByCpu[j].serverTypeName;
                cout << "(" << name << ", "<<aveNum<<")"<< endl;
                pair<int,int> t={this->mServerTypeByCpu[j].cpus/2,this->mServerTypeByCpu[j].memory/2};
                for(int k=0;k<aveNum;k++){
                    this->mServer[this->mServerId] = {name,this->mServerId,t,t};      //加入购买的服务器
                    this->mServerId++;
                }
                //sum += aveNum*this->mServerTypeByCpu[j].hardCost;
            }
        }else{
            cout << "(purchase,  0)"<< endl;
        }

        cout << "(migration, 0)"<< endl;

        /**
         * 当前服务器不满足，购买新的服务器
         * 最大CPU，内存
         */
        this->sizeFlag = 0; //每次进入之前，将标记置为0
        deploy(i,0,curId);




//        //1.购买
//        this->purchase();
//        //2.迁移
//        this->migrate();
//        //3.部署
//        this->deploy();
    }
    //cout<< sum << endl;
}

void Solution::purchase(){

}
void Solution::migrate(){

}
void Solution::deploy(int i,int k,int &curId){
    int j;
    for (j = k; j < this->mRequest[i].size() && curId < this->mServerId; ++j) {
        Request curReq = this->mRequest[i][j];
        if(curReq.requestType == "add" ){     //(add, 虚拟机类型，虚拟机id)
            VMType vmType = this->mMVTypeMap[curReq.vmTypeName];
            Server curServer = this->mServer[curId];
            int ACpus = curServer.A.first/2;
            int AMemory = curServer.A.second/2;
            int BCpus = curServer.B.first/2;
            int BMemory = curServer.B.second/2;
            int curCpus = vmType.cpus/2;
            int curMemory = vmType.memory/2;
            if(vmType.isDouble){    //双结点
                if(ACpus >= curCpus && AMemory >= curMemory && BCpus >= curCpus && BMemory >= curMemory ){
                    this->mServer[curId].A.first -= curCpus;
                    this->mServer[curId].A.second -= curMemory;
                    this->mServer[curId].B.first -= curCpus;
                    this->mServer[curId].B.second -= curMemory;
                    this->vmToServer[curReq.vmId]= make_pair(curId,0);    //虚拟机id映射到<服务器id,结点(0双结点，1:A结点,2:B结点)>
                    cout << "("<< curId << ")"<< endl;
                }else{
                    if(this->sizeFlag > this->mServerId){
                        //cout << "超标";
                        return;
                    }
                    this->sizeFlag ++;
                    curId++;    //不符合，下一个服务器
                    j--;        //保持当前请求
                }
            } else{ //单结点
                if(ACpus >= curCpus && AMemory >= curMemory){
                    this->mServer[curId].A.first -= curCpus;
                    this->mServer[curId].A.second -= curMemory;
                    this->vmToServer[curReq.vmId]= make_pair(curId,1);
                    cout << "("<< curId << ", A)"<< endl;
                }
                else if(BCpus >= curCpus && BMemory >= curMemory ){
                    this->mServer[curId].B.first -= curCpus;
                    this->mServer[curId].B.second -= curMemory;
                    this->vmToServer[curReq.vmId]= make_pair(curId,2);
                    cout << "("<< curId << ", B)"<< endl;
                }else{
                    if(this->sizeFlag > this->mServerId){
                        //cout << "超标";
                        return;
                    }
                    this->sizeFlag ++;
                    curId++;
                    j--;
                }
            }
        }else{  //(del, 虚拟机id)
            VMType vmTypeTmp = this->mMVTypeMap[this->vmToVMType[curReq.vmId]];
            int curCpus = vmTypeTmp.cpus/2;
            int curMemory = vmTypeTmp.memory/2;
            int targetServerId = this->vmToServer[curReq.vmId].first;
            if(vmTypeTmp.isDouble){//是否为双结点
                this->mServer[targetServerId].A.first += curCpus;    //先按vmid找服务器id，再找到对应服务器
                this->mServer[targetServerId].A.second += curMemory;
                this->mServer[targetServerId].B.first += curCpus;
                this->mServer[targetServerId].B.second += curMemory;
            }else{
                if(this->vmToServer[curReq.vmId].second == 1){  //A结点
                    this->mServer[targetServerId].A.first += curCpus;    //先按vmid找服务器id，再找到对应服务器
                    this->mServer[targetServerId].A.second += curMemory;
                }else if(this->vmToServer[curReq.vmId].second == 2) { //B结点
                    this->mServer[targetServerId].B.first += curCpus;
                    this->mServer[targetServerId].B.second += curMemory;
                }
            }
        }
    }

    if(curId == this->mServerId) {   //遍历完
        curId = 0;
        if(j < this->mRequest[i].size())    //如果还有请求未处理完，从j继续处理
            deploy(i,j,curId);          //从j开始，服务器从0开始
    }
}
