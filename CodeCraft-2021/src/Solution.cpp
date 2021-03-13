//
// Created by zcz on 2021/3/11.
//

#include "Solution.h"

#include <algorithm>
#include <string>
#include <iostream>
#include <math.h>
//#include <sys/mman.h>
//#include <unistd.h>
//#include <fcntl.h>
//#include <cstdio>
using namespace std;

Solution::Solution(string &filename){
    this->mFilename = filename;
    this->mDays=0;
    this->sizeFlag = 0;
    this->mServerTypeNum = 0;
    this->mVMTypeNum=0;
}
Solution::Solution(){
    this->mDays=0;
    this->sizeFlag = 0;
    this->mServerTypeNum = 0;
    this->mVMTypeNum=0;
}

void Solution::input() {

    //服务器类型,请求类型
    string serverTypeName,VMTypeName,requestType;
    //cpu核数,内存,硬件成本，能耗成本，是否双结点部署
    string cpus, memory, hardCost, energyCost,isDouble,vmId;

    //test 输入
    FILE *fp = freopen(this->mFilename.c_str(),"r",stdin);

    /**
     * 购买服务器序列 每种类型买100
     *
     * */
    cin >> this->mServerTypeNum;  //服务器类型数量
    //cout << this->mServerTypeNum << endl;
    for(int i=0;i<this->mServerTypeNum;i++){
        cin >> serverTypeName >> cpus >> memory >> hardCost >> energyCost;
        generateServer(i,serverTypeName,cpus,memory,hardCost,energyCost);
    }

    sort(this->mServerTypeByCpu,this->mServerTypeByCpu+this->mServerTypeNum,serverTypeCmpCpu);           //按cpu排序
    sort(this->mServerTypeByMemory,this->mServerTypeByMemory+this->mServerTypeNum,serverTypeCmpMemory);  //按内存排序

    cout<<"服务器\n";
    for(int i=0;i<this->mServerTypeNum;i++) {               //加入哈希表
        //this->mHelper.printServerType(this->mServerTypeByMemory[i]);
        this->mServerTypeByPercent[i].percent = int(this->mServerTypeByPercent[i].cpus*1.0/this->mServerTypeByPercent[i].memory);
        this->mPercentTypeSet.insert(this->mServerTypeByPercent[i].percent);
    }
    sort(this->mServerTypeByPercent,this->mServerTypeByPercent+this->mServerTypeNum,serverTypeCmpPercent);
    //for(auto it:this->mServerTypeByPercent) this->mHelper.printServerTypeBypercent(it);

    /*划分，选取服务器类型*/
    vector<ServerType> _serverType;
    set<int>::iterator iter = this->mPercentTypeSet.begin();
    for(int i=0;i<this->mServerTypeNum;i++){    //遍历服务器类型
        ServerType tmp =this->mServerTypeByPercent[i];
        if( *iter == tmp.percent){             //比例相同加入
            _serverType.push_back(tmp);
        }else{
            int tMemory=0,tCpus=0,index=0,tPercent=0;
            for(int j=0;j<_serverType.size();j++){
                if(_serverType[j].memory > tMemory){    //保存内存大的
                    index = j;
                    tMemory = _serverType[j].memory;
                    tPercent = _serverType[j].percent;
                }else if(_serverType[j].memory == tMemory) { //内存相等，保存cpu大的
                    if (_serverType[j].cpus > tCpus){
                        index = j;
                        tCpus = _serverType[j].cpus;
                        tPercent = _serverType[j].percent;
                    }
                }
            }
            this->serverSelected.emplace_back(make_pair(_serverType[index].serverTypeName,tPercent));  //选取的服务器类型
            _serverType.erase(_serverType.begin(),_serverType.end());   //删除之前的
            _serverType.push_back(tmp);
            iter++; //遍历下个比例
        }
        if(i == this->mServerTypeNum - 1){  //处理最后一种
            int tMemory=0,tCpus=0,index=0,tPercent=0;
            for(int j=0;j<_serverType.size();j++){
                if(_serverType[j].memory > tMemory){    //保存内存大的
                    index = j;
                    tCpus = _serverType[j].cpus;
                    tMemory = _serverType[j].memory;
                    tPercent = _serverType[j].percent;
                }else if(_serverType[j].memory == tMemory) { //内存相等，保存cpu大的
                    if (_serverType[j].cpus > tCpus){
                        index = j;
                        tCpus = _serverType[j].cpus;
                        tMemory = _serverType[j].memory;
                        tPercent = _serverType[j].percent;
                    }
                }
            }
            this->serverSelected.emplace_back(make_pair(_serverType[index].serverTypeName,tPercent));  //选取的服务器类型
        }
    }
    for(auto it:this->serverSelected) cout<<it.first << " " << it.second << endl;

    /**
     * 虚拟机售卖序列
     *
     * */
    //虚拟机类型按比值分类(CPU符不符合)+累加
    int vmSelectTypeNum = this->serverSelected.size();  //服务器类型分类的数目


    cin >> this->mVMTypeNum;   //售卖数量
    for(int i=0;i<this->mVMTypeNum;i++){
        cin >> VMTypeName >> cpus >> memory >> isDouble;
        generateVm(i,VMTypeName,cpus,memory,isDouble);
        vector<double> vec;     //虚拟机与所有服务器比值的距离保存下来
        vec.erase(vec.begin(),vec.end());
        for(int j=0;j<vmSelectTypeNum;j++){
            vec.emplace_back(fabs(this->mVMTypeByCpu[i].cpus*1.0/this->mVMTypeByCpu[i].memory -  this->serverSelected[j].second*1.0));
        }
        int minDistanceIndex = min_element(vec.begin(),vec.end()) - vec.begin();
        //cout << minDistanceIndex << endl;
        ServerType tmp = this->mServerTypeMap[this->serverSelected[minDistanceIndex].first];
        if(tmp.memory >= this->mVMTypeByCpu[i].memory && tmp.cpus >= this->mVMTypeByCpu[i].cpus){
            this->vmTypeToServerType[this->mVMTypeByCpu[i].vmTypeName] = this->serverSelected[minDistanceIndex].first;    //虚拟机类型映射到服务器类型
        }else{
            cout <<"最大的不能分配" <<endl;
        }
    }
    sort(this->mVMTypeByCpu,this->mVMTypeByCpu+this->mVMTypeNum,vmTypeCmpCpu);           //按cpu排序
    sort(this->mVMTypeByMemory,this->mVMTypeByMemory+this->mVMTypeNum,vmTypeCmpMemory);  //按内存排序

    //for(auto it:this->vmTyepToServerType) cout << it.first <<" " << it.second <<endl;

    cout<<"虚拟机\n";
    cout<< this->mVMTypeNum << endl;
    for(int i=0;i<this->mVMTypeNum;i++)
        this->mHelper.printVMType(this->mVMTypeByCpu[i]);

    /**
     * 请求序列
     *
     */
    vector<vector<long long>> memoryPerDay;        //每天的对应的服务器类型的内存实际的需求量

    cin >> this->mDays;  //请求天数
    //cout << this->mDays << endl;

    for (int i = 0; i < this->mDays; ++i) {
        int requestNum=0;  //当天请求数
        cin >> requestNum;
        //cout << requestNum << endl;
        vector<Request> vec;
        vec.erase(vec.begin(),vec.end());   //先清空容器

        for(int j=0;j<requestNum;j++){
            cin >> requestType;
            if(requestType[1] == 'a'){
                cin >> VMTypeName >> vmId;
                generateRequest(vec,requestType,VMTypeName,vmId);
                if(i == 0){ //第一天
                    totalMemory[minDistanceIndex] += this->mVMTypeByCpu[i].memory;  //将内存加入对应的服务器类型中，内存
                }else{  //第二天 = 第二天+ 第一天 - 第一天删除的

                }
            }else{
                cin >> vmId;
                generateRequest(vec,requestType,vmId);
            }
        }
        this->mRequest.push_back(vec);
    }
    fclose(fp);
}

