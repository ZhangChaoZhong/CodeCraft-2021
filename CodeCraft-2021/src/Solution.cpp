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
    this->mServerTypeNum = 0;
    this->mVMTypeNum=0;
    this->mNumServerTypeByPercent=0;
}
Solution::Solution(){
    this->mDays=0;
    this->mServerTypeNum = 0;
    this->mVMTypeNum=0;
    this->mNumServerTypeByPercent=0;
}

void Solution::input() {

    //服务器类型,请求类型
    string serverTypeName, VMTypeName, requestType;
    //cpu核数,内存,硬件成本，能耗成本，是否双结点部署
    string cpus, memory, hardCost, energyCost, isDouble, vmId;

    //test 输入
    //FILE *fp = freopen(this->mFilename.c_str(), "r", stdin);

    /**
     * 购买服务器序列 每种类型买100
     *
     * */
    cin >> this->mServerTypeNum;  //服务器类型数量
    //cout << this->mServerTypeNum << endl;
    for (int i = 0; i < this->mServerTypeNum; i++) {
        cin >> serverTypeName >> cpus >> memory >> hardCost >> energyCost;
        inputServer(i, serverTypeName, cpus, memory, hardCost, energyCost);
    }

//    sort(this->mServerTypeByCpu,this->mServerTypeByCpu+this->mServerTypeNum,serverTypeCmpCpu);           //按cpu排序

    //cout<<"服务器\n";
    for (int i = 0; i < this->mServerTypeNum; i++) {               //加入哈希表
        //this->mHelper.printServerType(this->mServerTypeByCpu[i]);
        this->mServerTypeByPercent[i].percent = int(
                this->mServerTypeByPercent[i].cpus * 1.0 / this->mServerTypeByPercent[i].memory);
        this->mPercentTypeSet.insert(this->mServerTypeByPercent[i].percent);
    }
    sort(this->mServerTypeByPercent, this->mServerTypeByPercent + this->mServerTypeNum, serverTypeCmpPercent);
    //for(auto it:this->mServerTypeByPercent) this->mHelper.printServerTypeBypercent(it);

    /*划分，选取服务器类型*/
    vector<ServerType> _serverType;
    set<int>::iterator iter = this->mPercentTypeSet.begin();
    for (int i = 0; i < this->mServerTypeNum; i++) {    //遍历服务器类型
        ServerType tmp = this->mServerTypeByPercent[i];  // 从比值最小开始遍历
        if (*iter == tmp.percent) {             //比例相同加入
            _serverType.push_back(tmp);
        } else {
            int tMemory = 0, tCpus = 0, index = 0, tPercent = 0;
            for (int j = 0; j < _serverType.size(); j++) {
                if (_serverType[j].memory > tMemory) {    //保存内存大的
                    index = j;
                    tMemory = _serverType[j].memory;
                    tPercent = _serverType[j].percent;
                } else if (_serverType[j].memory == tMemory) { //内存相等，保存cpu大的
                    if (_serverType[j].cpus > tCpus) {
                        index = j;
                        tCpus = _serverType[j].cpus;
                        tPercent = _serverType[j].percent;
                    }
                }
            }
            this->serverSelected.emplace_back(make_pair(_serverType[index].serverTypeName, tPercent));  //选取的服务器类型
            this->serverSelectedIndex[_serverType[index].serverTypeName] =
                    this->serverSelected.size() - 1;       //服务器类型名映射到下标
            this->indexSelectedServer[this->serverSelected.size() -
                                      1] = _serverType[index].serverTypeName;   //下标映射到服务器类型名
            _serverType.erase(_serverType.begin(), _serverType.end());   //删除之前的
            _serverType.push_back(tmp);
            iter++; //遍历下个比例
        }
        if (i == this->mServerTypeNum - 1) {  //处理最后一种
            int tMemory = 0, tCpus = 0, index = 0, tPercent = 0;
            for (int j = 0; j < _serverType.size(); j++) {
                if (_serverType[j].memory > tMemory) {    //保存内存大的
                    index = j;
                    tCpus = _serverType[j].cpus;
                    tMemory = _serverType[j].memory;
                    tPercent = _serverType[j].percent;
                } else if (_serverType[j].memory == tMemory) { //内存相等，保存cpu大的
                    if (_serverType[j].cpus > tCpus) {
                        index = j;
                        tCpus = _serverType[j].cpus;
                        tMemory = _serverType[j].memory;
                        tPercent = _serverType[j].percent;
                    }
                }
            }
            this->serverSelected.emplace_back(make_pair(_serverType[index].serverTypeName, tPercent));  //选取的服务器类型
            this->serverSelectedIndex[_serverType[index].serverTypeName] = this->serverSelected.size() - 1;//服务器类型名映射到下标
            this->indexSelectedServer[this->serverSelected.size() -
                                      1] = _serverType[index].serverTypeName;   //下标映射到服务器类型名
        }
    }
    //for(auto it:this->serverSelected) cout<<it.first << " " << it.second << endl;

    /**
     * 虚拟机售卖序列
     *
     * */
    //虚拟机类型按比值分类(CPU符不符合)+累加
    this->mNumServerTypeByPercent = this->serverSelected.size();  //虚拟机对应服务器类型分类的数目


    cin >> this->mVMTypeNum;   //售卖数量
    for (int i = 0; i < this->mVMTypeNum; i++) {
        cin >> VMTypeName >> cpus >> memory >> isDouble;
        inputVm(i, VMTypeName, cpus, memory, isDouble);


        vector<double> vec;     //虚拟机与所有服务器比值的距离保存下来
        vec.erase(vec.begin(), vec.end());
        for (int j = 0; j < this->mNumServerTypeByPercent; j++) {
            vec.emplace_back(fabs(this->mVMTypeByCpu[i].cpus * 1.0 / this->mVMTypeByCpu[i].memory -
                                  this->serverSelected[j].second * 1.0));
        }
        int minDistanceIndex = min_element(vec.begin(), vec.end()) - vec.begin();
        //cout << minDistanceIndex << endl;
        ServerType tmp = this->mServerTypeMap[this->serverSelected[minDistanceIndex].first];
        if (tmp.memory >= this->mVMTypeByCpu[i].memory && tmp.cpus >= this->mVMTypeByCpu[i].cpus) {
            this->vmTypeToServerType[this->mVMTypeByCpu[i].vmTypeName] = this->serverSelected[minDistanceIndex].first;    //虚拟机类型映射到服务器类型
        } else {
            cout << "最大的不能分配" << endl;
        }
    }
    sort(this->mVMTypeByCpu, this->mVMTypeByCpu + this->mVMTypeNum, vmTypeCmpCpu);           //按cpu排序
    sort(this->mVMTypeByMemory, this->mVMTypeByMemory + this->mVMTypeNum, vmTypeCmpMemory);  //按内存排序

    // cout<<"虚拟机\n";
    //cout<< this->mVMTypeNum << endl;
//    for(int i=0;i<this->mVMTypeNum;i++)
//        this->mHelper.printVMType(this->mVMTypeByCpu[i]);

    /**
     * 请求序列
     *
     */


    cin >> this->mDays;  //请求天数
    //cout << this->mDays << endl;

    for (int i = 0; i < this->mDays; ++i) {
        int requestNum = 0;  //当天请求数
        cin >> requestNum;
        //cout << requestNum << endl;
        vector<Request> vec;
        vec.erase(vec.begin(), vec.end());   //先清空容器

        for (int j = 0; j < requestNum; j++) {
            cin >> requestType;
            if (requestType[1] == 'a') {
                cin >> VMTypeName >> vmId;
                inputRequest(vec, requestType, VMTypeName, vmId);
            } else {
                cin >> vmId;
                inputRequest(vec, requestType, vmId);
            }
        }

        getMemoryPerDay(i, vec);
        this->mRequest.push_back(vec);
    }
    //long long sum = 0;
    for (int i = 0; i < this->mMemoryPerDay.back().size(); i++) {
        ServerType tmp = mServerTypeMap[this->serverSelected[i].first];
        int cur = int(this->mMemoryPerDay.back()[i] * 1.0 / tmp.memory * 0.45);
        this->mNumPerServerType.emplace_back(cur);    //将每种服务器类型的数量加入
        //sum += cur;
    }
    //cout << sum <<endl;
    // cout << "input over" << endl;

    //fclose(fp);
    mMemoryPerDay.erase(mMemoryPerDay.begin(), mMemoryPerDay.end());
    mMemoryPerDayDel.erase(mMemoryPerDayDel.begin(), mMemoryPerDayDel.end());

}

