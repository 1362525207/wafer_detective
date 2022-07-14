
#include "pch.h"
#include "machinecrobot.h"
#include "machinecommon.h"
/*ʵ�ֹ��ܣ�
* 1���߼��Ӻ�������������������
* 2��״̬��������
* 3��Log,Error����
* "$, 1,"
* ", INIT, 1, 1, G,"
*/
using namespace std;
volatile bool checkOnReceive = false;
bool machinecrobot::set_db_parameter(vector<string> variable_name, vector<int> variable) {
	for (unsigned int i = 0; i < variable.size(); i++) {
		thisrobot_common->robotcommon_set_varible(variable_name[i], to_string(variable[i]));//���������޸�
	}
	return true;
}
int machinecrobot::machine_action(robot_act_data ans) {
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
	thisrobot_common->robotcommon_tcp_send(ans.robot_command);//֮����Ҫ�������ģʽ
	string thisans = "";
	vector<int> vari;
	while (thisans.size() == 0) {
		while(!checkOnReceive){ }
		checkOnReceive = false;
		if (thisrobot_common->ismessageget()) {
			thisans = thisrobot_common->robotcommon_tcp_receive();
		}
	}//�õ����ܴ���
	if (thisans.substr(10, 4) == "0000") {//����ȷ�Ϻ���ACK
		//��������ר�������Ӳ��ֵ�����
		thisans = "";
		while (thisans.size() == 0) {
			if (thisrobot_common->ismessageget()) {
				thisans = thisrobot_common->robotcommon_tcp_receive();
			}
		}
		if (thisans.substr(0, 5) == "!0000") {//���صĴ������û�б���
			unsigned int right = 5, left = 5;
			while (right < thisans.size() && thisans[right] != ';') {//��ѭ������˼���ڷָ�����
				while (right < thisans.size() && thisans[right] != ',' && thisans[right] != ';') {
					right++;
				}
				string now = thisans.substr(left, right - left);
				left = right + 1;
				right++;
				vari.push_back(stoi(now));//���������
			}
			set_db_parameter(varible_name, vari);//����Ǹ���״̬��Ϣ����أ����о�error
		}
		else thisrobot_common->wafer_error();

	}
	else { thisrobot_common->wafer_error(); }//����ȷ����Ϣ���������б�
	//else thislp_common->error;
	return true;
}//���豸���������
string machinecrobot::robot_get_getcassage() {
	if (!this->initOK) return "";
	return thisrobot_common->wafer_json_common_getkey("Source Cassette stage");
}
string machinecrobot::robot_get_destcassage() {
	if (!this->initOK) return "";
	return thisrobot_common->wafer_json_common_getkey("Dest Cassette stage");
}
string machinecrobot::robot_get_getslot() {
	if (!this->initOK) return "";
	string ans = "";
	string getcass = robot_get_getcassage();
	getcass += "Slot";
	vector<int> now = thisrobot_common->wafer_json_common_getarray(getcass);//�۵�����
	int hhh = -1;
	for (unsigned int i = 0; i <= now.size(); i++) {
		if (i == now.size()) {
			return "-1";//��ʾ�Ѿ����������е�Ƭ����û�ˣ���������Ҫ���봦�����
		}
		else if (now[i] == 1) {
			ans = to_string(i);
			hhh = i;
			if (i < 10) ans = "0" + ans;
			break;
		}
	}
	thisrobot_common->wafer_json_common_setkey("nowslot", ans);
	thisrobot_common->wafer_json_common_setarray(getcass, hhh, "0");
	return ans;
}
string machinecrobot::robot_get_destslot() {
	if (!this->initOK) return "";
	string ans = "";
	string getcass = robot_get_destcassage();
	getcass += "Slot";
	vector<int> now = thisrobot_common->wafer_json_common_getarray(getcass);//�۵�����
	for (unsigned int i = 0; i <= now.size(); i++) {
		if (i == now.size()) {
			return "-1";//��ʾ�Ѿ����������е�Ƭ�������ˣ���������Ҫ���봦�����
		}
		else if (now[i] == 0) {
			ans = to_string(i);
			if (i < 10) ans = "0" + ans;
			break;
		}
	}
	thisrobot_common->wafer_json_common_setkey("nowslot", ans);
	return ans;
}
//Action///////////////////////////////////////////
int machinecrobot::act_init_robot() {
	if (!this->initOK) return 0;
	thismessage.robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ ",INIT,1,1,G;";
	thismessage.ack_robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",INIT";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",INIT";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//ִ�����������
	seqno=(seqno+1)%100;
	return result;
};
int machinecrobot::act_getfromLP(int blade) {
	if (blade == 1) {
		if (isplade1on) return 1;//Ƭ1���о�Բ�����ܲ���
	}
	else { if (isplage2on) return 1; }//Ƭ2���о�Բ�����ܲ���
	thismessage.robot_command = "$,1," + to_string(seqno) + ",MTRS,G," 
		+ robot_get_getcassage()+","+ robot_get_getslot() + ",A,"+to_string(blade)+",Gb; ";
	thismessage.ack_robot_command = "$,1," + to_string(seqno) +
		+ "0000" +  ",MTRS";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",MTRS";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//ִ�����������
	seqno = (seqno + 1) % 100;
	if (blade == 1) {
		isplade1on = true;
	}
	else { isplage2on = true; }//��־�о�Բ��
	return result;
};
int machinecrobot::act_getfromaligner(int blade) {
	if (blade == 1) {
		if (isplade1on) return 1;//Ƭ1���о�Բ�����ܲ���
	}
	else { if (isplage2on) return 1; }//Ƭ2���о�Բ�����ܲ���
	if (!this->initOK) return 0;
	thismessage.robot_command = "$,1," + to_string(seqno) + ",MTRS,G,"
		+ "P01,00,A,"+ to_string(blade) + ",Gb; ";
	thismessage.ack_robot_command = "$,1," + to_string(seqno) +
		+"0000" + ",MTRS";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",MTRS";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//ִ�����������
	seqno = (seqno + 1) % 100;
	this->isonalign = false;
	if (blade == 1) {
		isplade1on = true;
	}
	else { isplage2on = true; }//��־�о�Բ��
	return result;
}
int machinecrobot::act_put2LP(int blade) {
	if (blade == 1) {
		if (!isplade1on) return 0;//Ƭ1��û�о�Բ�����ܲ���
	}
	else { if (!isplage2on) return 0; }//Ƭ2��û�о�Բ�����ܲ���
	if (!this->initOK) return 0;
	thismessage.robot_command = "$,1," + to_string(seqno) + ",MTRS,P,"
		+ robot_get_destcassage() + "," + robot_get_destslot()
		+ ",A,"+ to_string(blade) + ", Gb; ";
	thismessage.ack_robot_command = "$,1," + to_string(seqno) +
		+"0000" + ",MTRS";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",MTRS";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//ִ�����������
	seqno = (seqno + 1) % 100;
	if (blade == 1) {
		isplade1on = false;
	}
	else { isplage2on = false; }//��־û�о�Բ��
	return result;
}
int machinecrobot::act_put2aligner(int blade) {
	if (this->isonalign) return 1;//�ж��Ƿ��Ѿ���������
	if (blade == 1) {
		if (!isplade1on) return 0;//Ƭ1��û�о�Բ�����ܲ���
	}
	else { if (!isplage2on) return 0; }//Ƭ2��û�о�Բ�����ܲ���
	if (!this->initOK) return 0;
	thismessage.robot_command = "$,1," + to_string(seqno) + ",MTRS,P,"
		+ "P01,00,A,"+ to_string(blade) + ",Gb; ";
	thismessage.ack_robot_command = "$,1," + to_string(seqno) +
		+"0000" + ",MTRS";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",MTRS";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//ִ�����������
	seqno = (seqno + 1) % 100;
	this->isonalign = true;
	if (blade == 1) {
		isplade1on = false;
	}
	else { isplage2on = false; }//��־û�о�Բ��
	return result;
}
int machinecrobot::act_test_robot() { 
	//��ע����
	if (machinetype == 1 || !this->initOK) return 0;
	thismessage.robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ ",MALN,0," + thisrobot_common->wafer_json_common_getkey("alignerAngle")
		+ ",;";
	thismessage.ack_robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",MALN";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",MALN";
	thismessage.variable_name = "axis_position;";//���ﻹ�кܶණ��������
	int result = machine_action(thismessage);//ִ�����������
	seqno = (seqno + 1) % 100;
	return result;
}
int machinecrobot::act_robot_mapping() {
	//�ݶ�
	if (machinetype == 1 || !this->initOK) return 0;
	thismessage.robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ ",MADL,0," + thisrobot_common->wafer_json_common_getkey("alignerAngle")
		+ ",;";
	thismessage.ack_robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",MADL";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",MADL";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//ִ�����������
	seqno = (seqno + 1) % 100;
	return result;
}
int machinecrobot::act_prtaligner() {
	if (machinetype == 1 || !this->initOK) return 0;
	thismessage.robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ ",MALN,0," + thisrobot_common->wafer_json_common_getkey("alignerAngle")
		+ ",;";
	thismessage.ack_robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",MALN";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",MALN";
	thismessage.variable_name = "axis_position;";//���ﻹ�кܶණ��������
	int result = machine_action(thismessage);//ִ�����������
	seqno = (seqno + 1) % 100;
	return result;
}

