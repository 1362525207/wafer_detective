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
	int id;				//商品id
	string name;	//商品名
	int price;			//商品价格
	int num;			//商品个数
};

class CInfoFile
{
public:
	CInfoFile();
	~CInfoFile();

	//读取登陆信息
	void ReadLogin(CString &name, CString &pwd);

	//修改密码
	void WritePwd(char* name, char* pwd);

	// 读取商品数据
	void ReadDocline();

	//商品写入文件
	void WirteDocline();

	//添加新商品
	void Addline(CString name, int num, int price);

	list<msg> ls;	//存储商品容器
	int num;			//用来记录商品个数
};

