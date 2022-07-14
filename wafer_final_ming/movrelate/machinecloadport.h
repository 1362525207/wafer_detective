#pragma once
#include "machinecommon.h"
#include <iostream>
#include <string>
#include <vector>
#include "common.h"
using namespace std;

#define COM3 "COM3"

class machinecloadport :
    public machinecommon //该类是LP的相关函数包含
{
public:
    struct lp_act_data {
        string need_status;
        string next_status;
        string lp_command;
        string ack_lp_command="";
        string variable_name;//需要修改的变量
        string fin_lp_command="";
    }LP_ACT_DATA;
    machinecloadport(Wafer_Pgsql* _wafer_pgsql,char* _com):com(_com) {
        thislp_common = new clpcommon(_wafer_pgsql);
    }//实例化的时候需要考虑到数据库绑定定义
    //继承部分或者公共部分
    virtual string* act_send(string message) { return nullptr; }//暂定：每个设备的运动发送指令需要设备内容再次封装，返还值是相对于每个设备的错误编码
    virtual string* read_send(string message) { return nullptr; }
    virtual int init();//重写初始化虚函数，有关发送消息继承在串口类中，S00以及最后的换行符
    bool set_db_parameter(vector<string>, vector<int>) override;
    int machine_action(lp_act_data);//执行相关函数
    int machine_check_action(lp_act_data);
    //设置函数

    bool setlight01ON();//设置警示灯为开
    bool setlight01OFF();//设置警示灯为关
    bool setcassege_exist_ON();//设置晶圆盒存在指示灯为开
    bool setcassege_exist_OFF();//设置晶圆盒存在指示灯为关
    
    //获得命令指令
    int get_lp_state();//获得相关的状态，定义errorcode对应不同的错误
    int get_lp_mapping();//获得晶圆存在信息
    string getsql(string eex) {
        return thislp_common->lpgetfromdb(eex);
    }
    //运动指令
    bool move2dooropened();//运动到开门位置
    bool move2mappinged();//mapping
    bool move2init();//回归初始位置
    bool move2clamped();//单步：运动到钳锁位置，后续的单步操作还没写
    bool back2init();//多步：运动回到开始的位置包括锁门
    ~machinecloadport();
private:
    bool initOK = false;
    bool connectOK = false;
    char* com = "COM3";
    string thiscom="";
    string nowstatu="lp_noinited";
    lp_act_data thismessage;
    clpcommon* thislp_common = nullptr;
};