//machinecrobot::control Commands
int machinecrobot::con_emergence_stop() { 
	//�ݶ�
	if (machinetype == 1 || !this->initOK) return 0;
	thismessage.robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ ",CONV,0," + thisrobot_common->wafer_json_common_getkey("alignerAngle")
		+ ",;";
	thismessage.ack_robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",CONV";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",CONV";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//ִ�����������
	seqno = (seqno + 1) % 100;
	return result;
};
int machinecrobot::con_decelar_stop() {
	//�ݶ�
	if (machinetype == 1 || !this->initOK) return 0;
	thismessage.robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ ",STOP,0," + thisrobot_common->wafer_json_common_getkey("alignerAngle")
		+ ",;";
	thismessage.ack_robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",STOP";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",STOP";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//ִ�����������
	seqno = (seqno + 1) % 100;
	return result;
};
int machinecrobot::con_servo_power() {
	//�ݶ�
	if (machinetype == 1 || !this->initOK) return 0;
	thismessage.robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ ",POWE,0," + thisrobot_common->wafer_json_common_getkey("alignerAngle")
		+ ",;";
	thismessage.ack_robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",POWE";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",POWE";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//ִ�����������
	seqno = (seqno + 1) % 100;
	return result;
};
int machinecrobot::con_clear_error() { 
	//�ݶ�
	if (machinetype == 1 || !this->initOK) return 0;
	thismessage.robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ ",CLCS,0," + thisrobot_common->wafer_json_common_getkey("alignerAngle")
		+ ",;";
	thismessage.ack_robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",CLCS";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",CLCS";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//ִ�����������
	seqno = (seqno + 1) % 100;
	return result;
};

