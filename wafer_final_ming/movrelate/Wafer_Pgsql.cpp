#include "pch.h"
#include "Wafer_Pgsql.h"
bool Wafer_Pgsql::wafer_pgsql_ConnectToDB() {
	//bool ans=wafer_db.Open(_T("PostgreSQL35W"), false, false, _T("ODBC;UID=postgres;PWD=MHQD0617"));
	bool ans = wafer_db.Open(_T("postgres1"),    false,    false,    _T("ODBC;UID=postgres;PWD=mhqd0617"));
	wafer_dbrs = new CRecordset(&wafer_db);//�����ݿ�
	return ans;
}
void Wafer_Pgsql::wafer_pgsql_Excsql(string terminao) {
	CString lan(terminao.c_str());
	wafer_db.ExecuteSQL(lan);
}//ִ��sql���
void Wafer_Pgsql::wafer_pgsql_CloseDB() {
	wafer_dbrs->Close();//�رռ�¼��
	wafer_db.Close();//�ر����ݿ������
}//�ر����ݿ�
void Wafer_Pgsql::wafer_pgsql_select() {//�Ȳ���һ��
	try
	{//��øñ���µ����в���
		CString strSQL = _T("SELECT * FROM cloadport_conv");	// ��֯SQL���
		wafer_dbrs->Open(CRecordset::forwardOnly, strSQL, CRecordset::readOnly); // ��SQL��ѯ���ݿ�
		CString* psd;
		short nFields = wafer_dbrs->GetODBCFieldCount();//�����ֶ���
//����¼�������λ�����Ļ����᷵��0�������뿴CRecordset��MSDN˵����
		CString nowvalue;
		while (!wafer_dbrs->IsEOF())
		{//����ѭ��,ֱ����ѯ���
			for (short index = 0; index < nFields; index++)
			{//���ֶ��е�ֵȡ�� �����뿴CDBVariant���Ա��������
				wafer_dbrs->GetFieldValue(index, nowvalue);
			}
			wafer_dbrs->MoveNext();
		}
	}//�����Ǹ����쳣����
	catch (CMemoryException* e){}
	catch (CFileException* e){}
	catch (CException* e){}
}
void Wafer_Pgsql::wafer_pgsql_select(string ans) {//Ŀǰ�Ǹ��������ݼ�
	if (wafer_dbrs->IsOpen()) {
		wafer_dbrs->Close();
	}
	CString strSQL(ans.c_str());
	wafer_dbrs->Open(CRecordset::dynaset, strSQL, CRecordset::none);
}
bool Wafer_Pgsql::wafer_pgsql_requery() {//ˢ�����ݿ�
	return wafer_dbrs->Requery();
}

bool Wafer_Pgsql_Record::wafer_pgsql_BindToDB() {
	this->wafer_dbrs = new CRecordset(this->this_wafer_db);//�����ݿ�
	return true;//wafer_dbrs->IsOpen();
}//�����ݿ�
void Wafer_Pgsql_Record::wafer_pgsql_CloseBIND(){
	this->wafer_dbrs->Close();
}//�رո����ݼ������ݿ�İ�
void Wafer_Pgsql_Record::wafer_pgsql_Excsql(string ans) {
	CString lan(ans.c_str());
	this->this_wafer_db->ExecuteSQL(lan);
}//ִ��sql���
void Wafer_Pgsql_Record::wafer_pgsql_select() {
	try
	{//��øñ���µ����в���
		CString strSQL(this->thisselct_sql.c_str());	// ��֯SQL���
		wafer_dbrs->Open(CRecordset::forwardOnly, strSQL, CRecordset::readOnly); // ��SQL��ѯ���ݿ�
		CString* psd;
		short nFields = wafer_dbrs->GetODBCFieldCount();//�����ֶ���
//����¼�������λ�����Ļ����᷵��0�������뿴CRecordset��MSDN˵����
		CString nowvalue;
		while (!wafer_dbrs->IsEOF())
		{//����ѭ��,ֱ����ѯ���
			for (short index = 0; index < nFields; index++)
			{//���ֶ��е�ֵȡ�� �����뿴CDBVariant���Ա��������
				wafer_dbrs->GetFieldValue(index, nowvalue);
			}
			wafer_dbrs->MoveNext();
		}
	}//�����Ǹ����쳣����
	catch (CMemoryException* e) {}
	catch (CFileException* e) {}
	catch (CException* e) {}
}//������ݼ�
string Wafer_Pgsql_Record::wafer_pgsql_select(string ans) {
	if (wafer_dbrs->IsOpen()) {
		wafer_dbrs->Close();
	}
	CString strSQL(ans.c_str());
	wafer_dbrs->Open(CRecordset::forwardOnly, strSQL, CRecordset::readOnly);
	short nFields = wafer_dbrs->GetODBCFieldCount();//�����ֶ���
	CString nowvalue;
	string ans1;
		for (short index = 0; index < nFields; index++)
		{//���ֶ��е�ֵȡ�� �����뿴CDBVariant���Ա��������
			wafer_dbrs->GetFieldValue(index, nowvalue);
			ans1 += CT2A(nowvalue.GetString());
		}
		wafer_dbrs->Close();
	return ans1;
}//���ָ�����ݵ����ݼ�
vector<string> Wafer_Pgsql_Record::wafer_pgsql_select_row(string ans) {
	vector<string> ans1;
	if (wafer_dbrs->IsOpen()) {
		wafer_dbrs->Close();
	}
	CString strSQL(ans.c_str());
	wafer_dbrs->Open(CRecordset::forwardOnly, strSQL, CRecordset::readOnly);
	short nFields = wafer_dbrs->GetODBCFieldCount();//�����ֶ���
	CString nowvalue;
	string ans2;
	for (short index = 0; index < nFields; index++)
	{//���ֶ��е�ֵȡ�� �����뿴CDBVariant���Ա��������
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