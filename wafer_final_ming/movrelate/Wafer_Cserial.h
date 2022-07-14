#pragma once
#include <iostream>
#include <Windows.h>
#include <thread>
#include <mutex>
using namespace std;
class Wafer_Cserial: public CWnd
{
private:
	mutex nowmutex;//�����Ʊ�֤�����޼���
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
		char databit = 8, char stopbit = 1, char synchronizeflag = 0);//�����ʱ�����9600
	//�رմ��ڣ���������
	void wafer_serial_close();
	//�������ݻ�д���ݣ��ɹ����ط������ݳ��ȣ�ʧ�ܷ���0
	virtual int wafer_serial_Send(string dat);
	//�������ݻ�����ݣ��ɹ����ض�ȡʵ�����ݵĳ��ȣ�ʧ�ܷ���0
	string wafer_serial_receive();
	bool wafer_serial_isopen() {
		return flag;
	}
};
