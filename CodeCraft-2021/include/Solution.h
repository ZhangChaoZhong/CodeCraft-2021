//
// Created by zcz on 2021/3/11.
//

#ifndef CODECRAFT_2021_SOLUTION_H
#define CODECRAFT_2021_SOLUTION_H

#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>
#include "common.h"
#include "Helper.h"

class Solution {
public:
    Solution(std::string &filename);

    void run();

    void input();

    void write_answer();

    void printTest();

    void judge();

    void purchase();

    void migrate();

    void deploy(int i,int k,int &curId);

//    static bool serverTypeCmp(ServerType &a,ServerType& b){ //从小到大
//        if(a.hardCost == b.hardCost){
//            if(a.cpus == b.cpus){
//                if(a.memory == b.memory){
//                    return a.energyCost < b.energyCost; //能耗 小
//                } else{
//                    return a.memory < b.memory; //内存 小
//                }
//            }else {
//                return a.cpus < b.cpus;     //cpu 小
//            }
//        }else {
//            return a.hardCost < b.hardCost;     //硬件成本小的
//        }
//    };
    static bool serverTypeCmpCpu(const ServerType &a,const ServerType &b){
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

public:

    enum { SERVER_TYPE_NUM = 100, VM_TYPE_NUM = 1000}; // 枚举常量
    Helper mHelper;
    ServerType mServerTypeByCpu[SERVER_TYPE_NUM];
    ServerType mServerTypeByMemory[SERVER_TYPE_NUM];
    Server mServer[100000];         //服务器 最多
    std::unordered_map<std::string, ServerType> mServerTypeMap;  //服务器类型(类型名，类型结构体) 按 价格 > cpu>内存>能耗
    int mServerId = 0;              //服务器 全局id

    std::vector<VM> mVM;         //虚拟机
    VMType mVMTypeByCpu[VM_TYPE_NUM];         //不能用vector存，太大排不了序
    VMType mVMTypeByMemory[VM_TYPE_NUM];
    std::unordered_map<std::string, VMType> mMVTypeMap;  //虚拟机类型(类型名，类型结构体)
    int mVMId = 0;              //虚拟机全局id

    //std::vector<std::vector<int>> reqCpu;           //统计每天请求中最大的CPU
    //std::vector<std::vector<int>> reqMemory;        //统计每天请求中最大的CPU
    std::vector<std::vector<Request>> mRequest; //请求序列
    int mDays;  //天数
    std::string mFilename;
    std::unordered_map<int,std::pair<int,int>> vmToServer;         //虚拟机id映射到<服务器id,结点(0双结点，1:A结点,2:B结点)>
    std::unordered_map<int,std::string> vmToVMType; //虚拟机id映射到虚拟机类型
    int mServerTypeNum;
    int sizeFlag;   //遍历过的服务器个数
};


#endif //CODECRAFT_2021_SOLUTION_H
