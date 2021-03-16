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
    Solution();
    Solution(std::string &filename);

    /** 主程序 */
    void run();
    /** 输入处理 */
    void input();
    /** 决策（购买，迁移，部署） */
    void judge();
    /** 部署 */
    void deploy(int i,int k,int &curId);

    /** 解析服务器类型 */
    void inputServer(int index,std::string &serverTypeName,std::string &cpus,std::string &memory,std::string &hardCost,std::string &energyCost);
    /** 解析虚拟机类型 */
    void inputVm(int index,std::string &vmType,std::string &vmCpuCores,std::string &vmMemory,std::string &vmTwoNodes);
    /** 解析添加请求 */
    void inputRequest(std::vector<Request> &vec,std::string &requestType,std::string &reqVmType,std::string &reqId);
    /** 解析删除请求 */
    void inputRequest(std::vector<Request> &vec,std::string &requestType,std::string &reqId);

    /** 统计每一天请求的虚拟机的内存需求量，删除量 */
    void getMemoryPerDay(int i,std::vector<Request> &vec);

    /** 按(CPU/内存)比例从小到大排序 */
    static bool serverTypeCmpPercent(const ServerType &a,const ServerType &b){
        return a.percent < b.percent;
    }

    static bool serverTypeCmpEnergyCost(const ServerType &a,const ServerType &b){           //按能耗从小到大排序
        return a.energyCost < b.energyCost;
    }
    static bool serverTypeCmpPM(const ServerType &a,const ServerType &b){           //按价格/内存 从小到大排序
        return a.pm < b.pm;
    }
//    static bool serverTypeCmpCpu(const ServerType &a,const ServerType &b){                    //cpu从大到小
//        return a.cpus > b.cpus;     //不能写>=
//    };


public:
    std::string mFilename;          //输入的训练文件名
    enum { SERVER_TYPE_NUM = 100, VM_TYPE_NUM = 1000,SERVER_NUM=100010}; // 枚举常量
    Helper mHelper;                                                      //打印帮助类

    /// 服务器
    int mServerId = 0;                                                   //服务器全局id
    int mServerTypeNum;                                                  // 实际的服务类型数量
    ServerType mServerType[SERVER_TYPE_NUM];                             //服务器类型 数组
    ServerType mServerTypeByPercent[SERVER_TYPE_NUM];                    //按比例从小到大排序的服务器类型 数组
    Server mServer[SERVER_NUM];                                          //服务器 数组
    std::unordered_map<std::string, ServerType> mServerTypeMap;          //<服务器类型名，服务器类型结构体>

    /// 虚拟机
    int mVMTypeNum;                                                      //实际的虚拟机类型数量
    VMType mVMType[VM_TYPE_NUM];                                         //虚拟机类型 数组
    std::unordered_map<std::string, VMType> mMVTypeMap;                  //<虚拟机类型名，虚拟机类型结构体>
    std::unordered_map<int,std::pair<int,int>> vmToServer;               //虚拟机id映射到<服务器id,结点(0双结点，1:A结点,2:B结点)>
    std::unordered_map<int,std::string> vmToVMType;                      //<虚拟机id映射到虚拟机类型名>
    std::unordered_map<std::string,std::string> vmTypeToServerType;      //<虚拟机类型名映射到服务器类型名>

    /// 请求
    int mDays;                                                           //请求天数
    std::vector<std::vector<Request>> mRequest;                          //请求序列

    /// 划分，选取服务器类型
    int mNumServerTypeByPercent;                                         //按比值划分的服务器类型数量
    std::vector<std::pair<std::string,int>> serverSelected;              //选取的服务器类型 <服务器类型，CPU/内存比例>
    std::unordered_map<std::string,int> serverSelectedIndex;             //<服务器类型,serverSelected下标>
    std::unordered_map<int,std::string> indexSelectedServer;             //<serverSelected下标,服务器类型>

    /// 计算最大内存，以及相应的服务器需求量
    std::vector<long long> mMemoryPerDay;                   //存储每天的对应的服务器类型的内存实际的需求量
    std::vector<long long> mMemoryPerDayDel;                //存储每天的对应的服务器类型的删除内存量
    std::vector<long long> mCpuPerDay;                //存储每天的对应的服务器类型的删除内存量
    std::vector<long long> mCpuPerDayDel;                //存储每天的对应的服务器类型的删除内存量
    std::vector<int> mNumPerServerType;                                  //每种类型的服务器的需求数量(多买10%)

    /// 按服务器类型遍历服务器；放不下，才放下一个；每次都从第一个开始放
    int mMax;   //最大数量
    int mSizeFlag;
    std::string mTestName;      //测试的服务器类型名

    std::vector<ServerType> mSelectServerType;   //选取的服务器类型
};


#endif //CODECRAFT_2021_SOLUTION_H
