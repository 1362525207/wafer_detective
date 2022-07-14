#pragma once
#include <afxsock.h>
#include <iostream>
#include <string.h>
#include <mutex>
//相关地址
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS 
using namespace std;
class Wafer_Csocket :
    public CSocket
{
public:
    Wafer_Csocket() {}
    Wafer_Csocket(string _ip, int _port) :ipaddress(_ip), port(_port) {}
    bool Wafer_Csocket_Init();//初始化函数
    void Wafer_Csocket_Close();//关闭socket
    bool Wafer_Csocket_Send(string);//发送消息
    bool Wafer_Csocket_Receive();//获取信息
    void OnReceive(int) override;//重写接受监听
    string Wafer_Csocket_Getstring();//获取缓冲区的内容
    bool tcp_isopen() { return flag; }
private:
    bool flag = false;
    sockaddr_in addr = {};
    string ipaddress = "10.0.0.2";
    int port = 10110;
    string strrecive;
    mutex mingmutext;
};