void Solution::run() {
    this->input();
    this->judge();
    //this->printTest();

//    for(int i=0;i<100;i++){
//        this->mHelper.printServer(this->mServer[i]);
//    }
}

void Solution::getMemoryPerDay(int i,vector<Request> &vec){
    //统计每种类型服务器需要的内存量
    vector<long long> curMemoryPerServer(this->mNumServerTypeByPercent,0);   //当天内存需求量
    vector<long long> curMemoryDel(this->mNumServerTypeByPercent,0);         //当天内存删除量

    if(i == 0){ //第一天
        for(auto it:vec){       //遍历
            string selected = this->vmTypeToServerType[it.vmTypeName]; //选取虚拟机对应的服务器类型
            int index = this->serverSelectedIndex[selected];    //对应服务器的下标
            int memory = this->mMVTypeMap[this->vmToVMType[it.vmId]].memory;    //对应虚拟机的所需内存
            if(it.requestType == "add"){
                curMemoryPerServer[index] += memory;   //将虚拟机的内存累加到对应的服务器
            }else{  //del
                curMemoryDel[index] = memory;          //将虚拟机的内存累加到对应的服务器
            }
        }
    }else{  //第2天...第n天
        for(auto it:vec){       //统计一天的总量
            string selected = this->vmTypeToServerType[it.vmTypeName]; //选取虚拟机对应的服务器类型
            int index = this->serverSelectedIndex[selected];    //对应服务器的下标
            int memory = this->mMVTypeMap[this->vmToVMType[it.vmId]].memory;    //对应虚拟机的所需内存
            if(it.requestType == "add"){
                curMemoryPerServer[index] += memory;   //将虚拟机的内存累加到对应的服务器
            }else{  //del
                curMemoryDel[index] = memory;          //将虚拟机的内存累加到对应的服务器
            }
        }

        for(int j=0;j<curMemoryPerServer.size();j++){   //第二天 = 第二天+ 第一天 - 第一天删除的
            curMemoryPerServer[j] += this->mMemoryPerDay.back()[j] - this->mMemoryPerDayDel.back()[j];
        }
    }
    this->mMemoryPerDay.emplace_back(curMemoryPerServer);  //将内存加入对应的服务器类型中 ,最后一个就是昨天的
    this->mMemoryPerDayDel.emplace_back(curMemoryDel);     //将删除的内存加入对应的服务器类型中 ,最后一个就是昨天的
}
void Solution::write_answer() {

}


