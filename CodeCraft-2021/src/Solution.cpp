//
// Created by zcz on 2021/3/11.
//

#include "Solution.h"
#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <iostream>
#include <cmath>
#include <thread>

using namespace std;

Solution::Solution(string &filename) {
    this->mFilename = filename;
    this->mDays = 0;
    this->mServerTypeNum = 0;
    this->mVMTypeNum = 0;
    this->mNumServerTypeByPercent = 0;
    this->mMax = 0;
    this->mMaxServerId = INT_MIN;
}

Solution::Solution() {
    this->mDays = 0;
    this->mServerTypeNum = 0;
    this->mVMTypeNum = 0;
    this->mNumServerTypeByPercent = 0;
    this->mMax = 0;
    this->mMaxServerId = INT_MIN;
}

/**
 * 运行主函数
 */
void Solution::run() {
    this->input();
    this->judge();
    this->myOutput();
}


void Solution::myOutput(){
    this->mDeployMsg[1] = "("+this->mSelectedServerType.serverTypeName+", "+to_string(this->mMaxServerId+1)+")";
    for(auto &it:this->mDeployMsg){
        cout << it << endl;
    }
}
/**
 * 处理数据输入
 */
void Solution::input() {

    /// 文件输入流重定向到标准输入流
    //FILE *fp = freopen(this->mFilename.c_str(), "r", stdin);

    /// 服务器类型,请求类型
    string serverTypeName, VMTypeName, requestType;
    /// cpu核数,内存,硬件成本，能耗成本，是否双结点部署
    string cpus, memory, hardCost, energyCost, isDouble, vmId;
    /// 服务器CPU/内存比值  set自动去重和排序(从小到大)
    std::set<int> percentTypeSet;

    /******************
     * 服务器类型序列
     ******************/
    cin >> this->mServerTypeNum;                                // 服务器类型数量
    //cout << this->mServerTypeNum << endl;
    for (int i = 0; i < this->mServerTypeNum; i++) {            // 遍历服务器类型数量，读取相应的数据
        cin >> serverTypeName >> cpus >> memory >> hardCost >> energyCost;
        inputServer(i, serverTypeName, cpus, memory, hardCost, energyCost);
    }

    /// (1)统计cpu/内存的比值，输入到set中，（set自动去重，排序）
    //cout<<"服务器\n";
    for (int i = 0; i < this->mServerTypeNum; i++) {               //加入哈希表
        //this->mHelper.printServerType(this->mServerType[i]);
        this->mServerTypeByPercent[i].percent = int(
                this->mServerTypeByPercent[i].cpus * 1.0 / this->mServerTypeByPercent[i].memory);   //存储所有类型服务器
        percentTypeSet.insert(this->mServerTypeByPercent[i].percent);                        //存储所有比值
    }
    sort(this->mServerTypeByPercent, this->mServerTypeByPercent + this->mServerTypeNum,
         serverTypeCmpPercent); //按比值从小到大排序
    //for(auto it:this->mServerTypeByPercent) this->mHelper.printServerTypeBypercent(it);     //打印服务器类型  按比值从小到大排序

    /// (2)划分，选取服务器类型
    vector<ServerType> _serverType;
    auto iter = percentTypeSet.begin();
    for (int i = 0; i < this->mServerTypeNum; i++) {        // 遍历服务器类型
        ServerType tmp = this->mServerTypeByPercent[i];     // 从比值最小开始遍历
        if (*iter == tmp.percent) {                         // 比例相同加入
            _serverType.push_back(tmp);
        } else {
            int tMemory = 0, tCpus = 0, index = 0, tPercent = 0;
            for (int j = 0; j < _serverType.size(); j++) {     // 将相同比值的服务器类型进行对比
                if (_serverType[j].memory > tMemory) {         // 保存内存大的
                    index = j;
                    tMemory = _serverType[j].memory;
                    tCpus = _serverType[j].cpus;
                    tPercent = _serverType[j].percent;
                } else if (_serverType[j].memory == tMemory) {  // 内存相等，保存cpu大的
                    if (_serverType[j].cpus > tCpus) {
                        index = j;
                        tMemory = _serverType[j].memory;
                        tCpus = _serverType[j].cpus;
                        tPercent = _serverType[j].percent;
                    }
                }
            }
            this->serverSelected.emplace_back(make_pair(_serverType[index].serverTypeName, tPercent));  //选取的服务器类型
            this->serverSelectedIndex[_serverType[index].serverTypeName] =
                    this->serverSelected.size() - 1;                                 // 服务器类型名映射到serverSelected下标
            this->indexSelectedServer[this->serverSelected.size() -
                                      1] = _serverType[index].serverTypeName;        // serverSelected下标映射到服务器类型名
            _serverType.erase(_serverType.begin(), _serverType.end());    // 删除之前所有的
            _serverType.push_back(tmp);                                              // 加入当前的
            iter++;                                                                  //遍历下个比值
        }
        if (i == this->mServerTypeNum - 1) {                            //处理最后一个比值
            int tMemory = 0, tCpus = 0, index = 0, tPercent = 0;
            for (int j = 0; j < _serverType.size(); j++) {
                if (_serverType[j].memory > tMemory) {                  //保存内存大的
                    index = j;
                    tCpus = _serverType[j].cpus;
                    tMemory = _serverType[j].memory;
                    tPercent = _serverType[j].percent;
                } else if (_serverType[j].memory == tMemory) {          //内存相等，保存cpu大的
                    if (_serverType[j].cpus > tCpus) {
                        index = j;
                        tCpus = _serverType[j].cpus;
                        tMemory = _serverType[j].memory;
                        tPercent = _serverType[j].percent;
                    }
                }
            }
            this->serverSelected.emplace_back(make_pair(_serverType[index].serverTypeName, tPercent));      //选取的服务器类型
            this->serverSelectedIndex[_serverType[index].serverTypeName] =
                    this->serverSelected.size() - 1;//服务器类型名映射到serverSelected下标
            this->indexSelectedServer[this->serverSelected.size() -
                                      1] = _serverType[index].serverTypeName;                   // serverSelected下标映射到服务器类型名
        }
    }

    /// 打印选取的<服务器类型,比值>
    //for(auto it:this->serverSelected) cout<<it.first << " " << it.second << endl;

    /******************
     * 虚拟机类型序列
     ******************/
    /// 虚拟机类型按比值分类(CPU符不符合)+累加
    this->mNumServerTypeByPercent = this->serverSelected.size();                            //虚拟机对应服务器类型分类的数目

    /// 虚拟机类型数量
    cin >> this->mVMTypeNum;
    for (int i = 0; i < this->mVMTypeNum; i++) {
        cin >> VMTypeName >> cpus >> memory >> isDouble;
        inputVm(i, VMTypeName, cpus, memory, isDouble);

        /// (3)划分虚拟机类型到对应的服务器类型中去
        vector<double> vec;                                                                 //虚拟机与所有服务器比值的距离保存下来
        vec.erase(vec.begin(), vec.end());
        for (int j = 0;
             j < this->mNumServerTypeByPercent; j++) {                           //计算第i种类型虚拟机比值与所有我们选取的服务器比值的距离
            vec.emplace_back(fabs(this->mVMType[i].cpus * 1.0 / this->mVMType[i].memory -
                                  this->serverSelected[j].second * 1.0));
        }
        int minDistanceIndex = min_element(vec.begin(), vec.end()) - vec.begin();           //取最小距离的服务器类型下标
        //cout << minDistanceIndex << endl;

        ServerType tmp = this->mServerTypeMap[this->serverSelected[minDistanceIndex].first];
        if (tmp.memory >= this->mVMType[i].memory && tmp.cpus >= this->mVMType[i].cpus) {
            this->vmTypeToServerType[this->mVMType[i].vmTypeName] = this->serverSelected[minDistanceIndex].first;    //虚拟机类型名映射到服务器类型名
        } else {
            cout << "最大的不能分配" << endl;
        }

        ///打印虚拟机类型名映射到服务器类型名
        //if(i < 200)
        //    cout << "比值："<<this->mVMType[i].cpus * 1.0 / this->mVMType[i].memory<<"  " <<this->mVMType[i].vmTypeName <<" ==> " << this->serverSelected[minDistanceIndex].first << endl;
    }

    /// 打印虚拟机类型
//    cout<<"虚拟机\n";
//    cout<< this->mVMTypeNum << endl;
//    for(int i=0;i<this->mVMTypeNum;i++)
//        this->mHelper.printVMType(this->mVMType[i]);

    /******************
     * 请求序列
     ******************/
    cin >> this->mDays;  //请求天数
    //cout << this->mDays << endl;
    /// 初始化每种类型服务器所需最大的内存量
    for (int i = 0; i < this->mDays; ++i) {
        int requestNum = 0;                             //当天请求数
        cin >> requestNum;
        //cout << requestNum << endl;
        vector<Request> vec;                            //存储当天的请求数据
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
        /// (4)统计每种类型服务器每天内存需求量，删除量模块
        getMemoryPerDay(i, vec);
        this->mRequest.push_back(vec);
    }

//    int vmTotalM=0,vmTotalC=0;  //所有天数的内存量,CPU量
//    vmTotalM += it.memory;
//    vmTotalC += it.cpus;
//    cout << "所有天数的内存量：" << vmTotalM <<"  CPU量：" << vmTotalC <<endl;

    /// 统计各类服务器所需的数量，以及总成本
    //long long sum = 0;        //总成本
    if (this->mServerTypeNum > 80)
        this->mMax = 10000;      // 第2个数据集3264  3246
    else
        this->mMax = 10000;      //第1个数据集 3737 3734

//    for (int i = 0; i < this->mNumServerTypeByPercent;i++) {
//        int cur = 0;
//        if(i == this->serverSelectedIndex["host2D54I"])
//            cur = this->mMax;
//        this->mNumPerServerType.emplace_back(cur);    //将每种服务器类型的数量加入
//        //sum += cur*tmp.hardCost;
//    }
    //cout << sum <<endl;

    /// 统计虚拟机 所有天数中哪一天的内存，cpu最大
    int maxM = *max_element(mMemoryPerDay.begin(), mMemoryPerDay.end());
    //for(auto it:mMemoryPerDay) cout << it << endl;
    int maxC = *max_element(mCpuPerDay.begin(), mCpuPerDay.end());
    //cout <<"所有天数中最大的内存：" << maxM << " CPU:" << maxC<< endl;
    /// 最大CPU比内存
    double macC_M = maxC * 1.0 / maxM;

    /// 统计虚拟机类型中CPU，内存最大
    int vmMaxM = 0, vmMaxC = 0;
    for (int i = 0; i < this->mVMTypeNum; i++) {
        VMType it = this->mVMType[i];
        vmMaxM = max(vmMaxM, it.memory);
        vmMaxC = max(vmMaxC, it.cpus);

    }
    //cout << "虚拟机中最大的内存："<<vmMaxM <<"  最大的CPU：" << vmMaxC <<endl;

    /// 统计CPU
    for (int i = 0; i < this->mServerTypeNum; i++) {
        ServerType it = this->mServerType[i];
        if (it.memory / 2 >= vmMaxM && it.cpus / 2 >= vmMaxC) {       //选取满足虚拟机中最大的CPU，内存的服务器类型
            int m = it.memory;
            int c = it.cpus;
            int p = it.hardCost;
            it.cm = c * 1.0 / m;
            it.pm = p / m;
            it.pc = p / c;
            it.p_cm = p / (c + m);
            //it.totalCost = ;
            if (it.cm - macC_M >= 0 && it.cm - macC_M <= 0.3) // 在 最大存量CPU/内存  范围[0,0.3]
            {
                this->mSelectServerType.emplace_back(it);
            }
        }
    }

    /// 删除服务器类型  取能耗最小的前2/3
    sort(this->mSelectServerType.begin(), this->mSelectServerType.end(), serverTypeCmpEnergyCost);     //按能耗从小到大排序
    //for(auto &it:this->mSelectServerType)   cout << it.serverTypeName << endl;
    int delSize = this->mSelectServerType.size() * 2 / 3;
    this->mSelectServerType.resize(delSize);

    /// 选取价格/内存 最小的服务器类型

    sort(this->mSelectServerType.begin(), this->mSelectServerType.end(), serverTypeCmpPM);    //按价格/内存 从小到大排序
//    for(auto it:this->mSelectServerType) {
//        cout << it.serverTypeName <<" "<< it.pm << endl;
//    }

    this->mSelectedServerType = this->mSelectServerType[0];
    //this->mTestName = this->mSelectServerType[0].serverTypeName;
    //cout << this->mTestName<< endl;

    /// 释放资源
    //fclose(fp);
    mMemoryPerDay.erase(mMemoryPerDay.begin(), mMemoryPerDay.end());
    mMemoryPerDayDel.erase(mMemoryPerDayDel.begin(), mMemoryPerDayDel.end());

}

