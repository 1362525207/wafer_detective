#pragma once
#include <string>
#include <afxsock.h>
#include <vector>
using namespace std;
class machinecommon : public CObject
{
public:
	virtual string* act_send(string message)=0;//�ݶ���ÿ���豸���˶�����ָ����Ҫ�豸�����ٴη�װ������ֵ�������ÿ���豸�Ĵ������
	virtual string* read_send(string message)=0;
	virtual bool set_db_parameter(vector<string>, vector<int>)=0;
	virtual int init() = 0;//�豸��ʼ����������ʼ���Ķ�̬
private:

};