void Solution::run() {
    this->input();
    this->judge();
    //this->printTest();

//    for(int i=0;i<100;i++){
//        this->mHelper.printServer(this->mServer[i]);
//    }
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

// 解析用户添加请求
void Solution::generateRequest(vector<Request> &vec,string &op,string &reqVmType,string &reqId){
    string _op,_reqVmType;
    int _reqId = 0;
    for(int i=0;i<reqId.size()-1;i++){
        _reqId = _reqId*10 + reqId[i] - '0';
    }
    _op = op.substr(1,op.size() -2);
    _reqVmType = reqVmType.substr(0,reqVmType.size() -1);
    Request req = {_op,_reqVmType,_reqId};
    vec.push_back(req);
    //cout << op <<"-" <<_op<<endl;
    //this->mHelper.printRequest(req);
}

// 解析用户删除请求
void Solution::generateRequest(vector<Request> &vec,string &op,string &reqId){
    string _op;
    int _reqId=0;
    for(int i=0;i<reqId.size()-1;i++){
        _reqId = _reqId*10 + reqId[i] - '0';
    }
    _op = op.substr(1,op.size() - 2);
    Request req = {_op,"",_reqId};
    vec.push_back(req);
    //cout << op <<"-" <<_op<<endl;
    //this->mHelper.printRequest(req);
}
void Solution::generateVm(int index,string &vmType,string &vmCpuCores,string &vmMemory,string &vmTwoNodes){
    string _vmType;
    for(int i=1;i<vmType.size() -1;i++){
        _vmType += vmType[i];
    }

    int _vmCpuCores = 0,_vmMemory=0,_vmTwoNodes=0;
    for(int i=0;i<vmCpuCores.size()-1;i++){
        _vmCpuCores = _vmCpuCores*10 + vmCpuCores[i] - '0';
    }
    for(int i=0;i<vmMemory.size()-1;i++){
        _vmMemory = _vmMemory*10 + vmMemory[i] - '0';
    }
    if(vmTwoNodes[0] == '1'){
        _vmTwoNodes = 1;
    }
    else{
        _vmTwoNodes =0;
    }
    VMType t={_vmType,_vmCpuCores,_vmMemory,_vmTwoNodes};
    this->mVMTypeByCpu[index] = t;
    this->mVMTypeByMemory[index] = t;
    this->mMVTypeMap[_vmType] = t;
}
void Solution::generateServer(int index,string &serverType,string &cpuCores,string &memorySize,string &serverCost,string &powerCost){
    string _serverType;
    for(int i =1;i<serverType.size() -1;i++){
        _serverType += serverType[i];
    }
    int _cpuCores =0,_memorySize=0,_serverCost=0,_powerCost=0;

    for(int i=0;i<cpuCores.size() -1;i++){
        _cpuCores = 10*_cpuCores + cpuCores[i] - '0';
    }
    for(int i=0;i<memorySize.size() -1;i++){
        _memorySize = 10*_memorySize + memorySize[i] - '0';
    }
    for(int i=0;i<serverCost.size() -1;i++){
        _serverCost = 10*_serverCost + serverCost[i] - '0';
    }
    for(int i=0;i<powerCost.size()-1;i++){
        _powerCost = 10*_powerCost + powerCost[i] - '0';
    }
    ServerType t = {_serverType,_cpuCores,_memorySize,_serverCost,_powerCost};
    this->mServerTypeByCpu[index]=t;
    this->mServerTypeByMemory[index]=t;
    this->mServerTypeByPercent[index]=t;
    this->mServerTypeMap[_serverType] = t;
}

void Solution::judge(){
    int total=0;
    for (int i = 0; i < this->mDays; ++i) {
        total+=this->mRequest[i].size();
    }
    //cout << total << endl;
    //int num =  total/179957.0*100000;  //12000     179957
    int num=75000;
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
            cout << "(purchase, 0)"<< endl;
        }

        cout << "(migration, 0)"<< endl;

        /**
         * 当前服务器不满足，购买新的服务器
         * 最大CPU，内存
         */
        this->sizeFlag = 0; //每次进入之前，将标记置为0
        deploy(i,0,curId);
    }
    //cout<< sum << endl;

}