/********************************************************************/
/**                      统计模块                                   **/
/********************************************************************/

/**
 * 统计每种类型服务器每天内存需求量，删除量模块
 * @param i 第i天
 * @param vec 第i天请求的数据
 */

void Solution::getMemoryPerDay(int i, vector<Request> &vec) {
    //统计每种类型服务器需要的内存量
    long long curMemoryPerServer = 0;   //当天内存需求量
    long long curMemoryDel = 0;         //当天内存删除量
    long long curCpuPerServer = 0;   //当天Cpu需求量
    long long curCpuDel = 0;         //当天Cpu删除量

    if (i == 0) { //第一天
        for (auto &it:vec) {             //遍历
            int memory = this->mMVTypeMap[this->vmToVMType[it.vmId]].memory;    //对应虚拟机的所需内存
            int cpu = this->mMVTypeMap[this->vmToVMType[it.vmId]].cpus;         //对应虚拟机的所需内存
            if (it.requestType == "add") {
                curMemoryPerServer += memory;   //将虚拟机的内存累加到对应的服务器
                curCpuPerServer += cpu;
            } else {  //del
                curMemoryDel += memory;          //将虚拟机的内存累加到对应的服务器
                curCpuDel += cpu;
            }
        }
    } else {  //第2天...第n天
        for (auto &it:vec) {       //统计一天的总量
            int memory = this->mMVTypeMap[this->vmToVMType[it.vmId]].memory;    //对应虚拟机的所需内存
            int cpu = this->mMVTypeMap[this->vmToVMType[it.vmId]].cpus;    //对应虚拟机的所需内存
            if (it.requestType == "add") {
                curMemoryPerServer += memory;   //将虚拟机的内存累加到对应的服务器
                curCpuPerServer += cpu;
            } else {  //del
                curMemoryDel += memory;          //将虚拟机的内存累加到对应的服务器
                curCpuDel += cpu;
            }
        }
        //第二天 = 第二天+ 第一天 - 第一天删除的
        curMemoryPerServer += this->mMemoryPerDay.back() - this->mMemoryPerDayDel.back();
        curCpuPerServer += this->mCpuPerDay.back() - this->mCpuPerDayDel.back();
    }
    this->mMemoryPerDay.emplace_back(curMemoryPerServer);  //将内存加入对应的服务器类型中 ,最后一个就是昨天的
    this->mMemoryPerDayDel.emplace_back(curMemoryDel);     //将删除的内存加入对应的服务器类型中 ,最后一个就是昨天的
    this->mCpuPerDay.emplace_back(curCpuPerServer);
    this->mCpuPerDayDel.emplace_back(curCpuDel);
}



