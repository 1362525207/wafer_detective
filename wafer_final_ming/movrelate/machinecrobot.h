#pragma once
#include "machinecommon.h"
#include <string>
#include "common.h"
#include <mutex>
using namespace std;



#define Manipulator 1;//��е��
#define Pre_aligner 2;//Ѱλ��

class machinecrobot : public machinecommon
{//�����Ű�������غ���
public:
    struct robot_act_data {
        string robot_command;
        string ack_robot_command = "";//���պ��������жԱȣ���ʵ���ã�
        string variable_name;//��Ҫ�޸ĵı���
        string fin_robot_command = "";//�ٴη��͵�ȷ�Ͻ������
    }ROBOT_ACT_DATA;

    //�̳в���
    machinecrobot(short _machine, Wafer_Pgsql* _wafer_pgsql, string _ip, int _port)
        :machinetype(_machine) {
        thisrobot_common = new robotcommon(_wafer_pgsql);//��ʼ�����ݿⲿ��
        thisrobot_common->robotcommon_tcp_start(_ip, _port);//��ʼ��socket����
        tcpconnectOK = true;
    }
    machinecrobot(short _machine, Wafer_Pgsql* _wafer_pgsql, wafer_json* _wafer_json,string _ip, int _port)
        :machinetype(_machine) {
        thisrobot_common = new robotcommon(_wafer_pgsql, _wafer_json);//��ʼ�����ݿⲿ��
        thisrobot_common->robotcommon_tcp_start(_ip, _port);//��ʼ��socket����
        tcpconnectOK = true;
    }
    virtual string* act_send(string message) { return nullptr; }//�ݶ���ÿ���豸���˶�����ָ����Ҫ�豸�����ٴη�װ������ֵ�������ÿ���豸�Ĵ������
    virtual string* read_send(string message) { return nullptr; }
    virtual int init() {
        thisrobot_common->robotcommon_dbrs_start();
        initOK = true;
        return initOK;
    }
    bool set_db_parameter(vector<string>, vector<int>) override;
    //���ز��֣����豸�Ļ�ȡһЩ������
    int machine_action(robot_act_data ans);
    string robot_get_destcassage();
    string robot_get_getcassage();
    string robot_get_getslot();
    string robot_get_destslot();

    //Action
    int act_init_robot();//INIT �����˳�ʼ��
    int act_getfromLP(int);//�����˴�Load port��ȡ��Բ
    int act_getfromaligner(int);//��е�ִ�Ԥ��׼������ȡ��Բ
    int act_put2LP(int);//����Բ�ŵ�Load port
    int act_put2aligner(int);//����Բ�ŵ�Ԥ��׼����
    int act_test_robot();//���Ի����ˣ�ǰ�������ƶ����ƣ�
    int act_robot_mapping();//������mapping
    int act_prtaligner();//Ԥ��׼����ִ��

//Control Commands
    int con_emergence_stop();//�����˽���ֹͣ��ť�����ɼ���
    int con_decelar_stop();//����ֹͣ��ť���ɼ���
    int con_servo_power();//�ŷ���Դ����
    int con_clear_error();//�����ʷ����

//Setting Commands
    int set_robot_speed(int SeqNo, int Speed);//���û������ٶ�
    int set_seppd_level();//�����ٶ�ˮƽ
    int set_current2transfer();//����ǰλ������Ϊtransfer
    int set_coordinate2transfer();//��ĳ��������������Ϊtransfer
    int set_parameter_value();//����ĳ��������ֵ
    int set_inner_lock();//�����ڲ���
    int set_curpoi2referposition();//Registers the current position as the manipulator coordinate��s reference  position

//Reference Commands
    string read_robot_speed();//��ȡ�������ٶ�
    string read_speed_level(int SeqNo);//��ȡ�ٶ�ˮƽ
    string read_current_position();//��ȡ��ǰλ��
    string read_mapping_result();//��ȡmapping��� References the specified transfer station��s mapping results.
    string read_align_result();//��ȡԤ��׼���

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
    robot_act_data thismessage;//�û����˵Ľṹ��
    robotcommon* thisrobot_common = nullptr;//error log socket��һ��
};