//machinecrobot::setting Commands///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int machinecrobot::set_robot_speed(int SeqNo, int Speed) {
	//�ݶ�
	if (machinetype == 1 || !this->initOK) return 0;
	thismessage.robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ ",SETS,0," + thisrobot_common->wafer_json_common_getkey("alignerAngle")
		+ ",;";
	thismessage.ack_robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",SETS";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",SETS";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//ִ�����������
	seqno = (seqno + 1) % 100;
	return result;
};
int machinecrobot::set_seppd_level() {
	//�ݶ�
	if (machinetype == 1 || !this->initOK) return 0;
	thismessage.robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ ",SETL,0," + thisrobot_common->wafer_json_common_getkey("alignerAngle")
		+ ",;";
	thismessage.ack_robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",SETL";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",SETL";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//ִ�����������
	seqno = (seqno + 1) % 100;
	return result;
}
int machinecrobot::set_current2transfer() { 
	//�ݶ�
	if (machinetype == 1 || !this->initOK) return 0;
	thismessage.robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ ",CURR,1," + thisrobot_common->wafer_json_common_getkey("alignerAngle")
		+ ",;";
	thismessage.ack_robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",CURR";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",CURR";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//ִ�����������
	seqno = (seqno + 1) % 100;
	return result;
}
int machinecrobot::set_coordinate2transfer() { 
	//�ݶ�
	if (machinetype == 1 || !this->initOK) return 0;
	thismessage.robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ ",COOR,1," + thisrobot_common->wafer_json_common_getkey("alignerAngle")
		+ ",;";
	thismessage.ack_robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",COOR";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",COOR";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//ִ�����������
	seqno = (seqno + 1) % 100;
	return result;
}
int machinecrobot::set_parameter_value() { 
	//�ݶ�
	if (machinetype == 1 || !this->initOK) return 0;
	thismessage.robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ ",VALU,1," + thisrobot_common->wafer_json_common_getkey("alignerAngle")
		+ ",;";
	thismessage.ack_robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",VALU";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",VALU";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//ִ�����������
	seqno = (seqno + 1) % 100;
	return result;
}
int machinecrobot::set_inner_lock() { 
	//�ݶ�
	if (machinetype == 1 || !this->initOK) return 0;
	thismessage.robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ ",SETI,1," + thisrobot_common->wafer_json_common_getkey("alignerAngle")
		+ ",;";
	thismessage.ack_robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",SETI";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",SETI";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//ִ�����������
	seqno = (seqno + 1) % 100;
	return result;
}
int machinecrobot::set_curpoi2referposition() { 
	//�ݶ�
	if (machinetype == 1 || !this->initOK) return 0;
	thismessage.robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ ",SETP,1," + thisrobot_common->wafer_json_common_getkey("alignerAngle")
		+ ",;";
	thismessage.ack_robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",SETP";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",SETP";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//ִ�����������
	seqno = (seqno + 1) % 100;
	return result;
}

