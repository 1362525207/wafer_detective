#include "pch.h"
#include "Wafer_Pgsql.h"
bool Wafer_Pgsql::wafer_pgsql_ConnectToDB() {
	//bool ans=wafer_db.Open(_T("PostgreSQL35W"), false, false, _T("ODBC;UID=postgres;PWD=MHQD0617"));
	bool ans = wafer_db.Open(_T("postgres1"),    false,    false,    _T("ODBC;UID=postgres;PWD=mhqd0617"));
	wafer_dbrs = new CRecordset(&wafer_db);//绑定数据库
	return ans;
}
void Wafer_Pgsql::wafer_pgsql_Excsql(string terminao) {
	CString lan(terminao.c_str());
	wafer_db.ExecuteSQL(lan);
}//执行sql语句
void Wafer_Pgsql::wafer_pgsql_CloseDB() {
	wafer_dbrs->Close();//关闭记录集
	wafer_db.Close();//关闭数据库的连接
}//关闭数据库
void Wafer_Pgsql::wafer_pgsql_select() {//先测试一个
	try
	{//获得该表格下的所有参数
		CString strSQL = _T("SELECT * FROM cloadport_conv");	// 组织SQL语句
		wafer_dbrs->Open(CRecordset::forwardOnly, strSQL, CRecordset::readOnly); // 用SQL查询数据库
		CString* psd;
		short nFields = wafer_dbrs->GetODBCFieldCount();//返回字段数
//（记录集如果定位在最后的话，会返回0，详情请看CRecordset的MSDN说明）
		CString nowvalue;
		while (!wafer_dbrs->IsEOF())
		{//不断循环,直到查询完成
			for (short index = 0; index < nFields; index++)
			{//将字段中的值取出 详情请看CDBVariant类成员变量介绍
				wafer_dbrs->GetFieldValue(index, nowvalue);
			}
			wafer_dbrs->MoveNext();
		}
	}//下面是各种异常处理
	catch (CMemoryException* e){}
	catch (CFileException* e){}
	catch (CException* e){}
}
void Wafer_Pgsql::wafer_pgsql_select(string ans) {//目前是更换了数据集
	if (wafer_dbrs->IsOpen()) {
		wafer_dbrs->Close();
	}
	CString strSQL(ans.c_str());
	wafer_dbrs->Open(CRecordset::dynaset, strSQL, CRecordset::none);
}
bool Wafer_Pgsql::wafer_pgsql_requery() {//刷新数据库
	return wafer_dbrs->Requery();
}

bool Wafer_Pgsql_Record::wafer_pgsql_BindToDB() {
	this->wafer_dbrs = new CRecordset(this->this_wafer_db);//绑定数据库
	return true;//wafer_dbrs->IsOpen();
}//绑定数据库
void Wafer_Pgsql_Record::wafer_pgsql_CloseBIND(){
	this->wafer_dbrs->Close();
}//关闭该数据集与数据库的绑定
void Wafer_Pgsql_Record::wafer_pgsql_Excsql(string ans) {
	CString lan(ans.c_str());
	this->this_wafer_db->ExecuteSQL(lan);
}//执行sql语句
void Wafer_Pgsql_Record::wafer_pgsql_select() {
	try
	{//获得该表格下的所有参数
		CString strSQL(this->thisselct_sql.c_str());	// 组织SQL语句
		wafer_dbrs->Open(CRecordset::forwardOnly, strSQL, CRecordset::readOnly); // 用SQL查询数据库
		CString* psd;
		short nFields = wafer_dbrs->GetODBCFieldCount();//返回字段数
//（记录集如果定位在最后的话，会返回0，详情请看CRecordset的MSDN说明）
		CString nowvalue;
		while (!wafer_dbrs->IsEOF())
		{//不断循环,直到查询完成
			for (short index = 0; index < nFields; index++)
			{//将字段中的值取出 详情请看CDBVariant类成员变量介绍
				wafer_dbrs->GetFieldValue(index, nowvalue);
			}
			wafer_dbrs->MoveNext();
		}
	}//下面是各种异常处理
	catch (CMemoryException* e) {}
	catch (CFileException* e) {}
	catch (CException* e) {}
}//获得数据集
string Wafer_Pgsql_Record::wafer_pgsql_select(string ans) {
	if (wafer_dbrs->IsOpen()) {
		wafer_dbrs->Close();
	}
	CString strSQL(ans.c_str());
	wafer_dbrs->Open(CRecordset::forwardOnly, strSQL, CRecordset::readOnly);
	short nFields = wafer_dbrs->GetODBCFieldCount();//返回字段数
	CString nowvalue;
	string ans1;
		for (short index = 0; index < nFields; index++)
		{//将字段中的值取出 详情请看CDBVariant类成员变量介绍
			wafer_dbrs->GetFieldValue(index, nowvalue);
			ans1 += CT2A(nowvalue.GetString());
		}
		wafer_dbrs->Close();
	return ans1;
}//获得指定内容的数据集
vector<string> Wafer_Pgsql_Record::wafer_pgsql_select_row(string ans) {
	vector<string> ans1;
	if (wafer_dbrs->IsOpen()) {
		wafer_dbrs->Close();
	}
	CString strSQL(ans.c_str());
	wafer_dbrs->Open(CRecordset::forwardOnly, strSQL, CRecordset::readOnly);
	short nFields = wafer_dbrs->GetODBCFieldCount();//返回字段数
	CString nowvalue;
	string ans2;
	for (short index = 0; index < nFields; index++)
	{//将字段中的值取出 详情请看CDBVariant类成员变量介绍
		wafer_dbrs->GetFieldValue(index, nowvalue);
		ans2 = CT2A(nowvalue.GetString());
		ans1.push_back(ans2);
	}
	wafer_dbrs->Close();
	return ans1;
}
bool Wafer_Pgsql_Record::wafer_pgsql_requery() {
	return wafer_dbrs->Requery();
}