// CGET.cpp: 实现文件
//

#include "pch.h"
#include "wafer_final_ming.h"
#include "CGET.h"


// CGET

IMPLEMENT_DYNCREATE(CGET, CFormView)

CGET::CGET()
	: CFormView(IDD_GET)
	, destrad(_T(""))
	, iswork(_T(""))
	, nowstatus(_T(""))
	, nowstatus2(_T(""))
	, nowvel2(_T(""))
	, nowvel(_T(""))
{

}

CGET::~CGET()
{
}

void CGET::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, lp1status);
	DDX_Control(pDX, IDC_LIST1, lp2status);
	DDX_Text(pDX, IDC_EDIT3, destrad);
	DDX_Text(pDX, IDC_EDIT1, iswork);
	DDX_Text(pDX, IDC_EDIT2, nowstatus);
	//  DDX_Control(pDX, IDC_EDIT5, nowstatus);
	DDX_Text(pDX, IDC_EDIT4, nowstatus2);
	DDX_Text(pDX, IDC_EDIT6, nowvel2);
	DDX_Text(pDX, IDC_EDIT5, nowvel);
}

BEGIN_MESSAGE_MAP(CGET, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CGET::OnBnClickedButton1)
END_MESSAGE_MAP()


// CGET 诊断

#ifdef _DEBUG
void CGET::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CGET::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CGET 消息处理程序


void CGET::OnInitialUpdate()
{
	//pWnd = GetDlgItem(IDC_STATICget1);
	//hhhhhhh1.CreatePointFont(50, _T("宋体"), NULL);
	//pWnd->SetFont(&hhhhhhh1);
	CFormView::OnInitialUpdate();

	
	lp1status.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	lp2status.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// TODO: 在此添加专用代码和/或调用基类
	lp1status.InsertColumn(0, _T("晶 圆"), LVCFMT_CENTER, 40);
	lp1status.InsertColumn(1, _T("有 无"), LVCFMT_CENTER, 100);
	lp2status.InsertColumn(0, _T("晶 圆"), LVCFMT_CENTER, 40);
	lp2status.InsertColumn(1, _T("有 无"), LVCFMT_CENTER, 100);
	int nRow = 0;
	for (int i = 0; i < 10; i++) {
		CString now = to_string(i).c_str();
		nRow = lp1status.InsertItem(i, now);
		lp1status.SetItemText(nRow, 1, _T("nop"));
		nRow = lp2status.InsertItem(i, now);
		lp2status.SetItemText(nRow, 1, _T("nop"));
	}

	iswork = "no";
	destrad = "00060000";
	nowstatus = "unkown";
	nowstatus2 = "unkown";
	nowvel = "00006000";
	nowvel2 = "00006000";
	UpdateData(FALSE);

}


void CGET::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	string cas1 = theApp.wafercontrol->wafercontrol_get_mappinged("C01");
	for (int i = 0; i < 10; i++) {
		if(cas1[i]=='0') lp1status.SetItemText(i, 1, _T("nop"));
		else lp1status.SetItemText(i, 1, _T("yep"));
		//lp2status.SetItemText(i, 1, _T("nop"));
	}
	cas1 = theApp.wafercontrol->wafercontrol_get_mappinged("C02");
	for (int i = 0; i < 10; i++) {
		if (cas1[i] == '0') lp2status.SetItemText(i, 1, _T("nop"));
		else lp2status.SetItemText(i, 1, _T("yep"));
		//lp2status.SetItemText(i, 1, _T("nop"));
	}

	cas1 = theApp.wafercontrol->wafercontrol_get_nowstatus(1);
	nowstatus = cas1.c_str();
	cas1 = theApp.wafercontrol->wafercontrol_get_nowstatus(2);
	nowstatus2 = cas1.c_str();

	iswork = "yep";
	UpdateData(FALSE);
}
