#pragma once
#include <iostream>
#include <Windows.h>
#include <thread>
#include <mutex>
using namespace std;
class Wafer_Cserial: public CWnd
{
private:
	mutex nowmutex;//锁机制保证了无限监听
	CString serial;
	int pHandle[16];
	char synchronizeflag;
	char buf[1024];
	char* portname = "COM3";
	bool flag=false;
public:
	Wafer_Cserial();
	Wafer_Cserial(char* _portname):portname(_portname){}
	~Wafer_Cserial();
	bool wafer_serial_open(
		int baudrate = 115200, char parity = 0,
		char databit = 8, char stopbit = 1, char synchronizeflag = 0);//波特率别忘了9600
	//关闭串口，参数待定
	void wafer_serial_close();
	//发送数据或写数据，成功返回发送数据长度，失败返回0
	virtual int wafer_serial_Send(string dat);
	//接受数据或读数据，成功返回读取实际数据的长度，失败返回0
	string wafer_serial_receive();
	bool wafer_serial_isopen() {
		return flag;
	}
};
