#pragma once



// CGET 窗体视图

class CGET : public CFormView
{
	DECLARE_DYNCREATE(CGET)

public:
	CGET();           // 动态创建所使用的受保护的构造函数
	virtual ~CGET();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GET };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	CListCtrl lp1status;
	CListCtrl lp2status;
	CString destrad;
	CString iswork;
	CString nowstatus;
	//CEdit nowstatus;
	CString nowstatus2;
	CString nowvel2;
	CString nowvel;
	CFont hhhhhhh1;
	CWnd* pWnd = nullptr;
	afx_msg void OnBnClickedButton1();
};


