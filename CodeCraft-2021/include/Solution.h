//
// Created by zcz on 2021/3/11.
//

#ifndef CODECRAFT_2021_SOLUTION_H
#define CODECRAFT_2021_SOLUTION_H

#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
#include <map>
#include <thread>
#include "common.h"
#include "Helper.h"
#define NUM_THREADS 4
#define WASTER_LOW 1
#define WASTER_HIGH 15

class Solution {
public:
    Solution();
    Solution(std::string &filename);

    /** 主程序 */
    void run();
    /** 输入处理 */
    void input();
    /** 输出处理 */
    void myOutput();
    /** 决策（购买，迁移，部署） */
    void judge();
    /** 迁移 */
    void migration(int i);
    void migration2(int i);
    /** 部署 */
    void deploy(int i,int k);

    /** 解析服务器类型 */
    void inputServer(int index,std::string &serverTypeName,std::string &cpus,std::string &memory,std::string &hardCost,std::string &energyCost);
    /** 解析虚拟机类型 */
    void inputVm(int index,std::string &vmType,std::string &vmCpuCores,std::string &vmMemory,std::string &vmTwoNodes);
    /** 解析添加请求 */
    void inputRequest(std::vector<Request> &vec,std::string &requestType,std::string &reqVmType,std::string &reqId);
    /** 解析删除请求 */
    static void inputRequest(std::vector<Request> &vecDel,std::vector<Request> &vec,std::string &requestType,std::string &reqId);

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
    static bool serverCmpId(const Server &a,const Server &b){                    //id从大到小
        return a.id > b.id;     //不能写>=
    };
    static bool serverCmpIdDel(const Server &a,const Server &b){                    //id从小到大
        return a.id < b.id;     //不能写>=
    };

    static bool serverCmpCM(const Server &a,const Server &b){                    //按CPU+内存的剩余总量 从小到大排序
        return (a.A.first+a.A.second+a.B.first+a.B.second) < (b.A.first+b.A.second+b.B.first+b.B.second);     //不能写>=
    };
    static bool serverCmpExtraMax(const Server &a,const Server &b){                    //按剩余的cpu+内存的值，从大到小排序
        return (a.A.first+a.A.second+a.B.first+a.B.second) > (b.A.first+b.A.second+b.B.first+b.B.second);
    };
    static bool serverCmpExtraMin(const Server &a,const Server &b){                    //按剩余的cpu+内存的值，从小到大排序
        return (a.A.first+a.A.second+a.B.first+a.B.second) < (b.A.first+b.A.second+b.B.first+b.B.second);
    };
    static void getDoubleIndex(int tid,int numSize,int cpu,int memory,std::vector<int> &res,std::vector<Server> &s,const int* arr); //获取下一个服务器的下标
    static void getSingleIndex(int tid,int numSize,int cpu,int memory,std::vector<int> &res,std::vector<Server> &s,std::vector<int> &node,const int* arr); //获取下一个服务器的下标
    static void getDoubleIndex2(int tid,int numSize,int cpu,int memory,std::vector<int> &res,std::vector<Server> &s,const int* arr);
    static void getSingleIndex2(int tid,int numSize,int cpu,int memory,std::vector<int> &res,std::vector<Server> &s,std::vector<int> &node,const int* arr);
    static void  getDoubleIndexM(int tid,int numSize,int cpu,int memory,std::vector<int> &res,std::vector<Server> &s,int serverId,const int* arr);
    static void  getSingleIndexM(int tid,int numSize,int cpu,int memory,std::vector<int> &res,std::vector<Server> &s,std::vector<int> &node,int serverId,const int* arr);

public:
    std::string mFilename;          //输入的训练文件名
    enum { SERVER_TYPE_NUM = 100, VM_TYPE_NUM = 1000,SERVER_NUM=100010}; // 枚举常量
    Helper mHelper;                                                      //打印帮助类

    /// 服务器
    int mServerId = 0;                                                   //服务器全局id
    int mServerTypeNum;                                                  // 实际的服务类型数量
    ServerType mServerType[SERVER_TYPE_NUM];                             //服务器类型 数组
    ServerType mServerTypeByPercent[SERVER_TYPE_NUM];                    //按比例从小到大排序的服务器类型 数组
    std::map<std::string, ServerType> mServerTypeMap;          //<服务器类型名，服务器类型结构体>

    /// 虚拟机
    int mVMTypeNum;                                                      //实际的虚拟机类型数量
    VMType mVMType[VM_TYPE_NUM];                                         //虚拟机类型 数组
    std::map<std::string, VMType> mMVTypeMap;                  //<虚拟机类型名，虚拟机类型结构体>
    std::map<int,std::pair<int,int>> vmToServer;               //虚拟机id映射到<服务器id,结点(0双结点，1:A结点,2:B结点)>
    std::map<int,std::string> vmToVMType;                      //<虚拟机id映射到虚拟机类型名>
    std::map<std::string,std::string> vmTypeToServerType;      //<虚拟机类型名映射到服务器类型名>


    /// 请求
    int mDays;                                                           //请求天数
    std::vector<std::vector<Request>> mRequest;                          //请求序列
    std::vector<std::vector<Request>> mRequestDel;                       //删除序列

    /// 划分，选取服务器类型
    int mNumServerTypeByPercent;                                         //按比值划分的服务器类型数量
    std::vector<std::pair<std::string,int>> serverSelected;              //选取的服务器类型 <服务器类型，CPU/内存比例>
    std::map<std::string,int> serverSelectedIndex;             //<服务器类型,serverSelected下标>
    std::map<int,std::string> indexSelectedServer;             //<serverSelected下标,服务器类型>

    /// 计算最大内存，以及相应的服务器需求量
    std::vector<long long> mMemoryPerDay;                   //存储每天的对应的服务器类型的内存实际的需求量
    std::vector<long long> mMemoryPerDayDel;                //存储每天的对应的服务器类型的删除内存量
    std::vector<long long> mCpuPerDay;                //存储每天的对应的服务器类型的删除内存量
    std::vector<long long> mCpuPerDayDel;                //存储每天的对应的服务器类型的删除内存量
    std::vector<int> mNumPerServerType;                                  //每种类型的服务器的需求数量(多买10%)

    /// 按服务器类型遍历服务器；放不下，才放下一个；每次都从第一个开始放
    int mMax;   //最大数量
    std::string mTestName;      //测试的服务器类型名
    ServerType mSelectedServerType;     //选取的服务器

    std::vector<ServerType> mSelectServerType;   //选取的服务器类型

    int isEmpty[10000];      // 0为空  1不为空
    std::vector<Server> mHasVm; //已经有部署虚拟机的服务器
    std::vector<Server> mNoHasVm; //没有部署虚拟机的服务器

    std::vector<Server> mHasVmDel; //部署当天删除的虚拟机的服务器 用来迁移

    std::thread  mThread[NUM_THREADS];      //创建4个线程

    std::vector<std::string> mDeployMsg;    /// 保存部署输出的信息
    int mMaxServerId;                   /// 已部署过的最大的服务器id

    int mVmIndex;       /// 虚拟机初始化的下标
    std::vector<int> mCurSaveVM;        /// 当天之前的存在虚拟机数量
    int mMigriationNum;
    std::map<int,std::vector<int>> mServerIdToVMSeq;      /// 服务器id映射到虚拟机id序列
};


#endif //CODECRAFT_2021_SOLUTION_H
