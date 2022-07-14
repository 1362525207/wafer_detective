#pragma once



// CSET 窗体视图

class CSET : public CFormView
{
	DECLARE_DYNCREATE(CSET)

public:
	CSET();           // 动态创建所使用的受保护的构造函数
	virtual ~CSET();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTEXT };
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
//	string ip;
	afx_msg void OnBnClickedButton3();
	CString ip;
	CComboBox ip_port;
	CComboBox ip_port1;
	afx_msg void OnBnClickedButton10();
	CProgressCtrl progress1;
	CProgressCtrl progress2;
	CString robot_vel;
	CComboBox lpcom1;
	CComboBox lpcom2;
	CComboBox lpbord;
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	CComboBox SRC;
	CComboBox DEST;
	CString RAD;
	CFont hhhhhhh;
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton6();
};


