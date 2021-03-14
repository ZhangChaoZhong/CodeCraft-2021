//
// Created by zcz on 2021/3/11.
//

#ifndef CODECRAFT_2021_SOLUTION_H
#define CODECRAFT_2021_SOLUTION_H

#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <unordered_map>
#include "common.h"
#include "Helper.h"

class Solution {
public:
    Solution(std::string &filename);
    Solution();
    void run();

    void input();

    void write_answer();

    void printTest();

    void judge();

//    void purchase();
//
//    void migrate();

    void deploy(int i,int k);

    void inputServer(int index,std::string &serverTypeName,std::string &cpus,std::string &memory,std::string &hardCost,std::string &energyCost);
    void inputVm(int index,std::string &vmType,std::string &vmCpuCores,std::string &vmMemory,std::string &vmTwoNodes);
    void inputRequest(std::vector<Request> &vec,std::string &requestType,std::string &reqVmType,std::string &reqId);
    void inputRequest(std::vector<Request> &vec,std::string &requestType,std::string &reqId);
    void getMemoryPerDay(int i,std::vector<Request> &vec);
    static bool serverTypeCmpCpu(const ServerType &a,const ServerType &b){  //cpu从大到小
        return a.cpus > b.cpus;     //不能写>=
    };
    static bool serverTypeCmpMemory(const ServerType &a,const ServerType &b){  //内存从大到小
        return a.memory > b.memory;
    };
    static bool vmTypeCmpCpu(const VMType &a,const VMType &b) { //cpu从大到小
        return a.cpus > b.cpus;
    };
    static bool vmTypeCmpMemory(const VMType &a,const VMType &b){   //内存从大到小
        return a.memory > b.memory;
    };
    static bool serverTypeCmpPercent(const ServerType &a,const ServerType &b){  //比例从小到大
        return a.percent < b.percent;
    }

public:

    enum { SERVER_TYPE_NUM = 100, VM_TYPE_NUM = 1000}; // 枚举常量
    Helper mHelper;
    ServerType mServerTypeByCpu[SERVER_TYPE_NUM];
    ServerType mServerTypeByPercent[SERVER_TYPE_NUM];
    Server mServer[100000];         //服务器 最多
    std::unordered_map<std::string, ServerType> mServerTypeMap;  //服务器类型(类型名，类型结构体) 按 价格 > cpu>内存>能耗
    int mServerId = 0;              //服务器 全局id

    std::vector<VM> mVM;         //虚拟机
    VMType mVMTypeByCpu[VM_TYPE_NUM];         //不能用vector存，太大排不了序
    VMType mVMTypeByMemory[VM_TYPE_NUM];
    std::unordered_map<std::string, VMType> mMVTypeMap;  //<虚拟机类型名，类型结构体>
    int mVMId = 0;              //虚拟机全局id

    std::vector<std::vector<Request>> mRequest; //请求序列
    int mDays;  //天数
    std::string mFilename;
    std::unordered_map<int,std::pair<int,int>> vmToServer;         //虚拟机id映射到<服务器id,结点(0双结点，1:A结点,2:B结点)>
    std::unordered_map<int,std::string> vmToVMType; //<虚拟机id映射到虚拟机类型>
    int mServerTypeNum;
    int mVMTypeNum;
    int sizeFlag;   //遍历过的服务器个数

    std::set<int> mPercentTypeSet;      //自动排序，从小到大
    std::vector<std::pair<std::string,int>> serverSelected;    //选取的 <服务器类型，CPU/内存比例>
    std::unordered_map<std::string,int> serverSelectedIndex;    //<服务器类型,下标>
    std::unordered_map<int,std::string> indexSelectedServer;    //<下标,服务器类型>
    std::unordered_map<std::string,std::string> vmTypeToServerType;       //<虚拟机类型名映射到服务器类型名>
    std::vector<std::vector<long long>> mMemoryPerDay;        //每天的对应的服务器类型的内存实际的需求量
    std::vector<std::vector<long long>> mMemoryPerDayDel;        //每天的对应的服务器类型的删除内存量
    std::vector<int> mNumPerServerType; //每种类型的服务器的需求数量(多买10%)
    int mNumServerTypeByPercent;//按比值划分的服务器类型数量
    std::vector<int> mMaxIndex;//每种服务器类型的最大下标  <这个最大下标
    std::vector<int> mMinIndex;//每种服务器类型的最小下标 >=这个最小下标
    std::vector<int> mCurIndex; //每种服务器类型的当前的下标
};


#endif //CODECRAFT_2021_SOLUTION_H
