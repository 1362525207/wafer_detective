#pragma once
#include "machinecommon.h"
#include <iostream>
#include <string>
#include <vector>
#include "common.h"
using namespace std;

#define COM3 "COM3"

class machinecloadport :
    public machinecommon //������LP����غ�������
{
public:
    struct lp_act_data {
        string need_status;
        string next_status;
        string lp_command;
        string ack_lp_command="";
        string variable_name;//��Ҫ�޸ĵı���
        string fin_lp_command="";
    }LP_ACT_DATA;
    machinecloadport(Wafer_Pgsql* _wafer_pgsql,char* _com):com(_com) {
        thislp_common = new clpcommon(_wafer_pgsql);
    }//ʵ������ʱ����Ҫ���ǵ����ݿ�󶨶���
    //�̳в��ֻ��߹�������
    virtual string* act_send(string message) { return nullptr; }//�ݶ���ÿ���豸���˶�����ָ����Ҫ�豸�����ٴη�װ������ֵ�������ÿ���豸�Ĵ������
    virtual string* read_send(string message) { return nullptr; }
    virtual int init();//��д��ʼ���麯�����йط�����Ϣ�̳��ڴ������У�S00�Լ����Ļ��з�
    bool set_db_parameter(vector<string>, vector<int>) override;
    int machine_action(lp_act_data);//ִ����غ���
    int machine_check_action(lp_act_data);
    //���ú���

    bool setlight01ON();//���þ�ʾ��Ϊ��
    bool setlight01OFF();//���þ�ʾ��Ϊ��
    bool setcassege_exist_ON();//���þ�Բ�д���ָʾ��Ϊ��
    bool setcassege_exist_OFF();//���þ�Բ�д���ָʾ��Ϊ��
    
    //�������ָ��
    int get_lp_state();//�����ص�״̬������errorcode��Ӧ��ͬ�Ĵ���
    int get_lp_mapping();//��þ�Բ������Ϣ
    string getsql(string eex) {
        return thislp_common->lpgetfromdb(eex);
    }
    //�˶�ָ��
    bool move2dooropened();//�˶�������λ��
    bool move2mappinged();//mapping
    bool move2init();//�ع��ʼλ��
    bool move2clamped();//�������˶���ǯ��λ�ã������ĵ���������ûд
    bool back2init();//�ಽ���˶��ص���ʼ��λ�ð�������
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
