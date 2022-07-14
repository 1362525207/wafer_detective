#pragma once
#include <afxwin.h>
#include "machinecloadport.h"
#include "machinecrobot.h"
#include "WaferLogic.h"
//总面向
class Wafer_Control :
    public CCmdTarget
{
public:
    //构造函数实现类的聚合
    Wafer_Control() {}
        //可以实现类的聚合
    Wafer_Control(machinecrobot* _wafer_control_robot
                , machinecrobot* _wafer_control_align
                , machinecloadport* _wafer_control_lp1
                , machinecloadport* _wafer_control_lp2
                , wafer_json* _jsontext
                , Wafer_Pgsql* _wafer_pgsql)
        :wafer_control_lp1(_wafer_control_lp1)
         , wafer_control_lp2(_wafer_control_lp2)
         , wafer_control_robot(_wafer_control_robot)
         , wafer_control_align(_wafer_control_align)
         , jsontext(_jsontext) , wafer_pgsql(_wafer_pgsql)
    {
        islp_connect = true;
        istcp_connect = true;
    }
    ~Wafer_Control() {
        islp_connect = false;
        istcp_connect = false;
    }
    //初始化函数等控制函数
    bool wafercontrol_con_init();//表示后台开始工作，让所有的对象初始化
    bool wafercontrol_con_serialcon(int port,string com);//让所有的串口连接
    bool wafercontrol_con_tcpcon();//让所有的网口连接
    bool wafercontrol_con_fsmrestart();//让状态机更新
    bool wafercontrol_con_fsmconver();//状态机更换状态


    //设置函数
    bool wafercontrol_set_serialport(string com1,string com2,int br);//设置串口信息包括端口和波特率
    bool wafercontrol_set_tcpipport(string ip,int port1,int port2);//设置网口的连接ip和串口
    bool wafercontrol_set_robotvel();//设置机械臂速度
    bool wafercontrol_set_alirad(int rad);//设置寻位角度
    bool wafercontrol_set_errorclc();//所有的错误清除
    bool wafercontrol_set_destcassage(string cas);//设置目标晶圆盒
    bool wafercontrol_set_srccassage(string cas);//设置源晶圆盒

    //读取函数
    bool wafercontrol_get_lpison();//获得晶圆盒子是否已经安上了
    string wafercontrol_get_mappinged(string cas);//获得mapping信息
    int wafercontrol_get_movevel();//获得运动速度
    string wafercontrol_get_nowstatus(int blade);//获得现在的状态
    string wafercontrol_get_nextstatus();//获得将来的状态
    string wafercontrol_get_destcassage();//获得目标晶圆盒
    string wafercontrol_get_srccassage();//获得源晶圆盒
    int wafercontrol_get_alirad();//获得寻位角度
    bool wafercontrol_getsourceslot();//获得源盒子是否还有片
    bool wafercontrol_getdestcaslot();//获得目标盒是否还满片

    //运动函数
    bool wafercontrol_mov_allinit();//运动的初始化
    bool wafercontrol_mov_allfor1();//一次完整的循环一步
    bool wafercontrol_mov_align(int blade);//寻位
    bool wafercontrol_mov_2mappinged(int blade);//lp移动到门开启
    bool wafercontrol_mov_2lpinit(int blade);//lp移动关闭位置
    bool wafercontrol_mov_getfromlp(int blade);//机械臂从lp取片
    bool wafercontrol_mov_getfromalig(int blade);//机械臂从align取片
    bool wafercontrol_mov_put2alig(int blade);//机械臂align放片
    bool wafercontrol_mov_put2lp(int blade);//机械臂lp放片
    bool wafercontrol_mov_onestep(int blade);//单步测试
public:
    wafer_json* jsontext = nullptr;//json操作对象
    Wafer_Pgsql* wafer_pgsql = nullptr;//数据库操作对象

private:
    //IO设备,启动的时候便定义好了这个对象，不对外开放了

    //定义状态机，所有的动作基于此进行
    FSMworkflow* wafer_control_fsm_blade1 = new FSMworkflow(1);
    FSMworkflow* wafer_control_fsm_blade2 = new FSMworkflow(2);
    unordered_map<int,WaferStatus*> wafer_control_status1;//状态池1进行管理
    unordered_map<int,WaferStatus*> wafer_control_status2;//状态池2进行管理
    //所有的运动设备
    machinecloadport* wafer_control_lp1 = nullptr;//lp设备
    machinecloadport* wafer_control_lp2 = nullptr;
    vector< machinecloadport*> nowlp;
    vector< machinecloadport*> * alllp = nullptr;//lp设备管理指针池
    machinecrobot* wafer_control_robot = nullptr;//robot
    machinecrobot* wafer_control_align = nullptr;//aligner

    bool islp_connect = false;
    bool istcp_connect = false;
    bool isthisinit = false;
    //上述是所有的状态代码
    //要建立一个状态池并动态维护
};

