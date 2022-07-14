#include "pch.h"
#include "machinecloadport.h"
#include <string>
//�豸��غ���
int machinecloadport::machine_action(lp_act_data ans) {
	if (nowstatu == ans.next_status) return true;//��ʾ��ǰ״̬����Ҫ��
	if (nowstatu != ans.need_status) return false;//�ж��Ƿ�������ڵ�״̬,�������򱨴�
	thislp_common->clpcommon_serial_send(ans.lp_command);//֮����Ҫ�������ģʽ
	string thisans = "";
	while (thisans.size() == 0) {
		thisans = thislp_common->clpcommon_serial_receive();//�õ����ܴ���
	}
	if (thisans == ans.ack_lp_command) {//����ȷ�Ϻ���ACK
		//��������ר�������Ӳ��ֵ�����
		thisans = "";
		while (thisans.size() == 0) {
			thisans = thislp_common->clpcommon_serial_receive();//�õ�����˵��
		}
		if (thisans.substr(0, 3) == "INF") {
			thislp_common->clpcommon_serial_send(ans.fin_lp_command);
		}
		else thislp_common->wafer_error();
	}
	else { thislp_common->wafer_error(); }//����ȷ����Ϣ���������б�
	nowstatu = ans.next_status;
	return true;
}//���豸���������
bool machinecloadport::set_db_parameter(vector<string> variable_name, vector<int> variable) {
	for (unsigned int i = 0; i < variable.size(); i++) {
		thislp_common->clpcommon_set_varible(variable_name[i], to_string(variable[i]));//���������޸�
	}
	return true;
}//�ú����ǽ��б����޸ĵĺ���
//���Կ���������Ļ������и�machine_action���麯��
//���ݿ�ĶԽӲ�����Ҫֱ���޸Ĳ�����Ϣ�ĺ���
int machinecloadport::machine_check_action(lp_act_data ans) {
	vector<string> varible_name;
	unsigned int rig = 0, lef = 0;//�����ǻ���˸�����Ҫ�޸ĵ�����
	while (rig < ans.variable_name.size() && ans.variable_name[rig] != ';') {//��ѭ������˼���ڷָ�����
		while (rig < ans.variable_name.size() && ans.variable_name[rig] != ',' && ans.variable_name[rig] != ';') {
			rig++;
		}
		string now = ans.variable_name.substr(lef, rig - lef);
		lef = rig + 1;
		rig++;
		varible_name.push_back(now);//���������
	}
	thislp_common->clpcommon_serial_send(ans.lp_command);//֮����Ҫ�������ģʽ
	string thisans = "";
	vector<int> vari;
	while (thisans.size() == 0) {
		thisans = thislp_common->clpcommon_serial_receive();//�õ����ܴ���
	}
	if (thisans.substr(0, 3) == "ACK") {//����ȷ�Ϻ���ACK
		//��������ר�������Ӳ��ֵ�����
		unsigned int right = 14, left = 14;
		while (right < thisans.size() && thisans[right] != ';') {//��ѭ������˼���ڷָ�����
			vari.push_back(thisans[right] - '0');//���������
			right++;
		}
		machinecloadport::set_db_parameter(varible_name, vari);//����Ǹ���״̬��Ϣ����أ����о�error
	}
	else { thislp_common->wafer_error(); }//����ȷ����Ϣ���������б�
	return true;
}
int machinecloadport::init() {
	bool ans = 0;
	ans = thislp_common->clpcommon_serial_start(com);//��������
	if (ans == 0) return ans;
	ans = thislp_common->clpcommon_dbrs_start();//����
	return ans;
}//��д��ʼ���麯�����йط�����Ϣ�̳��ڴ������У�S00�Լ����Ļ��з�

bool machinecloadport::setlight01ON() {
	string name = "setlight01ON";
	vector<string> lp_message = thislp_common->lp_getcommand_fromdb(name);//����ִ����Ϣ,ֱ�Ӳ�������豸ֱ���ڱ�������������
															//�ŵ��������ӵĲ��֣����ֱ����ŵ��ö����ڲ�
								//��������Ҫ��״̬��ת����״̬��ִ�е������Ҫ�޸ĵı���
	thismessage.need_status = lp_message[1];
	thismessage.next_status = lp_message[2];
	thismessage.lp_command = lp_message[4];
	thismessage.ack_lp_command = lp_message[6];
	thismessage.variable_name = lp_message[3];//�������ֶ�����Ҫ�޸ĵı���
	int result = machine_check_action(thismessage);//ִ�����������
	return result;
}//���þ�ʾ��Ϊ��
bool machinecloadport::setlight01OFF() {
	string name = "setlight01OFF";
	vector<string> lp_message = thislp_common->lp_getcommand_fromdb(name);//����ִ����Ϣ,ֱ�Ӳ�������豸ֱ���ڱ�������������
															//�ŵ��������ӵĲ��֣����ֱ����ŵ��ö����ڲ�
								//��������Ҫ��״̬��ת����״̬��ִ�е������Ҫ�޸ĵı���
	thismessage.need_status = lp_message[1];
	thismessage.next_status = lp_message[2];
	thismessage.lp_command = lp_message[4];
	thismessage.ack_lp_command = lp_message[6];
	thismessage.variable_name = lp_message[3];//�������ֶ�����Ҫ�޸ĵı���
	int result = machine_check_action(thismessage);//ִ�����������
	return result;
}//���þ�ʾ��Ϊ��
bool machinecloadport::setcassege_exist_ON() {
	string name = "setcassege_exist_ON";
	vector<string> lp_message = thislp_common->lp_getcommand_fromdb(name);//����ִ����Ϣ,ֱ�Ӳ�������豸ֱ���ڱ�������������
															//�ŵ��������ӵĲ��֣����ֱ����ŵ��ö����ڲ�
								//��������Ҫ��״̬��ת����״̬��ִ�е������Ҫ�޸ĵı���
	thismessage.need_status = lp_message[1];
	thismessage.next_status = lp_message[2];
	thismessage.lp_command = lp_message[4];
	thismessage.ack_lp_command = lp_message[6];
	thismessage.variable_name = lp_message[3];//�������ֶ�����Ҫ�޸ĵı���
	int result = machine_check_action(thismessage);//ִ�����������
	return result;
}//���þ�Բ�д���ָʾ��Ϊ��
bool machinecloadport::setcassege_exist_OFF() {
	string name = "setcassege_exist_OFF";
	vector<string> lp_message = thislp_common->lp_getcommand_fromdb(name);//����ִ����Ϣ,ֱ�Ӳ�������豸ֱ���ڱ�������������
															//�ŵ��������ӵĲ��֣����ֱ����ŵ��ö����ڲ�
								//��������Ҫ��״̬��ת����״̬��ִ�е������Ҫ�޸ĵı���
	thismessage.need_status = lp_message[1];
	thismessage.next_status = lp_message[2];
	thismessage.lp_command = lp_message[4];
	thismessage.ack_lp_command = lp_message[6];
	thismessage.variable_name = lp_message[3];//�������ֶ�����Ҫ�޸ĵı���
	int result = machine_check_action(thismessage);//ִ�����������
	return result;
}//���þ�Բ�д���ָʾ��Ϊ��

//�������ָ��
int machinecloadport::get_lp_state() {
	return {};
}//�����ص�״̬������errorcode��Ӧ��ͬ�Ĵ���
int machinecloadport::get_lp_mapping() {
	string name = "get_lp_mapping";
	vector<string> lp_message = thislp_common->lp_getcommand_fromdb(name);//����ִ����Ϣ,ֱ�Ӳ�������豸ֱ���ڱ�������������
															//�ŵ��������ӵĲ��֣����ֱ����ŵ��ö����ڲ�
								//��������Ҫ��״̬��ת����״̬��ִ�е������Ҫ�޸ĵı���
	thismessage.need_status = lp_message[1];
	thismessage.next_status = lp_message[2];
	thismessage.lp_command = lp_message[4];
	thismessage.ack_lp_command = lp_message[6];
	thismessage.variable_name = lp_message[3];//�������ֶ�����Ҫ�޸ĵı���
	int result = machine_check_action(thismessage);//ִ�����������
	return result;
}//��þ�Բ������Ϣ

