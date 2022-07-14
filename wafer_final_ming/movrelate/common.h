#pragma once
#include <afx.h>
#include "Wafer_Csocket.h"
#include "Wafer_Cserial.h"
#include "Wafer_Pgsql.h"
#include "wafer_json.h"
#include <vector>
#include <string>
//该文件表示了公共部分的使用，主要是对log error以及通信的使用；
//还未定义error的应用，也没定义通信上的流程，比如专门对目前设备的ack和error的测试
#define LOG_Send 1
#define LOG_Receive 2
#define LOG_ERROR 3
class common :public CObject
{
public:
    common(Wafer_Pgsql* _this_wafer_pgsql){
        this_wafer_pgsql = _this_wafer_pgsql;}
    common(Wafer_Pgsql* _this_wafer_pgsql, wafer_json* _this_wafer_json) {
        this_wafer_pgsql = _this_wafer_pgsql;
        this_wafer_json = _this_wafer_json;
    }
    virtual bool wafer_log_write(int mode, string ans);
    virtual bool wafer_error();
    bool wafer_db_start();
    bool wafer_db_close();
    virtual string wafer_json_common_getkey(string keyname);
    virtual vector<int> wafer_json_common_getarray(string keyname);
    virtual string wafer_json_common_getarray(string keyname, int key);
    virtual bool wafer_json_common_setarray(string keyname,int key, string keyans);
    virtual bool wafer_json_common_setkey(string keyname, string keyans);
    virtual bool wafer_json_common_setkey(string keyname1,string keyname2, string keyans);
protected:
    Wafer_Pgsql* this_wafer_pgsql = nullptr;
    wafer_json* this_wafer_json = nullptr;
};

class robotcommon :
    public common //相当于每一个设备的模型抽象后的需求
{
public:
    robotcommon(Wafer_Pgsql* _this_wafer_pgsql) :common(_this_wafer_pgsql) {}
    robotcommon(Wafer_Pgsql* _this_wafer_pgsql, wafer_json* _this_wafer_json) 
        :common(_this_wafer_pgsql, _this_wafer_json) {}
    bool wafer_log_write(int mode, string ans) override;//对安川的单独的log函数
    bool robotcommon_dbrs_start();//数据结果生成，一般而言是指定一个变量
    bool robotcommon_set_varible(string variable_name, string variable);
    vector<string> robot_getcommand_fromdb(string);
    //还需要自主搭建数据集的函数，还未定义

    bool wafer_error() override;//该设备发生错误了，需要处理
    bool ismessageget() {
        return  robot_socket->Wafer_Csocket_Receive();
    }

    bool robotcommon_tcp_start(string ,int);//socket通讯
    bool robotcommon_tcp_send(string);//socket发送消息
    string robotcommon_tcp_receive() {
        string ans = robot_socket->Wafer_Csocket_Getstring();
        if (ans.size() == 0) return "";
        this->wafer_log_write(LOG_Receive, ans);
        return ans;
    }//接受socket消息
    //bool robotcommon_tcp_end();


    virtual string wafer_json_common_getkey(string keyname);
    virtual vector<int> wafer_json_common_getarray(string keyname);
    virtual string wafer_json_common_getarray(string keyname, int key);
    virtual bool wafer_json_common_setarray(string keyname, int key, string keyans);
    virtual bool wafer_json_common_setkey(string keyname, string keyans);
    virtual bool wafer_json_common_setkey(string keyname1, string keyname2, string keyans);

private:
    bool db_connected = false;
    Wafer_Csocket* robot_socket=nullptr;//与安川机械臂通信的对象
    Wafer_Pgsql_Record* robot_dbcommon_rs = nullptr;
};

class clpcommon :public common {//定义lp与error和log的一些函数
public:
    clpcommon(Wafer_Pgsql* _this_wafer_pgsql):common(_this_wafer_pgsql){}
    clpcommon(Wafer_Pgsql* _this_wafer_pgsql, wafer_json* _this_wafer_json) 
        :common(_this_wafer_pgsql, _this_wafer_json) {}
    bool wafer_log_write(int mode, string ans) override;//写日志函数
    bool clpcommon_dbrs_start();//数据查询任务开启
    bool clpcommon_set_varible(string variable_name,string variable);
    vector<string> lp_getcommand_fromdb(string);
    string lpgetfromdb(string exc) {
        string ans = lp_dbcommon_rs->wafer_pgsql_select(exc);
        return ans;
    }
    bool wafer_error() override;//lp状态错误了

    bool clpcommon_serial_start(char*);//串口开启
    bool clpcommon_serial_send(string);//发送消息，需要包含该设备的流程
    string clpcommon_serial_receive() {
        string ans = lp_serial_port->wafer_serial_receive();
        if (ans.size() == 0) return "";
        this->wafer_log_write(LOG_Receive, ans);
        return ans;
    }

    virtual string wafer_json_common_getkey(string keyname);
    virtual vector<int> wafer_json_common_getarray(string keyname);
    virtual string wafer_json_common_getarray(string keyname, int key);
    virtual bool wafer_json_common_setarray(string keyname, int key, string keyans);
    virtual bool wafer_json_common_setkey(string keyname, string keyans);
    virtual bool wafer_json_common_setkey(string keyname1, string keyname2, string keyans);

private:
    Wafer_Cserial* lp_serial_port=nullptr ;//与一个lp的交流
    Wafer_Pgsql_Record* lp_dbcommon_rs = nullptr;
    bool db_connected = false;
    //Wafer_Cserial* lp_serial_port2 = new Wafer_Cserial;//与第二个lp的交流
};