//void Solution::purchase(){
//
//}
//void Solution::migrate(){
//
//}
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
            int curCpus = vmType.cpus;
            int curMemory = vmType.memory;
            if(vmType.isDouble){    //双结点
                if(ACpus >= curCpus/2 && AMemory >= curMemory/2 && BCpus >= curCpus/2 && BMemory >= curMemory/2 ){
                    this->mServer[curId].A.first -= curCpus/2;
                    this->mServer[curId].A.second -= curMemory/2;
                    this->mServer[curId].B.first -= curCpus/2;
                    this->mServer[curId].B.second -= curMemory/2;
                    this->vmToServer[curReq.vmId]= make_pair(curId,0);    //虚拟机id映射到<服务器id,结点(0双结点，1:A结点,2:B结点)>
                    cout << "("<< curId << ")"<< endl;
                    curId++;
                }else{
                    if(this->sizeFlag > this->mServerId){
                        cout << "超标\n";
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
                    curId++;
                }
                else if(BCpus >= curCpus && BMemory >= curMemory ){
                    this->mServer[curId].B.first -= curCpus;
                    this->mServer[curId].B.second -= curMemory;
                    this->vmToServer[curReq.vmId]= make_pair(curId,2);
                    cout << "("<< curId << ", B)"<< endl;
                    curId++;
                }else{
                    if(this->sizeFlag > this->mServerId){
                        cout << "超标\n";
                        return;
                    }
                    this->sizeFlag ++;
                    curId++;
                    j--;
                }
            }
        }else{  //(del, 虚拟机id)
            VMType vmTypeTmp = this->mMVTypeMap[this->vmToVMType[curReq.vmId]];
            int curCpus = vmTypeTmp.cpus;
            int curMemory = vmTypeTmp.memory;
            int targetServerId = this->vmToServer[curReq.vmId].first;
            if(vmTypeTmp.isDouble){//是否为双结点
                this->mServer[targetServerId].A.first += curCpus/2;    //先按vmid找服务器id，再找到对应服务器
                this->mServer[targetServerId].A.second += curMemory/2;
                this->mServer[targetServerId].B.first += curCpus/2;
                this->mServer[targetServerId].B.second += curMemory/2;
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