//�˶�ָ��
bool machinecloadport::move2dooropened() {
	string name = "move2dooropened";
	vector<string> lp_message = thislp_common->lp_getcommand_fromdb(name);//����ִ����Ϣ,ֱ�Ӳ�������豸ֱ���ڱ�������������
	thismessage.need_status = lp_message[1];
	thismessage.next_status = lp_message[2];
	thismessage.lp_command = lp_message[4];
	thismessage.ack_lp_command = lp_message[6];
	thismessage.fin_lp_command = lp_message[7];
	thismessage.variable_name = lp_message[3];//�������ֶ�����Ҫ�޸ĵı���
	bool result = machine_action(thismessage);//ִ�����������
	return result;
}//�˶�������λ��
bool machinecloadport::move2mappinged() {
	string name = "move2mappinged";
	vector<string> lp_message = thislp_common->lp_getcommand_fromdb(name);//����ִ����Ϣ,ֱ�Ӳ�������豸ֱ���ڱ�������������
	thismessage.need_status = lp_message[1];
	thismessage.next_status = lp_message[2];
	thismessage.lp_command = lp_message[4];
	thismessage.ack_lp_command = lp_message[6];
	thismessage.fin_lp_command = lp_message[7];
	thismessage.variable_name = lp_message[3];//�������ֶ�����Ҫ�޸ĵı���
	bool result = machine_action(thismessage);//ִ�����������
	return result;
}//mapping
bool machinecloadport::move2init() {
	string name = "set_init_robot";
	vector<string> lp_message = thislp_common->lp_getcommand_fromdb(name);//����ִ����Ϣ,ֱ�Ӳ�������豸ֱ���ڱ�������������
															//�ŵ��������ӵĲ��֣����ֱ����ŵ��ö����ڲ�
								//��������Ҫ��״̬��ת����״̬��ִ�е������Ҫ�޸ĵı���
	thismessage.need_status = lp_message[1];
	thismessage.next_status = lp_message[2];
	thismessage.lp_command = lp_message[4];
	thismessage.ack_lp_command = lp_message[6];
	thismessage.fin_lp_command = lp_message[7];
	thismessage.variable_name = lp_message[3];//�������ֶ�����Ҫ�޸ĵı���
	int result = machine_action(thismessage);//ִ�����������
	return result;
}//�ع��ʼλ��
bool machinecloadport::move2clamped() {
	string name = "move2clamped";
	vector<string> lp_message = thislp_common->lp_getcommand_fromdb(name);//����ִ����Ϣ,ֱ�Ӳ�������豸ֱ���ڱ�������������
															//�ŵ��������ӵĲ��֣����ֱ����ŵ��ö����ڲ�
								//��������Ҫ��״̬��ת����״̬��ִ�е������Ҫ�޸ĵı���
	thismessage.need_status = lp_message[1];
	thismessage.next_status = lp_message[2];
	thismessage.lp_command = lp_message[4];
	thismessage.ack_lp_command = lp_message[6];
	thismessage.fin_lp_command = lp_message[7];
	thismessage.variable_name = lp_message[3];//�������ֶ�����Ҫ�޸ĵı���
	int result = machine_action(thismessage);//ִ�����������
	return result;
}//�������˶���ǯ��λ�ã������ĵ���������ûд
bool machinecloadport::back2init() {
	string name = "back2init";
	vector<string> lp_message = thislp_common->lp_getcommand_fromdb(name);//����ִ����Ϣ,ֱ�Ӳ�������豸ֱ���ڱ�������������
															//�ŵ��������ӵĲ��֣����ֱ����ŵ��ö����ڲ�
								//��������Ҫ��״̬��ת����״̬��ִ�е������Ҫ�޸ĵı���
	thismessage.need_status = lp_message[1];
	thismessage.next_status = lp_message[2];
	thismessage.lp_command = lp_message[4];
	thismessage.ack_lp_command = lp_message[6];
	thismessage.fin_lp_command = lp_message[7];
	thismessage.variable_name = lp_message[3];//�������ֶ�����Ҫ�޸ĵı���
	int result = machine_action(thismessage);//ִ�����������
	return result;
}
machinecloadport::~machinecloadport() {

}