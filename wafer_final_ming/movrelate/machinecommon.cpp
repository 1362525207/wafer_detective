#include "pch.h"
#include "machinecommon.h"
string* machinecommon::act_send(string message) {
	string data[2] = { "data1","data2" };//data=�Ƿ���ȷ��������Ϣ
	return data;
}

string* machinecommon::read_send(string message) {
	string data[3] = { "data1","data2","data3" };//data=�Ƿ���ȷ����Ҫ������Լ�������ȫ�����
	return data;
}
/*

int machinecommon::action_set_parameter(act_data act_d)
{
	string tatus = read_db_parametr(act_d.need_read_status);//�Ӵ�������ж�ȡ��Ҫ״̬������ͬ����ȡ����ȫ����
	if (tatus == act_d.required_status) {//�ж�״̬�����Ƿ���������
		string* acceptmessage = act_send(act_d.text);//����ִ��ָ��
		//string acceptmessage = get();//��ȡ����ָ��
		if (acceptmessage[0] == act_d.right_condition) {//�жϷ���ָ���Ƿ�����
			set_db_parameter(act_d.variable_name, act_d.variable_value);//����״̬����
			Log(act_d.event_name, Success_act);//����log�ɹ���־
			return(0);
		}
		else {
			Log(act_d.event_name, Fail_act);//����logʧ����־
			Error(act_d.event_name, Fail_act, acceptmessage[1]);//����������error
			return(1);
		}
	}
	else {
		Log(act_d.event_name, Fail_condition);//����logʧ����־
		Error(act_d.event_name, Fail_condition, "");//����������error
		return(1);
	}
}

string machinecommon::read(read_data_type read_d)
{
	string tatus = read_db_parametr(read_d.need_read_status);//��ȡ��Ҫ״̬����
	if (tatus == read_d.required_status) {//�ж�״̬�����Ƿ���������
		string* acceptmessage = read_send(read_d.text);//����ִ��ָ��
		//string acceptmessage = get();//��ȡ����ָ��
		if (acceptmessage[0] == read_d.right_condition) {//�жϷ���ָ���Ƿ�����
			Log(read_d.event_name, Success_act);//����log�ɹ���־
			return(acceptmessage[1]);
		}
		else {
			Log(read_d.event_name, Fail_act);//����logʧ����־
			Error(read_d.event_name, Fail_act, acceptmessage[2]);//����������error
			return(read_feedback_result_fail);
		}
	}
	else {
		Log(read_d.event_name, Fail_condition);//����logʧ����־
		Error(read_d.event_name, Fail_condition, "");//����������error
		return(read_feedback_result_fail);
	}
}
*/