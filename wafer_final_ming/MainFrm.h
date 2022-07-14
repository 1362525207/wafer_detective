
// MainFrm.h: CMainFrame 类的接口
//

#pragma once
#include "CSET.h"
#include "CMOV.h"
#include "CGET.h"

//WM_USER+100是为了区分系统消息和用户消息，避免冲突



#define NM_A	(WM_USER + 100)
#define NM_B	(WM_USER + 101)
#define NM_C	(WM_USER + 102)
#define NM_D	(WM_USER + 103)
class CMainFrame : public CFrameWnd
{
	
protected: // 仅从序列化创建
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)

// 特性
public:
	CGET* CGET1;
	CSET* CSET1;
	CMOV* CMOV1;
// 操作
public:

	//machinecrobot* robot1 = new machinecrobot(1, wafer_pgsql, jsontext, "192.168.23.1", 10110);//模拟机械臂
	//machinecrobot* robot2 = new machinecrobot(2, wafer_pgsql, jsontext, "192.168.23.1", 10111);//模拟寻位
	
// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
private:
	CSplitterWnd m_spliter; // 切分窗口类对象

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	afx_msg LRESULT OnMyChange(WPARAM wParam, LPARAM lParam);
protected:  // 控件条嵌入成员
	CStatusBar  m_wndStatusBar;
// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	afx_msg void Onopen();
};


