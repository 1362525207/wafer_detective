// CDispalyView.cpp: 实现文件
//

#include "pch.h"
#include "wafer_final_ming.h"
#include "CDispalyView.h"


// CDispalyView

IMPLEMENT_DYNCREATE(CDispalyView, CFormView)

CDispalyView::CDispalyView()
	: CFormView(IDD_DISPALYVIEW)
{

}

CDispalyView::~CDispalyView()
{
}

void CDispalyView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDispalyView, CFormView)
END_MESSAGE_MAP()


// CDispalyView 诊断

#ifdef _DEBUG
void CDispalyView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDispalyView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDispalyView 消息处理程序
