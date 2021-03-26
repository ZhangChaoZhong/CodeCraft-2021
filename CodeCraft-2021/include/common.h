//
// Created by zcz on 2021/3/10.
//

#ifndef CODECRAFT_2021_COMMON_H
#define CODECRAFT_2021_COMMON_H

#include <iostream>
//采购的服务器类型数量 不超过100
//售卖的服务器数量 不超过1000

/*
 * 服务器类型 以及 服务器
 *
 */
struct ServerType{
    std::string serverTypeName;  //服务器类型[,20]
    int cpus;             //CPU核数  [1,1024]
    int memory;           //内存大小  [1,1024]
    int hardCost;         //硬件成本  [1,5x10^5]
    int energyCost;       //能耗成本  [,5000]
    int percent;       //CPU与内存比例
    double cm;  // CPU/内存
    int pm; // 价格/内存
    int pc; // 价格/cpu
    int p_cm;   //价格/(CPU+内存)
    int totalCost;  //生命周期总能耗量
};

struct Server{
    std::string serverTypeName; //服务器类型
    int id;               //服务器id  int范围 [0,2^32]
    std::pair<int,int> A;
    std::pair<int,int> B;
};

/*
 * 虚拟机序列
 *
 */
struct VMType{
    std::string vmTypeName;  //虚拟机类型[,20]
    int cpus;             //CPU核数  [1,1024]
    int memory;           //内存大小  [1,1024]
    int isDouble;         //是否双结点部署 0不是  1是
};
struct VM{
    std::string  vmTypeName;        //虚拟机类型
    int vmId;               //虚拟机id  int范围 [0,2^32]
};

/*
 * 请求序列
 * 用户请求不超过10^5
 */
struct Request{
    std::string requestType;   //请求类型，add 或者 del
    std::string vmTypeName;   //虚拟机类型[,20]
    int vmId;              //服务器id  int范围 [0,2^32]
    int index;      // 输出下标从0开始
    bool flag;      /// 当天部署，当天删除的，为true，否则为false;
};

/***************************************************/

/*
 * 购买序列
 *
 */
struct Purchaser{
    std::string serverType;   //服务器类型[,20]
    int num;               //数量
};

/*
 * 迁移序列
 *
 */
struct Migration{
    int id;                //服务器id  int范围 [0,2^32]
    int targetId;          //目标服务器id
    char node;         //目标服务器的结点 A或者B
};

/*
 * 部署序列
 *
 */
struct Deployment{
    int id;                //服务器id  int范围 [0,2^32]
    char node;             //部署的结点 A或者B
};


#endif //CODECRAFT_2021_COMMON_H
