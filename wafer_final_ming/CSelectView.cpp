// CSelectView.cpp: 实现文件
//

#include "pch.h"
#include "wafer_final_ming.h"
#include "CSelectView.h"
#include "MainFrm.h"

// CSelectView

IMPLEMENT_DYNCREATE(CSelectView, CTreeView)

CSelectView::CSelectView()
{

}

CSelectView::~CSelectView()
{
}

BEGIN_MESSAGE_MAP(CSelectView, CTreeView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CSelectView::OnTvnSelchanged)
END_MESSAGE_MAP()


// CSelectView 诊断

#ifdef _DEBUG
void CSelectView::AssertValid() const
{
	CTreeView::AssertValid();
}

#ifndef _WIN32_WCE
void CSelectView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif
#endif //_DEBUG


// CSelectView 消息处理程序


void CSelectView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();
	HICON icon = AfxGetApp()->LoadIcon(IDI_ICON1);
	// TODO: 在此添加专用代码和/或调用基类
	//设置竖直节点
	m_imageList.Create(40, 40, ILC_COLOR32, 1, 1);
	m_imageList.Add(icon);

	m_treeCtrl = &GetTreeCtrl();
	//CFont ft;
	//ft.CreateFontA(-30, 5, 0, 0, 400, 0, 0, 0, 134, 3, 2, 1, 2, TEXT("宋体"));
	//m_treeCtrl->SetFont(&ft);
	m_treeCtrl->SetImageList(&m_imageList, TVSIL_NORMAL);
	m_treeCtrl->InsertItem(TEXT("状态监控"), 0, 0, NULL);
	m_treeCtrl->InsertItem(TEXT("运动控制"), 0, 0, NULL);
	m_treeCtrl->InsertItem(TEXT("设备设置"), 0, 0, NULL);

	m_treeCtrl->SetBkColor(RGB(193, 210, 255));
	m_treeCtrl->SetItemHeight(100);
}


void CSelectView::OnTvnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//获取当前节点选中项目 CTreeCtrl::GetSelectedItem
	HTREEITEM item = m_treeCtrl->GetSelectedItem();
	CString str = m_treeCtrl->GetItemText(item);
	if (str == TEXT("状态监控"))
	{
		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_A, (WPARAM)NM_A, (LPARAM)0);
	}
	else if (str == TEXT("运动控制")) {
		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_B, (WPARAM)NM_B, (LPARAM)0);
	}
	else if (str == TEXT("设备设置")) {
		::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), NM_C, (WPARAM)NM_C, (LPARAM)0);
	}

	*pResult = 0;
}
