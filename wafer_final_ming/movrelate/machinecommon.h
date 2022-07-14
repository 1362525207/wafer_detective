#pragma once
#include <string>
#include <afxsock.h>
#include <vector>
using namespace std;
class machinecommon : public CObject
{
public:
	virtual string* act_send(string message)=0;//暂定：每个设备的运动发送指令需要设备内容再次封装，返还值是相对于每个设备的错误编码
	virtual string* read_send(string message)=0;
	virtual bool set_db_parameter(vector<string>, vector<int>)=0;
	virtual int init() = 0;//设备初始化动作，初始化的多态
private:

};