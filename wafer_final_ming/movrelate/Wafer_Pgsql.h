#pragma once
#include <afxdb.h>
#include <iostream>
#include <string.h>
#include <vector>
using namespace std;
class Wafer_Pgsql_Record;
class Wafer_Pgsql : 
    public CObject //��CDB��CREһ���������
{//https://blog.csdn.net/nonecode/article/details/7939693 ������ϸ�鿴������
public:
	CDatabase wafer_db;
	bool wafer_pgsql_ConnectToDB();//�������ݿ�
	void wafer_pgsql_Excsql(string);//ִ��sql���
	void wafer_pgsql_CloseDB();//�ر����ݿ�
	void wafer_pgsql_select();//������ݼ�
	void wafer_pgsql_select(string);//���ָ�����ݵ����ݼ�
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
	bool wafer_pgsql_BindToDB();//�����ݿ�
	void wafer_pgsql_Excsql(string);//ִ��sql���
	void wafer_pgsql_CloseBIND();//�ر������ݿ������
	void wafer_pgsql_select();//������ݼ�
	string wafer_pgsql_select(string);//���ָ�����ݵ����ݼ�
	vector<string> wafer_pgsql_select_row(string);//���ָ�����ݵ����ݼ�
	bool wafer_pgsql_requery();
protected:
	string thisselct_sql="select * from conv1";
	CDatabase* this_wafer_db=nullptr;
	CRecordset* wafer_dbrs = nullptr;
};