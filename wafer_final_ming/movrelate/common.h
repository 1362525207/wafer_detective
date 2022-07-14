#pragma once
#include <afx.h>
#include "Wafer_Csocket.h"
#include "Wafer_Cserial.h"
#include "Wafer_Pgsql.h"
#include "wafer_json.h"
#include <vector>
#include <string>
//���ļ���ʾ�˹������ֵ�ʹ�ã���Ҫ�Ƕ�log error�Լ�ͨ�ŵ�ʹ�ã�
//��δ����error��Ӧ�ã�Ҳû����ͨ���ϵ����̣�����ר�Ŷ�Ŀǰ�豸��ack��error�Ĳ���
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
    public common //�൱��ÿһ���豸��ģ�ͳ���������
{
public:
    robotcommon(Wafer_Pgsql* _this_wafer_pgsql) :common(_this_wafer_pgsql) {}
    robotcommon(Wafer_Pgsql* _this_wafer_pgsql, wafer_json* _this_wafer_json) 
        :common(_this_wafer_pgsql, _this_wafer_json) {}
    bool wafer_log_write(int mode, string ans) override;//�԰����ĵ�����log����
    bool robotcommon_dbrs_start();//���ݽ�����ɣ�һ�������ָ��һ������
    bool robotcommon_set_varible(string variable_name, string variable);
    vector<string> robot_getcommand_fromdb(string);
    //����Ҫ��������ݼ��ĺ�������δ����

    bool wafer_error() override;//���豸���������ˣ���Ҫ����
    bool ismessageget() {
        return  robot_socket->Wafer_Csocket_Receive();
    }

    bool robotcommon_tcp_start(string ,int);//socketͨѶ
    bool robotcommon_tcp_send(string);//socket������Ϣ
    string robotcommon_tcp_receive() {
        string ans = robot_socket->Wafer_Csocket_Getstring();
        if (ans.size() == 0) return "";
        this->wafer_log_write(LOG_Receive, ans);
        return ans;
    }//����socket��Ϣ
    //bool robotcommon_tcp_end();


    virtual string wafer_json_common_getkey(string keyname);
    virtual vector<int> wafer_json_common_getarray(string keyname);
    virtual string wafer_json_common_getarray(string keyname, int key);
    virtual bool wafer_json_common_setarray(string keyname, int key, string keyans);
    virtual bool wafer_json_common_setkey(string keyname, string keyans);
    virtual bool wafer_json_common_setkey(string keyname1, string keyname2, string keyans);

private:
    bool db_connected = false;
    Wafer_Csocket* robot_socket=nullptr;//�밲����е��ͨ�ŵĶ���
    Wafer_Pgsql_Record* robot_dbcommon_rs = nullptr;
};

class clpcommon :public common {//����lp��error��log��һЩ����
public:
    clpcommon(Wafer_Pgsql* _this_wafer_pgsql):common(_this_wafer_pgsql){}
    clpcommon(Wafer_Pgsql* _this_wafer_pgsql, wafer_json* _this_wafer_json) 
        :common(_this_wafer_pgsql, _this_wafer_json) {}
    bool wafer_log_write(int mode, string ans) override;//д��־����
    bool clpcommon_dbrs_start();//���ݲ�ѯ������
    bool clpcommon_set_varible(string variable_name,string variable);
    vector<string> lp_getcommand_fromdb(string);
    string lpgetfromdb(string exc) {
        string ans = lp_dbcommon_rs->wafer_pgsql_select(exc);
        return ans;
    }
    bool wafer_error() override;//lp״̬������

    bool clpcommon_serial_start(char*);//���ڿ���
    bool clpcommon_serial_send(string);//������Ϣ����Ҫ�������豸������
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
    Wafer_Cserial* lp_serial_port=nullptr ;//��һ��lp�Ľ���
    Wafer_Pgsql_Record* lp_dbcommon_rs = nullptr;
    bool db_connected = false;
    //Wafer_Cserial* lp_serial_port2 = new Wafer_Cserial;//��ڶ���lp�Ľ���
};