/********************************************************************/
/**                      决策模块                                   **/
/********************************************************************/

void Solution::judge() {
    for (int i = 0; i < this->mDays; ++i) {
        if (i == 0) {     /// (1) 第一天购买所需要的全部服务器
            //cout << "(purchase, 1)" << endl;
            this->mDeployMsg.emplace_back("(purchase, 1)");

            int tC = this->mSelectedServerType.cpus / 2, tM = this->mSelectedServerType.memory / 2;
            //cout << "(" << this->mSelectedServerType.serverTypeName << ", " << this->mMax << ")" << endl;
            this->mDeployMsg.emplace_back("("+this->mSelectedServerType.serverTypeName+", "+to_string(this->mMax)+")");
            pair<int, int> t = {tC, tM};
            for (int k = this->mMax - 1; k >= 0; k--) {       //id 从大到小
                Server s = {this->mSelectedServerType.serverTypeName, k, t, t};
                //this->mServer[this->mServerId] = s;      //加入购买的服务器
                this->mNoHasVm.emplace_back(s);
                this->mServerId++;
            }
        } else {
            //cout << "(purchase, 0)" << endl;
            this->mDeployMsg.emplace_back("(purchase, 0)");
        }

        /// (2) 迁移
        //cout << "(migration, 0)" << endl;
        this->mDeployMsg.emplace_back("(migration, 0)");

        /// (3) 部署
//        clock_t start = clock();
        deploy(i, 0);
//        clock_t ends = clock();
//        cout <<"Running Time "<<i<<" "<<(double)(ends - start)/ CLOCKS_PER_SEC << endl;
    }
}

