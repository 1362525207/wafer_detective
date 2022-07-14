// CSET.cpp: 实现文件
//
#pragma once
#include "pch.h"
#include "wafer_final_ming.h"
#include "CSET.h"


// CSET

IMPLEMENT_DYNCREATE(CSET, CFormView)

CSET::CSET()
	: CFormView(IDD_SETTEXT)
	, ip(_T(""))
	, robot_vel(_T(""))
	, RAD(_T(""))
{

}

CSET::~CSET()
{
}

void CSET::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_EDIT1, ip);
	DDX_Text(pDX, IDC_EDIT1, ip);
	DDX_Control(pDX, IDC_COMBO1, ip_port);
	DDX_Control(pDX, IDC_COMBO8, ip_port1);
	DDX_Control(pDX, IDC_PROGRESS1, progress1);
	DDX_Control(pDX, IDC_PROGRESS2, progress2);
	DDX_Text(pDX, IDC_EDIT3, robot_vel);
	DDX_Control(pDX, IDC_COMBO2, lpcom1);
	DDX_Control(pDX, IDC_COMBO3, lpcom2);
	DDX_Control(pDX, IDC_COMBO4, lpbord);
	DDX_Control(pDX, IDC_COMBO5, SRC);
	DDX_Control(pDX, IDC_COMBO6, DEST);
	DDX_Text(pDX, IDC_EDIT6, RAD);
}

BEGIN_MESSAGE_MAP(CSET, CFormView)
	ON_BN_CLICKED(IDC_BUTTON3, &CSET::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON10, &CSET::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON7, &CSET::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CSET::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CSET::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON5, &CSET::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON4, &CSET::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON6, &CSET::OnBnClickedButton6)
END_MESSAGE_MAP()


// CSET 诊断

#ifdef _DEBUG
void CSET::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CSET::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSET 消息处理程序


void CSET::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ip_port.InsertString(0, "10110");
	ip_port.InsertString(1, "10111");
	ip_port.SetCurSel(0);
	ip_port1.InsertString(0, "10110");
	ip_port1.InsertString(1, "10111");
	ip_port1.SetCurSel(0);
	ip = "192.168.23.1";
	progress1.SetRange(0, 100);
	progress1.SetPos(10);
	progress2.SetRange(0, 100);
	progress2.SetPos(10);
	robot_vel = "00360000";
	lpcom1.InsertString(0, "COM3");
	lpcom1.InsertString(1, "COM4");
	lpcom1.InsertString(2, "COM5");
	lpcom1.SetCurSel(0);
	lpcom2.InsertString(0, "COM3");
	lpcom2.InsertString(1, "COM4");
	lpcom2.InsertString(2, "COM5");
	lpcom2.SetCurSel(1);
	lpbord.InsertString(0, "115200");
	lpbord.SetCurSel(0);
	SRC.InsertString(0, "C01");SRC.InsertString(1, "C02");
	SRC.InsertString(2, "C03");SRC.InsertString(3, "C04");
	SRC.SetCurSel(0);
	DEST.InsertString(0, "C01"); DEST.InsertString(1, "C02");
	DEST.InsertString(2, "C03"); DEST.InsertString(3, "C04");
	DEST.SetCurSel(1);
	RAD = "00060000";
	UpdateData(FALSE);
	// TODO: 在此添加专用代码和/或调用基类
}


void CSET::OnBnClickedButton3()
{
	UpdateData(TRUE);
	string ipnow = ip.GetBuffer();
	int nIndex = ip_port.GetCurSel();
	CString strCBText;
	ip_port.GetLBText(nIndex, strCBText);
	string port = strCBText.GetBuffer();
	int portnow = stoi(port);
	theApp.robot1 = new machinecrobot(1, theApp.wafer_pgsql,theApp.jsontext, ipnow, portnow);
	progress1.SetPos(100);
	//tcp连接
}


void CSET::OnBnClickedButton10()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	string ipnow = ip.GetBuffer();
	int nIndex = ip_port1.GetCurSel();
	CString strCBText;
	ip_port1.GetLBText(nIndex, strCBText);
	string port = strCBText.GetBuffer();
	int portnow = stoi(port);
	theApp.robot2 = new machinecrobot(2, theApp.wafer_pgsql, theApp.jsontext, ipnow, portnow);
	progress2.SetPos(100);
	//第二个的tcp 的连接
}


void CSET::OnBnClickedButton7()
{
	UpdateData(TRUE);
	int nIndex = lpcom1.GetCurSel();
	CString strCBText;
	lpcom1.GetLBText(nIndex, strCBText);
	string port = strCBText.GetBuffer();
	char* ming = (char*)port.c_str();
	theApp.loadport1 = new machinecloadport(theApp.wafer_pgsql, "COM3");
	// TODO: 在此添加控件通知处理程序代码
}


void CSET::OnBnClickedButton8()
{
	UpdateData(TRUE);
	int nIndex = lpcom2.GetCurSel();
	CString strCBText;
	lpcom2.GetLBText(nIndex, strCBText);
	string port = strCBText.GetBuffer();
	char* ming = (char*)port.c_str();
	theApp.loadport2 = new machinecloadport(theApp.wafer_pgsql, "COM4");
	// TODO: 在此添加控件通知处理程序代码
}


void CSET::OnBnClickedButton9()
{
	//设置相关函数
	UpdateData(TRUE);
	int nIndex = SRC.GetCurSel();
	CString strCBText;
	SRC.GetLBText(nIndex, strCBText);
	string port = strCBText.GetBuffer();
	theApp.wafercontrol->wafercontrol_set_srccassage(port);
	nIndex = DEST.GetCurSel();
	DEST.GetLBText(nIndex, strCBText);
	port = strCBText.GetBuffer();
	theApp.wafercontrol->wafercontrol_set_destcassage(port);
	// TODO: 在此添加控件通知处理程序代码
}


void CSET::OnBnClickedButton5()
{
	theApp.wafercontrol = new Wafer_Control(theApp.robot1, theApp.robot2,
		theApp.loadport1, theApp.loadport2, theApp.jsontext, theApp.wafer_pgsql);
	bool ans = theApp.wafercontrol->wafercontrol_con_init();
	if (!ans) MessageBox("启动错误！查看原因！");

	// TODO: 在此添加控件通知处理程序代码
}

static UINT ThreadFunc1(LPVOID pParam)
{

	bool ans = false;
	if (theApp.ismovthreadwork == false) {
		theApp.ismovthreadwork = true;
		ans = theApp.wafercontrol->wafercontrol_mov_allinit();
		theApp.ismovthreadwork = false;
	}
	else {
		AfxMessageBox("运动启动失败！");
		return 0;
	}
	if(ans==true) AfxMessageBox("运动结束！");
	return 0;
}

void CSET::OnBnClickedButton4()
{
	bool ans = theApp.wafercontrol->wafercontrol_con_fsmrestart();
	if (!ans) MessageBox("连接失败，无法重置状态机！");
	AfxBeginThread(ThreadFunc1,this, THREAD_PRIORITY_HIGHEST,0);
	//thread now(&(Wafer_Control::wafercontrol_mov_allinit),theApp.wafercontrol);
	//now.detach();
	//ans = theApp.wafercontrol->wafercontrol_mov_allinit();
	// TODO: 在此添加控件通知处理程序代码
	//if (!ans) MessageBox("连接失败，无法进入运动状态！");
}


void CSET::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
}
