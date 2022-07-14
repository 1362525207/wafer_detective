// CMOV.cpp: 实现文件
//

#include "pch.h"
#include "wafer_final_ming.h"
#include "CMOV.h"


// CMOV

IMPLEMENT_DYNCREATE(CMOV, CFormView)

CMOV::CMOV()
	: CFormView(IDD_MOV)
	, nowstatus(_T(""))
{

}

CMOV::~CMOV()
{
}

void CMOV::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PIC, m_picture);
	DDX_Text(pDX, IDC_EDIT1, nowstatus);
	//  DDX_Radio(pDX, IDC_RADIO1, isalign);
	//  DDX_Radio(pDX, IDC_RADIO2, istext);
	DDX_Control(pDX, IDC_LIST3, listcontrol);
}

BEGIN_MESSAGE_MAP(CMOV, CFormView)
	ON_BN_CLICKED(IDC_BUTTON2, &CMOV::OnBnClickedButton2)
	ON_COMMAND(ID_32773, &CMOV::On32773)
	ON_STN_CLICKED(IDC_PIC, &CMOV::OnStnClickedPic)
	ON_BN_CLICKED(IDC_BUTTON1, &CMOV::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CMOV::OnBnClickedButton3)
	ON_BN_CLICKED(IDOK, &CMOV::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO1, &CMOV::OnBnClickedRadio1)
END_MESSAGE_MAP()


// CMOV 诊断

#ifdef _DEBUG
void CMOV::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMOV::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMOV 消息处理程序


void CMOV::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	nowstatus = "no work start";
	listcontrol.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	listcontrol.InsertColumn(0, _T("序 号"), LVCFMT_CENTER,80);
	listcontrol.InsertColumn(1, _T("LOG 记录"), LVCFMT_CENTER,300);
	int nRow = listcontrol.InsertItem(0, _T("1"));
	listcontrol.SetItemText(nRow, 1, _T(""));
	nRow = listcontrol.InsertItem(1, _T("2"));
	listcontrol.SetItemText(nRow, 1, _T(""));
	nRow = listcontrol.InsertItem(2, _T("3"));
	listcontrol.SetItemText(nRow, 1, _T(""));
	nRow = listcontrol.InsertItem(3, _T("4"));
	listcontrol.SetItemText(nRow, 1, _T(""));
	UpdateData(FALSE);
	// TODO: 在此添加专用代码和/或调用基类
}


static UINT DANBUCESHI(LPVOID PARAM) {
	int now = (int)PARAM;
	bool ans = false;
	if (theApp.ismovthreadwork == false) {
		theApp.ismovthreadwork = true;
		ans = theApp.wafercontrol->wafercontrol_mov_onestep(now);
		theApp.ismovthreadwork = false;
	}
	else {
		AfxMessageBox("单步运动启动失败！");
		return 0;
	}
	if (ans == true) AfxMessageBox("单步运动结束！");
	return 0;
}

void CMOV::OnBnClickedButton2()
{
	//char* la = "crobot_vari.json";//文件设备并绑定json名字
	//wafer_json* jsontext = new wafer_json(la);//json操作对象
	// TODO: 在此添加控件通知处理程序代码
	//theApp.wafercontrol->wafercontrol_mov_onestep(1);
	string now = theApp.wafercontrol->wafercontrol_get_nowstatus(1);
	nowstatus = now.c_str();
	UpdateData(FALSE);
	AfxBeginThread(DANBUCESHI, (LPVOID)1, THREAD_PRIORITY_NORMAL,0);


}


void CMOV::On32773()
{
	// TODO: 在此添加命令处理程序代码
}


void CMOV::OnStnClickedPic()
{
	// TODO: 在此添加控件通知处理程序代码

}


void CMOV::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (isalignhhh == false) theApp.jsontext->setjson_keyvalue("isalign", "0");
	else theApp.jsontext->setjson_keyvalue("isalign", "1");

	theApp.wafercontrol->wafercontrol_con_fsmrestart();
	theApp.wafercontrol->wafercontrol_con_fsmconver();//转到状态1，因为已经初始化了

	nowstatus = "WORK COMPLISH!";
	UpdateData(FALSE);
}

UINT DUOBUCESHI(LPVOID PARAM) {
	int now = (int)PARAM;
	bool ans = false;
	if (theApp.ismovthreadwork == false) {
		theApp.ismovthreadwork = true;
		ans = theApp.wafercontrol->wafercontrol_mov_allfor1();
		theApp.ismovthreadwork = false;
	}
	else {
		AfxMessageBox("多步运动启动失败！");
		return 0;
	}
	if (ans == true) AfxMessageBox("多步运动结束！");
	return 0;
}
void CMOV::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	theApp.wafercontrol->wafercontrol_mov_onestep(1);
	AfxBeginThread(DUOBUCESHI, (LPVOID)1, THREAD_PRIORITY_NORMAL, 0);

	string now = theApp.wafercontrol->wafercontrol_get_nowstatus(1);
	nowstatus = now.c_str();
	UpdateData(FALSE);
}


void CMOV::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	string ans = theApp.loadport1->getsql("SELECT * FROM wafer_log ORDER BY datenow DESC LIMIT 1");
	CString minghh = ans.c_str();
	listcontrol.SetItemText(nowlist, 1, minghh);
	nowlist = (nowlist + 1) % 4;

}


void CMOV::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	isalignhhh = !isalignhhh;
}
