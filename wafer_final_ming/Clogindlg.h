#pragma once
#include "afxdialogex.h"


// Clogindlg 对话框

class Clogindlg : public CDialogEx
{
	DECLARE_DYNAMIC(Clogindlg)

public:
	Clogindlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Clogindlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 用户登录用户名
	CString m_user;
	CFont hhhhhhh2;
	// 登录密码
	CString m_pwd;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedLoginIn();
	afx_msg void OnBnClickedLoginOut();
	afx_msg void OnClose();
	virtual void OnOK();
};
