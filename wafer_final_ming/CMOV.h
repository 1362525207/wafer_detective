#pragma once



// CMOV 窗体视图

class CMOV : public CFormView
{
	DECLARE_DYNCREATE(CMOV)

public:
	CMOV();           // 动态创建所使用的受保护的构造函数
	virtual ~CMOV();
	int nowlist = 0;
public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MOV };
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
	bool isalignhhh = false;
	virtual void OnInitialUpdate();
	afx_msg void OnFileOpen();
	CStatic m_picture;
	afx_msg void OnBnClickedButton2();
	afx_msg void On32773();
	afx_msg void OnStnClickedPic();
	CString nowstatus;
//	BOOL isalign;
//	BOOL istext;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedOk();
	CListCtrl listcontrol;
	afx_msg void OnBnClickedRadio1();
};


