
// wafer_final_ming.h: wafer_final_ming 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含 'pch.h' 以生成 PCH"
#endif

#include "resource.h"       // 主符号
#include "Wafer_Control.h"



// CwaferfinalmingApp:
// 有关此类的实现，请参阅 wafer_final_ming.cpp
//


class CwaferfinalmingApp : public CWinApp
{
public:
	CwaferfinalmingApp() noexcept;
	char* la = "crobot_vari.json";//文件设备并绑定json名字
	wafer_json* jsontext = new wafer_json(la);//json操作对象
	Wafer_Pgsql* wafer_pgsql = new Wafer_Pgsql;//数据库操作对象
	volatile bool ismovthreadwork = false;//标志运动线程不可到达
	machinecrobot* robot1 = nullptr;//机械臂
	machinecrobot* robot2 = nullptr;//寻危机
	//Wafer_Pgsql_Record* controlsql_record = new Wafer_Pgsql_Record(wafer_pgsql);
	machinecloadport* loadport1 = nullptr;
	machinecloadport* loadport2 = nullptr;
	CFont cfontnow;
	//cfont.CreatePointFont(230, _T("宋体"), NULL);
	Wafer_Control* wafercontrol = nullptr;

// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CwaferfinalmingApp theApp;