void Solution::getDoubleIndex2(int tid, int numSize, int cpu, int memory, vector<int> &res, vector<Server> &s) {
    int end = numSize * (tid + 1);      //线程对应的开始下标
    if (tid == NUM_THREADS - 1) {       //最后一个线程，则遍历到最后
        end = s.size();
    }
    for (int i = numSize * tid; i < end; i++) {   //遍历对应的序列
        if (s[i].A.first >= cpu && s[i].A.second >= memory && s[i].B.first >= cpu && s[i].B.second >= memory) {
            if (!((s[i].A.first - cpu <= WASTER_LOW && s[i].A.second - memory >= WASTER_HIGH) ||
                  (s[i].A.second - memory <= WASTER_LOW && s[i].A.first - cpu >= WASTER_HIGH)
                  || (s[i].B.first - cpu <= WASTER_LOW && s[i].B.second - memory >= WASTER_HIGH) ||
                  (s[i].B.second - memory <= WASTER_LOW && s[i].B.first - cpu >= WASTER_HIGH))) {
                res[tid] = i;
                break;
            }
        }
    }
}

void
Solution::getSingleIndex2(int tid, int numSize, int cpu, int memory, vector<int> &res, vector<Server> &s, vector<int> &node) {
    int end = numSize * (tid + 1);      //线程对应的开始下标
    if (tid == NUM_THREADS - 1) {       //最后一个线程，则遍历到最后
        end = s.size();
    }
    for (int i = numSize * tid; i < end; i++) {   //遍历对应的序列
        if (s[i].A.first >= cpu && s[i].A.second >= memory) {
            if (!((s[i].A.first - cpu <= WASTER_LOW && s[i].A.second - memory >= WASTER_HIGH) ||
                  (s[i].A.second - memory <= WASTER_LOW && s[i].A.first - cpu >= WASTER_HIGH))) {
                res[tid] = i;
                node[tid] = 1;
                break;
            }
        }
        if (s[i].B.first >= cpu && s[i].B.second >= memory) {
            if (!((s[i].B.first - cpu <= WASTER_LOW && s[i].B.second - memory >= WASTER_HIGH) ||
                  (s[i].B.second - memory <= WASTER_LOW && s[i].B.first - cpu >= WASTER_HIGH))) {
                res[tid] = i;
                node[tid] = 2;
                break;
            }
        }
    }
}

