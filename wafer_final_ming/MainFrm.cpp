
// MainFrm.cpp: CMainFrame 类的实现
//

#include "pch.h"
#include "framework.h"
#include "wafer_final_ming.h"
#include "CDispalyView.h"
#include "CSelectView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
int height = 550;
int row = 1020;
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_MESSAGE(NM_A, OnMyChange)
	ON_MESSAGE(NM_B, OnMyChange)
	ON_MESSAGE(NM_C, OnMyChange)
	ON_COMMAND(ID_32773, &CMainFrame::Onopen)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame() noexcept
{
	// TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	SetClassLong(m_hWnd, GCL_HICON, (LONG)AfxGetApp()->LoadIcon(IDI_WIN_LOGO));
	MoveWindow(0, 0, row, height);//窗口设置大小在这
	CenterWindow();
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式,下面是后来加的
	cs.cx = row;
	cs.cy = height;
	cs.style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类
	// 静态拆分窗口，1行2列，CSplitterWnd::CreateStatic
	//CSETM->OnInitialUpdate();
	//CGETM->OnInitialUpdate();
	//CMOVM->OnInitialUpdate();

	m_spliter.CreateStatic(this, 1,4);
	m_spliter.CreateView(0, 0, RUNTIME_CLASS(CSelectView), CSize(200, height), pContext);
	m_spliter.CreateView(0, 1, RUNTIME_CLASS(CGET), CSize(row-200, height), pContext);
	m_spliter.CreateView(0, 2, RUNTIME_CLASS(CMOV), CSize(0, height), pContext);
	m_spliter.CreateView(0, 3, RUNTIME_CLASS(CSET), CSize(0, height), pContext);
	CSET1 = (CSET*)m_spliter.GetPane(0, 3);
	CenterWindow(CSET1);
	CMOV1 = (CMOV*)m_spliter.GetPane(0, 2);
	CenterWindow(CMOV1);
	CGET1 = (CGET*)m_spliter.GetPane(0, 1);
	CenterWindow(CGET1);
	//pContext->m_pNewViewClass = RUNTIME_CLASS(CSET);
	//pContext->m_pCurrentFrame = this;
	//pContext->m_pLastView = (CFormView*)m_spliter.GetPane(0, 1);
	//m_spliter.CreateView(0, 1, CSETM, CSize(950, 800), pContext);

	//m_spliter.CreateView(0, 1, RUNTIME_CLASS(CGET), CSize(950, 800), pContext);
	//m_spliter.CreateView(0, 1, RUNTIME_CLASS(CMOV), CSize(950, 800), pContext);
	//return CFrameWnd::OnCreateClient(lpcs, pContext);
	return true;
}
LRESULT CMainFrame::OnMyChange(WPARAM wParam, LPARAM lParam) {
	CCreateContext   Context;
	switch (wParam)
	{
	case NM_A:
		m_spliter.SetColumnInfo(1, row - 200, row - 200 -500);
		m_spliter.SetColumnInfo(2, 0, 0);
		m_spliter.SetColumnInfo(3, 0, 0);
		
		//m_spliter.GetPane(0, 1);
		//m_spliter.SetColumnInfo(4, 0, 0);
		m_spliter.RecalcLayout();
		//CSETM.ShowWindow(SW_SHOW);
		break;
	case NM_B:
		m_spliter.SetColumnInfo(1, 0, 0);
		m_spliter.SetColumnInfo(2, row - 200, row - 200 -500);
		m_spliter.SetColumnInfo(3, 0, 0);
		//m_spliter.SetColumnInfo(4, 0, 0);

		m_spliter.RecalcLayout();
		//m_spliter.GetPane(0, 1)->ShowWindow(HIDE_WINDOW);
		break;
	case NM_C:
		m_spliter.SetColumnInfo(1, 0, 0);
		m_spliter.SetColumnInfo(2, 0, 0);
		m_spliter.SetColumnInfo(3, row - 200, row - 200 -500);

		m_spliter.RecalcLayout();
		break;
	default:
		MessageBox(_T("error"));
	}
	return 0;
}


/*
		Context.m_pNewViewClass = RUNTIME_CLASS(CSET);
		Context.m_pCurrentFrame = this;
		Context.m_pLastView = (CFormView*)m_spliter.GetPane(0, 1);
		m_spliter.DeleteView(0, 1);
		m_spliter.CreateView(0, 1, RUNTIME_CLASS(CSET), CSize(950, 800), &Context);
		{
			CSET* pNewView = (CSET*)m_spliter.GetPane(0, 1);
		}
		m_spliter.RecalcLayout();
		//pNewView->OnInitialUpdate();
		m_spliter.SetActivePane(0, 1);
*/

void CMainFrame::Onopen()
{
	CBitmap bitmap;  // CBitmap对象，用于加载位图   
	HBITMAP hBmp;    // 保存CBitmap加载的位图的句柄   

	bitmap.LoadBitmap(IDB_BITMAP3);  // 将位图IDB_BITMAP1加载到bitmap  
	hBmp = (HBITMAP)bitmap.GetSafeHandle();  // 获取bitmap加载位图的句柄  
	CMOV1->m_picture.SetBitmap(hBmp);    // 设置图片控件m_Picture的位图图片为IDB_BITMAP1
	// TODO: 在此添加命令处理程序代码

}
