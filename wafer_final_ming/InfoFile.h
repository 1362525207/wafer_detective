#pragma once

#include <list>
#include <fstream>
#include <iostream>
#include <string>

#define _F_LOGIN "./login.ini"
#define _F_STOCK "./stock.txt"

using namespace std;

struct msg
{
	int id;				//��Ʒid
	string name;	//��Ʒ��
	int price;			//��Ʒ�۸�
	int num;			//��Ʒ����
};

class CInfoFile
{
public:
	CInfoFile();
	~CInfoFile();

	//��ȡ��½��Ϣ
	void ReadLogin(CString &name, CString &pwd);

	//�޸�����
	void WritePwd(char* name, char* pwd);

	// ��ȡ��Ʒ����
	void ReadDocline();

	//��Ʒд���ļ�
	void WirteDocline();

	//�������Ʒ
	void Addline(CString name, int num, int price);

	list<msg> ls;	//�洢��Ʒ����
	int num;			//������¼��Ʒ����
};

