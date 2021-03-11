//
// Created by zcz on 2021/3/11.
//

#include "Solution.h"

#include <string>
#include <iostream>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
using namespace std;

Solution::Solution(string &filename){
    this->mFilename = filename;
}

void Solution::input() {
    //服务器类型,请求类型
    string serverType,requestType;
    //cpu核数,内存,硬件成本，能耗成本，是否双结点部署
    int cpus = 0, memory = 0, hardCost = 0, energyCost = 0,isDouble = 0,id=0;

    int fd = open(this->mFilename.c_str(), O_RDONLY);
    int buf_len = 40 * 1024 * 1024;  // 定义buffer大小
    char *buf = (char *) mmap(NULL, buf_len, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);

    char *currentChar = buf;
    int currentNum = 0;
    string curStr = "";

    /**
     * 购买服务器序列
     *
     * */
    int serverTypeNum;      //服务器类型数量
    do {
        currentNum = 10 * currentNum + *currentChar - '0'; // 每次取出数字第一位
        currentChar++;
    } while (*currentChar != '\n'); // 前两个数字以逗号结尾
    serverTypeNum = currentNum;
    currentNum = 0, currentChar++; //跳过换行
    cout << serverTypeNum << endl;

    for(int i=0;i<serverTypeNum;i++){
        curStr = "";
        // 读取 serverType
        do {
            if(*currentChar != '(') curStr += *currentChar; // 每次取出数字第一位
            currentChar++;
        } while (*currentChar != ','); // 前两个数字以逗号结尾
        serverType = curStr;
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
        } while (*currentChar != ','); // windows 数据集以\r 结尾 linux以 \n结尾
        hardCost = currentNum;
        currentNum = 0, currentChar+=2; // 数据集比较奇怪 以\r\n结尾；+2跳过\n

        // 读取 energyCost
        do {
            if(*currentChar != ')')
                currentNum = 10 * currentNum + *currentChar - '0';
            currentChar++;
        } while (*currentChar != '\n');
        energyCost = currentNum;
        currentNum = 0, currentChar++;

        // cout << serverType << "," << cpus << "," << memory << "," << hardCost << "," << energyCost << endl;
    }

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
        // 读取 serverType
        do {
            if(*currentChar != '(') curStr += *currentChar; // 每次取出数字第一位
            currentChar++;
        } while (*currentChar != ','); // 前两个数字以逗号结尾
        serverType = curStr;
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
        //cout << serverType<<" "<<cpus << " " << memory <<" " << isDouble <<endl;
    }

    /**
     * 请求序列
     *
     * */
    int days;    //天数
    do {
        currentNum = 10 * currentNum + *currentChar - '0'; // 每次取出数字第一位
        currentChar++;
    } while (*currentChar != '\n'); // 前两个数字以逗号结尾
    days = currentNum;
    currentNum = 0, currentChar++; //跳过换行
    //cout << days << endl;

    for (int i = 0; i < days; ++i) {
        int requestNum;  //当天请求数
        do {
            currentNum = 10 * currentNum + *currentChar - '0'; // 每次取出数字第一位
            currentChar++;
        } while (*currentChar != '\n'); // 前两个数字以逗号结尾
        requestNum = currentNum;
        currentNum = 0, currentChar++; //跳过换行
        //cout << requestNum << endl;

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
                // 读取 serverType
                do {
                    curStr += *currentChar; // 每次取出数字第一位
                    currentChar++;
                } while (*currentChar != ','); // 前两个数字以逗号结尾
                serverType = curStr;
                currentNum = 0, currentChar+=2;
            }

            // 读取服务器 id
            do {
                currentNum = 10 * currentNum + *currentChar - '0';
                currentChar++;
            } while (*currentChar != ')');
            id = currentNum;
            currentNum = 0, currentChar+=2;

            //cout << requestType << " " << id <<endl;
        }
    }
}

void Solution::run() {
    this->input();
}

void Solution::write_answer() {

}
