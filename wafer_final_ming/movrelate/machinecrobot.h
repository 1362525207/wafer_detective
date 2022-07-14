#pragma once
#include "machinecommon.h"
#include <string>
#include "common.h"
#include <mutex>
using namespace std;



#define Manipulator 1;//机械臂
#define Pre_aligner 2;//寻位机

class machinecrobot : public machinecommon
{//该类存放安川的相关函数
public:
    struct robot_act_data {
        string robot_command;
        string ack_robot_command = "";//接收后和这个进行对比（其实不用）
        string variable_name;//需要修改的变量
        string fin_robot_command = "";//再次发送的确认结束语句
    }ROBOT_ACT_DATA;

    //继承部分
    machinecrobot(short _machine, Wafer_Pgsql* _wafer_pgsql, string _ip, int _port)
        :machinetype(_machine) {
        thisrobot_common = new robotcommon(_wafer_pgsql);//初始化数据库部分
        thisrobot_common->robotcommon_tcp_start(_ip, _port);//初始化socket部分
        tcpconnectOK = true;
    }
    machinecrobot(short _machine, Wafer_Pgsql* _wafer_pgsql, wafer_json* _wafer_json,string _ip, int _port)
        :machinetype(_machine) {
        thisrobot_common = new robotcommon(_wafer_pgsql, _wafer_json);//初始化数据库部分
        thisrobot_common->robotcommon_tcp_start(_ip, _port);//初始化socket部分
        tcpconnectOK = true;
    }
    virtual string* act_send(string message) { return nullptr; }//暂定：每个设备的运动发送指令需要设备内容再次封装，返还值是相对于每个设备的错误编码
    virtual string* read_send(string message) { return nullptr; }
    virtual int init() {
        thisrobot_common->robotcommon_dbrs_start();
        initOK = true;
        return initOK;
    }
    bool set_db_parameter(vector<string>, vector<int>) override;
    //独特部分：该设备的获取一些参数等
    int machine_action(robot_act_data ans);
    string robot_get_destcassage();
    string robot_get_getcassage();
    string robot_get_getslot();
    string robot_get_destslot();

    //Action
    int act_init_robot();//INIT 机器人初始化
    int act_getfromLP(int);//机器人从Load port获取晶圆
    int act_getfromaligner(int);//机械手从预对准机构获取晶圆
    int act_put2LP(int);//将晶圆放到Load port
    int act_put2aligner(int);//将晶圆放到预对准机构
    int act_test_robot();//测试机器人（前后左右移动控制）
    int act_robot_mapping();//机器人mapping
    int act_prtaligner();//预对准机构执行

//Control Commands
    int con_emergence_stop();//机器人紧急停止按钮，不可继续
    int con_decelar_stop();//紧急停止按钮，可继续
    int con_servo_power();//伺服电源开关
    int con_clear_error();//清除历史错误

//Setting Commands
    int set_robot_speed(int SeqNo, int Speed);//设置机器人速度
    int set_seppd_level();//设置速度水平
    int set_current2transfer();//将当前位置设置为transfer
    int set_coordinate2transfer();//将某个具体坐标设置为transfer
    int set_parameter_value();//设置某个参数的值
    int set_inner_lock();//设置内部锁
    int set_curpoi2referposition();//Registers the current position as the manipulator coordinate’s reference  position

//Reference Commands
    string read_robot_speed();//读取机器人速度
    string read_speed_level(int SeqNo);//读取速度水平
    string read_current_position();//读取当前位置
    string read_mapping_result();//读取mapping结果 References the specified transfer station’s mapping results.
    string read_align_result();//读取预对准结果

    void thissend(string ans) {
        thisrobot_common->robotcommon_tcp_send(ans);
    }
private:
    mutex movelock;
    short machinetype = Manipulator;
    int seqno = 1;
    bool initOK = false;
    bool tcpconnectOK = false;
    string thiscom = "";
    bool isonalign = false;
    bool isplade1on = false;
    bool isplage2on = false;
    bool isserveon = false;
    robot_act_data thismessage;//该机器人的结构体
    robotcommon* thisrobot_common = nullptr;//error log socket的一体
};