/// 第2个策略
void Solution::getDoubleIndex(int tid, int numSize, int cpu, int memory, vector<int> &res, vector<Server> &s) {
    int end = numSize * (tid + 1);      //线程对应的开始下标
    if (tid == NUM_THREADS - 1) {       //最后一个线程，则遍历到最后
        end = s.size();
    }
    for (int i = numSize * tid; i < end; i++) {   //遍历对应的序列
        if (s[i].A.first >= cpu && s[i].A.second >= memory && s[i].B.first >= cpu && s[i].B.second >= memory) {
            res[tid] = i;
            break;
        }
    }
}

/// 第2个策略
void
Solution::getSingleIndex(int tid, int numSize, int cpu, int memory, vector<int> &res, vector<Server> &s, vector<int> &node) {
    int end = numSize * (tid + 1);      //线程对应的开始下标
    if (tid == NUM_THREADS - 1) {       //最后一个线程，则遍历到最后
        end = s.size();
    }
    for (int i = numSize * tid; i < end; i++) {   //遍历对应的序列
        if (s[i].A.first >= cpu && s[i].A.second >= memory) {
            res[tid] = i;
            node[tid] = 1;       /// A结点
            break;
        }
        if (s[i].B.first >= cpu && s[i].B.second >= memory) {
            res[tid] = i;
            node[tid] = 2;   /// B结点
            break;
        }
    }
}

/********************************************************************/
/**                      部署模块                                   **/
/********************************************************************/
/**
 *
 * @param i 第i天
 * @param k 第i天请求数据的第k条数据(从下标0开始)
 */
