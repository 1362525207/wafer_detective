#pragma once
#include <afxwin.h>
#include "machinecloadport.h"
#include "machinecrobot.h"
#include "WaferLogic.h"
//������
class Wafer_Control :
    public CCmdTarget
{
public:
    //���캯��ʵ����ľۺ�
    Wafer_Control() {}
        //����ʵ����ľۺ�
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
    //��ʼ�������ȿ��ƺ���
    bool wafercontrol_con_init();//��ʾ��̨��ʼ�����������еĶ����ʼ��
    bool wafercontrol_con_serialcon(int port,string com);//�����еĴ�������
    bool wafercontrol_con_tcpcon();//�����е���������
    bool wafercontrol_con_fsmrestart();//��״̬������
    bool wafercontrol_con_fsmconver();//״̬������״̬


    //���ú���
    bool wafercontrol_set_serialport(string com1,string com2,int br);//���ô�����Ϣ�����˿ںͲ�����
    bool wafercontrol_set_tcpipport(string ip,int port1,int port2);//�������ڵ�����ip�ʹ���
    bool wafercontrol_set_robotvel();//���û�е���ٶ�
    bool wafercontrol_set_alirad(int rad);//����Ѱλ�Ƕ�
    bool wafercontrol_set_errorclc();//���еĴ������
    bool wafercontrol_set_destcassage(string cas);//����Ŀ�꾧Բ��
    bool wafercontrol_set_srccassage(string cas);//����Դ��Բ��

    //��ȡ����
    bool wafercontrol_get_lpison();//��þ�Բ�����Ƿ��Ѿ�������
    string wafercontrol_get_mappinged(string cas);//���mapping��Ϣ
    int wafercontrol_get_movevel();//����˶��ٶ�
    string wafercontrol_get_nowstatus(int blade);//������ڵ�״̬
    string wafercontrol_get_nextstatus();//��ý�����״̬
    string wafercontrol_get_destcassage();//���Ŀ�꾧Բ��
    string wafercontrol_get_srccassage();//���Դ��Բ��
    int wafercontrol_get_alirad();//���Ѱλ�Ƕ�
    bool wafercontrol_getsourceslot();//���Դ�����Ƿ���Ƭ
    bool wafercontrol_getdestcaslot();//���Ŀ����Ƿ���Ƭ

    //�˶�����
    bool wafercontrol_mov_allinit();//�˶��ĳ�ʼ��
    bool wafercontrol_mov_allfor1();//һ��������ѭ��һ��
    bool wafercontrol_mov_align(int blade);//Ѱλ
    bool wafercontrol_mov_2mappinged(int blade);//lp�ƶ����ſ���
    bool wafercontrol_mov_2lpinit(int blade);//lp�ƶ��ر�λ��
    bool wafercontrol_mov_getfromlp(int blade);//��е�۴�lpȡƬ
    bool wafercontrol_mov_getfromalig(int blade);//��е�۴�alignȡƬ
    bool wafercontrol_mov_put2alig(int blade);//��е��align��Ƭ
    bool wafercontrol_mov_put2lp(int blade);//��е��lp��Ƭ
    bool wafercontrol_mov_onestep(int blade);//��������
public:
    wafer_json* jsontext = nullptr;//json��������
    Wafer_Pgsql* wafer_pgsql = nullptr;//���ݿ��������

private:
    //IO�豸,������ʱ��㶨�����������󣬲����⿪����

    //����״̬�������еĶ������ڴ˽���
    FSMworkflow* wafer_control_fsm_blade1 = new FSMworkflow(1);
    FSMworkflow* wafer_control_fsm_blade2 = new FSMworkflow(2);
    unordered_map<int,WaferStatus*> wafer_control_status1;//״̬��1���й���
    unordered_map<int,WaferStatus*> wafer_control_status2;//״̬��2���й���
    //���е��˶��豸
    machinecloadport* wafer_control_lp1 = nullptr;//lp�豸
    machinecloadport* wafer_control_lp2 = nullptr;
    vector< machinecloadport*> nowlp;
    vector< machinecloadport*> * alllp = nullptr;//lp�豸����ָ���
    machinecrobot* wafer_control_robot = nullptr;//robot
    machinecrobot* wafer_control_align = nullptr;//aligner

    bool islp_connect = false;
    bool istcp_connect = false;
    bool isthisinit = false;
    //���������е�״̬����
    //Ҫ����һ��״̬�ز���̬ά��
};

