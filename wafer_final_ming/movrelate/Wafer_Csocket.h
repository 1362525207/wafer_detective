#pragma once
#include <afxsock.h>
#include <iostream>
#include <string.h>
#include <mutex>
//��ص�ַ
#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS 
using namespace std;
class Wafer_Csocket :
    public CSocket
{
public:
    Wafer_Csocket() {}
    Wafer_Csocket(string _ip, int _port) :ipaddress(_ip), port(_port) {}
    bool Wafer_Csocket_Init();//��ʼ������
    void Wafer_Csocket_Close();//�ر�socket
    bool Wafer_Csocket_Send(string);//������Ϣ
    bool Wafer_Csocket_Receive();//��ȡ��Ϣ
    void OnReceive(int) override;//��д���ܼ���
    string Wafer_Csocket_Getstring();//��ȡ������������
    bool tcp_isopen() { return flag; }
private:
    bool flag = false;
    sockaddr_in addr = {};
    string ipaddress = "10.0.0.2";
    int port = 10110;
    string strrecive;
    mutex mingmutext;
};