void Solution::deploy(int i, int k) {
    int j;
    int curIndex = -1;
    for (j = k; j < this->mRequest[i].size(); ++j) {


        /// 调试 服务器资源超标
//        for(auto &it:this->mHasVm){
//            if(it.A.first < 0 || it.A.second < 0 || it.B.first <0 || it.B.second <0){
//                cout << it.id <<" **** "<<it.A.first  <<" " << it.A.second <<" "<<it.B.first  <<" " << it.B.second << endl;
//             }
//        }

        Request curReq = this->mRequest[i][j];
        if (curReq.requestType == "add") {     //(add, 虚拟机类型，虚拟机id)
            VMType vmType = this->mMVTypeMap[curReq.vmTypeName];    //虚拟机类型
            int curCpus = vmType.cpus;
            int curMemory = vmType.memory;

            if (this->mHasVm.empty()) { // 是不是空的
                if (this->mNoHasVm.empty()) { // 服务器没有存量了
                    cout << "超标:第" << i << "天" << endl;
                    return;
                }
                Server t = this->mNoHasVm.back();
                this->mHasVm.emplace_back(t);           // 将没有部署虚拟机的第一个服务器，添加到队列中
                this->mNoHasVm.pop_back();   // 删除第一个
            }

            if (vmType.isDouble) {        /// 双结点  符合
                /// 第1个策略
                curIndex = -1;
                vector<int> res(NUM_THREADS, -1);//存放结果，服务器的下标
                int numSize = this->mHasVm.size() / NUM_THREADS; //划分每个线程处理的序列长度
                for (int l = 0; l < NUM_THREADS; ++l) {
                    this->mThread[l] = std::thread(&Solution::getDoubleIndex2, l, numSize, curCpus / 2, curMemory / 2,
                                                   ref(res), ref(this->mHasVm));//调用函数要加类名，并且传引用，要用ref()
                }
                /// 等待所有线程结束，再取
                for (auto &l : this->mThread) {
                    l.join();
                }

                /// 取出找到的下标
                for (auto it:res) {
                    if (it != -1) {
                        curIndex = it;
                        break;
                    }
                }
                if (curIndex == -1) {        /// 第1个策略没有找到了
                    /// 第2策略   必须部署   上面成功，下面不运行上面成功，下面不运行
                    curIndex = -1;
                    vector<int> res2(NUM_THREADS, -1);//存放结果，服务器的下标
                    numSize = this->mHasVm.size() / NUM_THREADS; //划分每个线程处理的序列长度
                    for (int l = 0; l < NUM_THREADS; ++l) {
                        this->mThread[l] = std::thread(&Solution::getDoubleIndex, l, numSize, curCpus / 2,
                                                       curMemory / 2, ref(res2),
                                                       ref(this->mHasVm));//调用函数要加类名，并且传引用，要用ref()
                    }
                    /// 等待所有线程结束，再取
                    for (auto &l : this->mThread) {
                        l.join();
                    }

                    /// 取出找到的下标
                    for (auto it:res2) {
                        if (it != -1) {
                            curIndex = it;
                            break;
                        }
                    }

                    if (curIndex == -1) {      /// 没有找到 取新的部署
                        if (this->mNoHasVm.empty()) {           //服务器没有存量了
                            cout << "超标:第" << i << "天" << endl;
                            return;
                        } else {      /// 有存量
                            Server t = this->mNoHasVm.back();
                            this->mHasVm.emplace_back(t);           // 将没有部署虚拟机的第一个服务器，添加到队列中
                            this->mNoHasVm.pop_back();   // 删除第一个
                            curIndex = this->mHasVm.size() - 1;
                        }
                    }

                    /// 部署
                    this->mHasVm[curIndex].A.first -= curCpus / 2;
                    this->mHasVm[curIndex].A.second -= curMemory / 2;
                    this->mHasVm[curIndex].B.first -= curCpus / 2;
                    this->mHasVm[curIndex].B.second -= curMemory / 2;
                    //cout << "(" << this->mHasVm[curIndex].id << ")" << endl;
                    this->mDeployMsg.emplace_back("("+to_string(this->mHasVm[curIndex].id)+")");
                    this->mMaxServerId = max(this->mMaxServerId,this->mHasVm[curIndex].id);

                    this->vmToServer[curReq.vmId] = make_pair(this->mHasVm[curIndex].id,
                                                              0);    //虚拟机id映射到服务器id,结点(0双结点，1:A结点,2:B结点)
                } else {      /// 不浪费
                    this->mHasVm[curIndex].A.first -= curCpus / 2;
                    this->mHasVm[curIndex].A.second -= curMemory / 2;
                    this->mHasVm[curIndex].B.first -= curCpus / 2;
                    this->mHasVm[curIndex].B.second -= curMemory / 2;
                    //cout << "(" << this->mHasVm[curIndex].id << ")" << endl;
                    this->mDeployMsg.emplace_back("("+to_string(this->mHasVm[curIndex].id)+")");
                    this->mMaxServerId = max(this->mMaxServerId,this->mHasVm[curIndex].id);

                    this->vmToServer[curReq.vmId] = make_pair(this->mHasVm[curIndex].id,
                                                              0);    //虚拟机id映射到服务器id,结点(0双结点，1:A结点,2:B结点)
                }
            } else {      /// 单结点
                /// A结点 符合
                /// 第1个策略
                curIndex = -1;
                int curNode = -1;
                vector<int> res(NUM_THREADS, -1);//存放结果，服务器的下标
                vector<int> node(NUM_THREADS,-1);   //哪个结点
                int numSize = this->mHasVm.size() / NUM_THREADS; //划分每个线程处理的序列长度

                for (int l = 0; l < NUM_THREADS; ++l) {
                    this->mThread[l] = std::thread(&Solution::getSingleIndex2, l, numSize, curCpus, curMemory, ref(res),
                                                   ref(this->mHasVm), ref(node));   //调用函数要加类名，并且传引用，要用ref()
                }
                /// 等待所有线程结束，再取
                for (auto &l : this->mThread) {
                    l.join();
                }

                /// 取出找到的下标
                for (auto it:res) {
                    if (it != -1) {
                        curIndex = it;
                        break;
                    }
                }
                /// 找线程id小的，且找到的结点
                for (auto it:node) {
                    if (it != -1) {
                        curNode = it;
                        break;
                    }
                }


                if (curIndex == -1) {     /// 第1个策略没有找到
                    /// 第2个策略  必须部署
                    curIndex = -1;
                    vector<int> res2(NUM_THREADS, -1);//存放结果，服务器的下标
                    numSize = this->mHasVm.size() / NUM_THREADS; //划分每个线程处理的序列长度
                    curNode = -1;   /// 结点
                    for (int l = 0; l < NUM_THREADS; ++l) {
                        this->mThread[l] = std::thread(&Solution::getSingleIndex, l, numSize, curCpus, curMemory,
                                                       ref(res2), ref(this->mHasVm),
                                                       ref(node));   //调用函数要加类名，并且传引用，要用ref()
                    }
                    /// 等待所有线程结束，再取
                    for (auto &l : this->mThread) {
                        l.join();
                    }

                    /// 取出找到的下标
                    for (auto it:res2) {
                        if (it != -1) {
                            curIndex = it;
                            break;
                        }
                    }
                    /// 找线程id小的，且找到的结点
                    for (auto it:node) {
                        if (it != -1) {
                            curNode = it;
                            break;
                        }
                    }

                    //if(curIndex == this->mHasVm.size()) {     //没有找到
                    if (curIndex == -1) {     //遍历完了
                        if (this->mNoHasVm.empty()) {           //服务器没有存量了
                            cout << "超标:第" << i << "天" << endl;
                            return;
                        } else {      // 有存量
                            Server t = this->mNoHasVm.back();
                            this->mHasVm.emplace_back(t);           // 将没有部署虚拟机的第一个服务器，添加到队列中
                            this->mNoHasVm.pop_back();   // 删除第一个
                            curIndex = this->mHasVm.size() - 1;

                            this->mHasVm[curIndex].A.first -= curCpus;
                            this->mHasVm[curIndex].A.second -= curMemory;
                            this->vmToServer[curReq.vmId] = make_pair(this->mHasVm[curIndex].id, 1);
                            //cout << "(" << this->mHasVm[curIndex].id << ", A)" << endl;
                            this->mDeployMsg.emplace_back("("+to_string(this->mHasVm[curIndex].id)+", A)");
                            this->mMaxServerId = max(this->mMaxServerId,this->mHasVm[curIndex].id);
                        }
                    } else {          ///  找到了
                        if (curNode == 1) {
                            this->mHasVm[curIndex].A.first -= curCpus;
                            this->mHasVm[curIndex].A.second -= curMemory;
                            this->vmToServer[curReq.vmId] = make_pair(this->mHasVm[curIndex].id, 1);
                            //cout << "(" << this->mHasVm[curIndex].id << ", A)" << endl;
                            this->mDeployMsg.emplace_back("("+to_string(this->mHasVm[curIndex].id)+", A)");
                            this->mMaxServerId = max(this->mMaxServerId,this->mHasVm[curIndex].id);
                        } else if (curNode == 2) {
                            this->mHasVm[curIndex].B.first -= curCpus;
                            this->mHasVm[curIndex].B.second -= curMemory;
                            this->vmToServer[curReq.vmId] = make_pair(this->mHasVm[curIndex].id, 2);
                            //cout << "(" << this->mHasVm[curIndex].id << ", B)" << endl;
                            this->mDeployMsg.emplace_back("("+to_string(this->mHasVm[curIndex].id)+", B)");
                            this->mMaxServerId = max(this->mMaxServerId,this->mHasVm[curIndex].id);
                        }
                    }
                } else {          ///  找到了
                    if (curNode == 1) {
                        this->mHasVm[curIndex].A.first -= curCpus;
                        this->mHasVm[curIndex].A.second -= curMemory;
                        this->vmToServer[curReq.vmId] = make_pair(this->mHasVm[curIndex].id, 1);
                        //cout << "(" << this->mHasVm[curIndex].id << ", A)" << endl;
                        this->mDeployMsg.emplace_back("("+to_string(this->mHasVm[curIndex].id)+", A)");
                        this->mMaxServerId = max(this->mMaxServerId,this->mHasVm[curIndex].id);
                    } else if (curNode == 2) {
                        this->mHasVm[curIndex].B.first -= curCpus;
                        this->mHasVm[curIndex].B.second -= curMemory;
                        this->vmToServer[curReq.vmId] = make_pair(this->mHasVm[curIndex].id, 2);
                        //cout << "(" << this->mHasVm[curIndex].id << ", B)" << endl;
                        this->mDeployMsg.emplace_back("("+to_string(this->mHasVm[curIndex].id)+", B)");
                        this->mMaxServerId = max(this->mMaxServerId,this->mHasVm[curIndex].id);
                    }
                }
            }
        } else {  //(del, 虚拟机id)
            VMType vmTypeTmp = this->mMVTypeMap[this->vmToVMType[curReq.vmId]];
            int curCpus = vmTypeTmp.cpus;
            int curMemory = vmTypeTmp.memory;
            int targetServerId = this->vmToServer[curReq.vmId].first;

            int index = 0;
            for (int l = 0; l < this->mHasVm.size(); l++) { //找出下标
                if (this->mHasVm[l].id == targetServerId) {
                    index = l;
                    break;
                }
            }

            if (vmTypeTmp.isDouble) {//是否为双结点
                this->mHasVm[index].A.first += curCpus / 2;    //先按vmid找服务器id，再找到对应服务器
                this->mHasVm[index].A.second += curMemory / 2;
                this->mHasVm[index].B.first += curCpus / 2;
                this->mHasVm[index].B.second += curMemory / 2;
            } else {
                if (this->vmToServer[curReq.vmId].second == 1) {  //A结点
                    this->mHasVm[index].A.first += curCpus;    //先按vmid找服务器id，再找到对应服务器
                    this->mHasVm[index].A.second += curMemory;
                } else if (this->vmToServer[curReq.vmId].second == 2) { //B结点
                    this->mHasVm[index].B.first += curCpus;
                    this->mHasVm[index].B.second += curMemory;
                }
            }
            //ServerType src = mServerTypeMap[this->mServer[targetServerId].serverTypeName];
            /// 删除虚拟机后，判断是否为服务器是否没有部署虚拟机了
            if (this->mHasVm[index].A.first + this->mHasVm[index].B.first == mSelectedServerType.cpus &&
                this->mHasVm[index].A.second + this->mHasVm[index].B.second == mSelectedServerType.memory) {
                this->mNoHasVm.emplace_back(this->mHasVm[index]);     //将服务器添加到空的队列中
                sort(this->mNoHasVm.begin(), this->mNoHasVm.end(), serverCmpId);  //按id从大到小排序
                auto iter = this->mHasVm.begin();
                while (iter != this->mHasVm.end()) {      //删除对应的服务器
                    if ((*iter).id == targetServerId) {
                        this->mHasVm.erase(iter);
                        break;
                    }
                    iter++;
                }
            }
        }
    }
}

