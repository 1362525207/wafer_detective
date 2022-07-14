#include "pch.h"
#include "Wafer_Csocket.h"
#include "thread"
extern volatile bool checkOnReceive;
bool Wafer_Csocket::Wafer_Csocket_Init() {
	AfxSocketInit();
	CString ip(ipaddress.c_str());
	this->Create();//经验之谈，有了create和connect对客户端即可
	BOOL ret = this->Connect(ip, port);
	if (!ret)
	{
		CString strErr;
		strErr.Format(_T("tcp connect fail!"));
		AfxMessageBox(strErr);
		return false;
	}
	CString strErr;
	strErr.Format(_T("tcp connect success!"));
	AfxMessageBox(strErr);
	flag = true;
	return true;
}
void Wafer_Csocket::Wafer_Csocket_Close() {
	this->Close();
	flag = false;
}//关闭socket
bool Wafer_Csocket::Wafer_Csocket_Send(string strInput) {
	int err = Send(strInput.c_str(), strInput.length() + 1, 0);
	return err;
}
void Wafer_Csocket::OnReceive(int errorcode) {
	//Sleep(10);
	
	checkOnReceive = true;
	//this->Wafer_Csocket_Receive();
}
bool Wafer_Csocket::Wafer_Csocket_Receive() {
	
	//thread now(Receive,szBuf,1024,0);
	mingmutext.lock();
	//string m = "";
	//string* nowm = &m;
	//int len = Receive(szBuf, 1024, 0);
	//string m;
	char szBuf[1024] = "ming";
	int len = Receive(szBuf, 1024, 0);
	if (len > 0) {
		strrecive = szBuf;
		mingmutext.unlock();
		//delete[] szBuf;
		return true;
	}
	else {
		mingmutext.unlock();
		return false;
	}

}
string Wafer_Csocket::Wafer_Csocket_Getstring() {
	string ans = strrecive;
	strrecive.clear();
	return ans;
}