//Reference Commands///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
string machinecrobot::read_robot_speed() {
	//�ݶ�
	if (machinetype == 1 || !this->initOK) return 0;
	thismessage.robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ ",RDSP,1," + thisrobot_common->wafer_json_common_getkey("alignerAngle")
		+ ",;";
	thismessage.ack_robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",RDSP";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",RDSP";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//ִ�����������
	seqno = (seqno + 1) % 100;
	return to_string(result);
};
string machinecrobot::read_speed_level(int SeqNo) {
	//�ݶ�
	if (machinetype == 1 || !this->initOK) return 0;
	thismessage.robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ ",RDSL,1," + thisrobot_common->wafer_json_common_getkey("alignerAngle")
		+ ",;";
	thismessage.ack_robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",RDSL";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",RDSL";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//ִ�����������
	seqno = (seqno + 1) % 100;
	return to_string(result);
};

string machinecrobot::read_current_position() {
	//�ݶ�
	if (machinetype == 1 || !this->initOK) return 0;
	thismessage.robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ ",RDCP,1," + thisrobot_common->wafer_json_common_getkey("alignerAngle")
		+ ",;";
	thismessage.ack_robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",RDCP";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",RDCP";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//ִ�����������
	seqno = (seqno + 1) % 100;
	return to_string(result);
};

string machinecrobot::read_mapping_result() {
	//�ݶ�
	if (machinetype == 1 || !this->initOK) return 0;
	thismessage.robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ ",RDRE,1," + thisrobot_common->wafer_json_common_getkey("alignerAngle")
		+ ",;";
	thismessage.ack_robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",RDRE";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",RDRE";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//ִ�����������
	seqno = (seqno + 1) % 100;
	return to_string(result);
};

string machinecrobot::read_align_result() {
	//�ݶ�
	if (machinetype == 1 || !this->initOK) return 0;
	thismessage.robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ ",RDAL,1," + thisrobot_common->wafer_json_common_getkey("alignerAngle")
		+ ",;";
	thismessage.ack_robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",RDAL";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",RDAL";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//ִ�����������
	seqno = (seqno + 1) % 100;
	return to_string(result);
};
