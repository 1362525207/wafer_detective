#pragma once
#include <afxdb.h>
#include <iostream>
#include <string.h>
#include <vector>
using namespace std;
class Wafer_Pgsql_Record;
class Wafer_Pgsql : 
    public CObject //与CDB和CRE一个基本类层
{//https://blog.csdn.net/nonecode/article/details/7939693 可以详细查看该连接
public:
	CDatabase wafer_db;
	bool wafer_pgsql_ConnectToDB();//连接数据库
	void wafer_pgsql_Excsql(string);//执行sql语句
	void wafer_pgsql_CloseDB();//关闭数据库
	void wafer_pgsql_select();//获得数据集
	void wafer_pgsql_select(string);//获得指定内容的数据集
	bool wafer_pgsql_requery();
private:
	CRecordset* wafer_dbrs=nullptr;
};

class Wafer_Pgsql_Record :
	public CObject
{
public:
	Wafer_Pgsql_Record(CDatabase* wafer_db):this_wafer_db(wafer_db) {}
	Wafer_Pgsql_Record(CDatabase* wafer_db,string ans) :
		this_wafer_db(wafer_db),thisselct_sql(ans) {}
	bool wafer_pgsql_BindToDB();//绑定数据库
	void wafer_pgsql_Excsql(string);//执行sql语句
	void wafer_pgsql_CloseBIND();//关闭与数据库的连接
	void wafer_pgsql_select();//获得数据集
	string wafer_pgsql_select(string);//获得指定内容的数据集
	vector<string> wafer_pgsql_select_row(string);//获得指定内容的数据集
	bool wafer_pgsql_requery();
protected:
	string thisselct_sql="select * from conv1";
	CDatabase* this_wafer_db=nullptr;
	CRecordset* wafer_dbrs = nullptr;
};