/********************************************************************/
/**                      数据解析模块                                **/
/********************************************************************/

void Solution::inputRequest(vector<Request> &vec, string &requestType, string &vmTypeName, string &vmId) {
    string _requestType, _vmTypeName;
    int _vmId = 0;
    for (int i = 0; i < vmId.size() - 1; i++) {
        _vmId = _vmId * 10 + vmId[i] - '0';
    }
    _requestType = requestType.substr(1, requestType.size() - 2);
    _vmTypeName = vmTypeName.substr(0, vmTypeName.size() - 1);
    Request req = {_requestType, _vmTypeName, _vmId};
    vec.push_back(req);
    this->vmToVMType[_vmId] = _vmTypeName;  //虚拟机id映射到虚拟机类型
}

void Solution::inputRequest(vector<Request> &vec, string &requestType, string &vmId) {
    string _requestType;
    int _vmId = 0;
    for (int i = 0; i < vmId.size() - 1; i++) {
        _vmId = _vmId * 10 + vmId[i] - '0';
    }
    _requestType = requestType.substr(1, requestType.size() - 2);
    Request req = {_requestType, "", _vmId};
    vec.push_back(req);
}

void Solution::inputVm(int index, string &vmTypeName, string &cpus, string &memory, string &isDouble) {
    string _vmTypeName;
    for (int i = 1; i < vmTypeName.size() - 1; i++) {
        _vmTypeName += vmTypeName[i];
    }

    int _cpus = 0, _memory = 0, _isDouble = 0;
    for (int i = 0; i < cpus.size() - 1; i++) {
        _cpus = _cpus * 10 + cpus[i] - '0';
    }
    for (int i = 0; i < memory.size() - 1; i++) {
        _memory = _memory * 10 + memory[i] - '0';
    }
    if (isDouble[0] == '1') {
        _isDouble = 1;
    } else {
        _isDouble = 0;
    }
    VMType t = {_vmTypeName, _cpus, _memory, _isDouble};
    this->mVMType[index] = t;
    this->mMVTypeMap[_vmTypeName] = t;
}

void Solution::inputServer(int index, string &serverTypeName, string &cpus, string &memory, string &hardCost,
                           string &energyCost) {
    string _serverTypeName;
    for (int i = 1; i < serverTypeName.size() - 1; i++) {
        _serverTypeName += serverTypeName[i];
    }
    int _cpus = 0, _memory = 0, _hardCost = 0, _energyCost = 0;

    for (int i = 0; i < cpus.size() - 1; i++) {
        _cpus = 10 * _cpus + cpus[i] - '0';
    }
    for (int i = 0; i < memory.size() - 1; i++) {
        _memory = 10 * _memory + memory[i] - '0';
    }
    for (int i = 0; i < hardCost.size() - 1; i++) {
        _hardCost = 10 * _hardCost + hardCost[i] - '0';
    }
    for (int i = 0; i < energyCost.size() - 1; i++) {
        _energyCost = 10 * _energyCost + energyCost[i] - '0';
    }

    ServerType t = {_serverTypeName, _cpus, _memory, _hardCost, _energyCost};
    this->mServerType[index] = t;
    this->mServerTypeByPercent[index] = t;
    this->mServerTypeMap[_serverTypeName] = t;
}