void Solution::printTest(){

//    for(auto it:this->mServerTypeMap) this->mHelper.printServerType(it.second);
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

void Solution::inputRequest(vector<Request> &vec,string &requestType,string &vmTypeName,string &vmId){
    string _requestType,_vmTypeName;
    int _vmId = 0;
    for(int i=0;i<vmId.size()-1;i++){
        _vmId = _vmId*10 + vmId[i] - '0';
    }
    _requestType = requestType.substr(1,requestType.size() -2);
    _vmTypeName = vmTypeName.substr(0,vmTypeName.size() -1);
    Request req = {_requestType,_vmTypeName,_vmId};
    vec.push_back(req);
    this->vmToVMType[_vmId] = _vmTypeName;  //虚拟机id映射到虚拟机类型
}

void Solution::inputRequest(vector<Request> &vec,string &requestType,string &vmId){
    string _requestType;
    int _vmId=0;
    for(int i=0;i<vmId.size()-1;i++){
        _vmId = _vmId*10 + vmId[i] - '0';
    }
    _requestType = requestType.substr(1,requestType.size() - 2);
    Request req = {_requestType,"",_vmId};
    vec.push_back(req);
}
void Solution::inputVm(int index,string &vmTypeName,string &cpus,string &memory,string &isDouble){
    string _vmTypeName;
    for(int i=1;i<vmTypeName.size() -1;i++){
        _vmTypeName += vmTypeName[i];
    }

    int _cpus = 0,_memory=0,_isDouble=0;
    for(int i=0;i<cpus.size()-1;i++){
        _cpus = _cpus*10 + cpus[i] - '0';
    }
    for(int i=0;i<memory.size()-1;i++){
        _memory = _memory*10 + memory[i] - '0';
    }
    if(isDouble[0] == '1'){
        _isDouble = 1;
    }
    else{
        _isDouble =0;
    }
    VMType t={_vmTypeName,_cpus,_memory,_isDouble};
    this->mVMTypeByCpu[index] = t;
    this->mVMTypeByMemory[index] = t;
    this->mMVTypeMap[_vmTypeName] = t;
}
void Solution::inputServer(int index,string &serverTypeName,string &cpus,string &memory,string &hardCost,string &energyCost){
    string _serverTypeName;
    for(int i =1;i<serverTypeName.size() -1;i++){
        _serverTypeName += serverTypeName[i];
    }
    int _cpus =0,_memory=0,_hardCost=0,_energyCost=0;

    for(int i=0;i<cpus.size() -1;i++){
        _cpus = 10*_cpus + cpus[i] - '0';
    }
    for(int i=0;i<memory.size() -1;i++){
        _memory = 10*_memory + memory[i] - '0';
    }
    for(int i=0;i<hardCost.size() -1;i++){
        _hardCost = 10*_hardCost + hardCost[i] - '0';
    }
    for(int i=0;i<energyCost.size()-1;i++){
        _energyCost = 10*_energyCost + energyCost[i] - '0';
    }
    ServerType t = {_serverTypeName,_cpus,_memory,_hardCost,_energyCost};
    this->mServerTypeByCpu[index]=t;
    this->mServerTypeByPercent[index]=t;
    this->mServerTypeMap[_serverTypeName] = t;
}

void Solution::judge(){

    for (int i = 0; i < this->mDays; ++i) {
        if(i == 0){
            cout << "(purchase, " <<  this->mNumServerTypeByPercent << ")"<< endl;
            for (int j = 0; j <  this->mNumServerTypeByPercent; ++j) {    //先全部买
                string name = this->indexSelectedServer[j];
                int reqNum = this->mNumPerServerType[j];
                ServerType tmpType =  this->mServerTypeMap[name];
                cout << "(" << name << ", "<< reqNum <<")"<< endl;
                pair<int,int> t={tmpType.cpus/2,tmpType.memory/2};
                this->mMinIndex.emplace_back(this->mServerId);      //保存当前服务器类型的最小下标
                this->mCurIndex.emplace_back(this->mServerId);      //保存当前服务器类型的当前下标
                this->mReIndex.emplace_back(this->mServerId);   //请求下个服务器的下标
                for(int k=0;k<reqNum;k++){
                    this->mServer[this->mServerId] = {name,this->mServerId,t,t};      //加入购买的服务器
                    this->mServerId++;
                }
                this->mMaxIndex.emplace_back(this->mServerId);  //保存当前服务器类型的最大下标
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
       //cout << "第" << i << "天" << endl;
        deploy(i,0);
    }
    //cout<< sum << endl;

}

//void Solution::purchase(){
//
//}
//void Solution::migrate(){
//
//}
void Solution::deploy(int i,int k){
    int j;
    for (j = k; j < this->mRequest[i].size(); ++j) {
        Request curReq = this->mRequest[i][j];
        if(curReq.requestType == "add" ){     //(add, 虚拟机类型，虚拟机id)
            VMType vmType = this->mMVTypeMap[curReq.vmTypeName];    //虚拟机类型
            int index = this->serverSelectedIndex[this->vmTypeToServerType[vmType.vmTypeName]];    //虚拟机类型名对应的服务器类型名,再找对应的服务器下标
            //int curId = this->mCurIndex[index]; //当前服务器下标
            int curId = this->mMinIndex[index];              //每次从当前服务器最小的下标开始遍历
            if(curId != this->mReIndex[index]){ //当前id与再次请求的不一样，证明得移到下个服务器
                curId = this->mReIndex[index];
            }
            Server curServer = this->mServer[curId];
            int ACpus = curServer.A.first;      //这里不能除以2了
            int AMemory = curServer.A.second;
            int BCpus = curServer.B.first;
            int BMemory = curServer.B.second;
            int curCpus = vmType.cpus;
            int curMemory = vmType.memory;
            if(vmType.isDouble){    //双结点
                if(ACpus >= curCpus/2 && AMemory >= curMemory/2 && BCpus >= curCpus/2 && BMemory >= curMemory/2 ){
                    this->mServer[curId].A.first -= curCpus/2;
                    this->mServer[curId].A.second -= curMemory/2;
                    this->mServer[curId].B.first -= curCpus/2;
                    this->mServer[curId].B.second -= curMemory/2;
                    this->vmToServer[curReq.vmId]= make_pair(curId,0);    //虚拟机id映射到服务器id,结点(0双结点，1:A结点,2:B结点)
                    cout << "("<< curId << ")"<< endl;
                    this->mReIndex[index] = this->mMinIndex[index];  //部署成功复原
                }else{
                    if(this->mReIndex[index] == this->mMaxIndex[index]){
                        cout<<"超标"<<endl;
                    }
                    this->mReIndex[index]++;    //不符合，下一个服务器
                    j--;        //保持当前请求
                }
            } else{ //单结点
                if(ACpus >= curCpus && AMemory >= curMemory){
                    this->mServer[curId].A.first -= curCpus;
                    this->mServer[curId].A.second -= curMemory;
                    this->vmToServer[curReq.vmId]= make_pair(curId,1);
                    cout << "("<< curId << ", A)"<< endl;
                    this->mReIndex[index] = this->mMinIndex[index];  //部署成功复原
                }
                else if(BCpus >= curCpus && BMemory >= curMemory ){
                    this->mServer[curId].B.first -= curCpus;
                    this->mServer[curId].B.second -= curMemory;
                    this->vmToServer[curReq.vmId]= make_pair(curId,2);
                    cout << "("<< curId << ", B)"<< endl;
                    this->mReIndex[index] = this->mMinIndex[index];  //部署成功复原
                }else{
//                    this->mCurIndex[index]++;
                    if(this->mReIndex[index] == this->mMaxIndex[index]){
                        cout<<"超标"<<endl;
                    }
                    this->mReIndex[index]++;    //不符合，下一个服务器
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

    for(int l=0;l<this->mNumServerTypeByPercent;l++){   //看当前服务器id是否遍历最大,是的话置为对应服务器的最小下标
        if(this->mCurIndex[l] >= mMaxIndex[l])
            this->mCurIndex[l] = mMinIndex[l];
    }
}
