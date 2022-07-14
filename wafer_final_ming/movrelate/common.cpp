#include "pch.h"
#include "common.h"
bool common::wafer_log_write(int mode, string ans) {
	return false;
}
bool common::wafer_error() {
	//����ͣ���еĶ��������ع��ʼ����Ȼ�������
	//this->wafer_log_write(ERROR,to_string(errortype));
	return false;
}
bool common::wafer_db_start() {
	if (this_wafer_pgsql->wafer_db.IsOpen()) return true;//�ж����ݿ��Ƿ��Ѿ�����
	return this_wafer_pgsql->wafer_pgsql_ConnectToDB();//û�����ӵĻ���������
}
bool common::wafer_db_close() {
	this_wafer_pgsql->wafer_pgsql_CloseDB();
	return true;
}
string common::wafer_json_common_getkey(string keyname) {
	return this_wafer_json->getjson_keyvalue(keyname);
}
bool common::wafer_json_common_setkey(string keyname, string keyans) {
	return this_wafer_json->setjson_keyvalue(keyname, keyans);
}
bool common::wafer_json_common_setkey(string keyname1, string keyname2, string keyans) {
	return this_wafer_json->setjson_keyvalue(keyname1,keyname2, keyans);
}
string common::wafer_json_common_getarray(string keyname, int key) {
	vector<int> ming = this_wafer_json->getjson_array(keyname);
	return to_string(ming[key]);
}
bool common::wafer_json_common_setarray(string keyname, int key, string keyans) {
	return this_wafer_json->setjson_arrayvalue(keyname, key, keyans);
}
vector<int> common::wafer_json_common_getarray(string keyname) {
	return this_wafer_json->getjson_array(keyname);
}



bool robotcommon::wafer_log_write(int mode, string ans) {
	ans += "',";
	string wafer_log_write_this;
	string xing = "'" + robot_dbcommon_rs->wafer_pgsql_select("SELECT current_timestamp") + "'";
	switch (mode) {
	case LOG_Send:
		wafer_log_write_this = "'Send','yaskawarobot','";
		ans = "INSERT INTO wafer_log VALUES(" + wafer_log_write_this + ans + xing + ");";
		break;
	case LOG_Receive:
		wafer_log_write_this = "'Receive','yaskawarobot','";
		ans = "INSERT INTO wafer_log VALUES(" + wafer_log_write_this + ans + xing + ");";
		break;
	case LOG_ERROR:
		wafer_log_write_this = "'ERROR','yaskawarobot','";
		ans = "INSERT INTO wafer_log VALUES(" + wafer_log_write_this + ans + xing + ");";
		break;
	default:
		return false;
		break;
	}
	robot_dbcommon_rs->wafer_pgsql_Excsql(ans);
	return true;
}//�԰����ĵ�����log����
bool robotcommon::robotcommon_dbrs_start() {
	if (this->wafer_db_start()) {//���ݿ⿪���˲��ܲ���,Ȼ�󽫸����ݼ���һ�����ݿ�
		robot_dbcommon_rs = new Wafer_Pgsql_Record(&(this->this_wafer_pgsql->wafer_db));
		db_connected = true;
		return robot_dbcommon_rs->wafer_pgsql_BindToDB();
	}
	else return false;
}//���ݽ�����ɣ�һ�������ָ��һ������
bool robotcommon::wafer_error() {
	string wafer_log_write_this;
	string xing = "'" + robot_dbcommon_rs->wafer_pgsql_select("SELECT current_timestamp") + "'";
	wafer_log_write_this = "'ERROR','yaskawarobot','";
	string ans = "INSERT INTO wafer_log VALUES(" + wafer_log_write_this + "unkownerror'," + xing + ");";
	robot_dbcommon_rs->wafer_pgsql_Excsql(ans);
	return true;
}//���豸���������ˣ���Ҫ����
bool robotcommon::robotcommon_tcp_start(string ip,int port) {
	this->robot_socket = new Wafer_Csocket(ip,port);
	return this->robot_socket->Wafer_Csocket_Init();
}//socketͨѶ
bool robotcommon::robotcommon_tcp_send(string ans) {
	if (!this->robot_socket->tcp_isopen()) {
		this->wafer_log_write(LOG_ERROR, "tcp not connect!");
		return false;
	}
	this->wafer_log_write(LOG_Send, ans);
	return this->robot_socket->Wafer_Csocket_Send(ans);
}//socket������Ϣ
bool robotcommon::robotcommon_set_varible(string variable_name, string variable) {
	string ans = "UPDATE crobot_vari SET robot_variable ="
		+ variable + " WHERE crobot_vari_name = '" + variable_name + "';";
	this->this_wafer_pgsql->wafer_pgsql_Excsql(ans);
	return true;
}
vector<string> robotcommon::robot_getcommand_fromdb(string ans) {
	vector<string> this_variable_name;
	string ans1 = "";
	if (db_connected == true) {//���ݿ������˵
		ans1 += "SELECT* FROM crobot_conv WHERE funciton_name = '" + ans + "';";
		this_variable_name = this->robot_dbcommon_rs->wafer_pgsql_select_row(ans1);
	}
	return this_variable_name;
}
string robotcommon::wafer_json_common_getkey(string keyname) {
	return this_wafer_json->getjson_keyvalue(keyname);
}
vector<int> robotcommon::wafer_json_common_getarray(string keyname) {
	return this_wafer_json->getjson_array(keyname);
}
string robotcommon::wafer_json_common_getarray(string keyname, int key) {
	vector<int> ming = this_wafer_json->getjson_array(keyname);
	return to_string(ming[key]);
}
bool robotcommon::wafer_json_common_setarray(string keyname, int key, string keyans) {
	return this_wafer_json->setjson_arrayvalue(keyname, key, keyans);
}
bool robotcommon::wafer_json_common_setkey(string keyname, string keyans){
	return this_wafer_json->setjson_keyvalue(keyname, keyans);
}
bool robotcommon::wafer_json_common_setkey(string keyname1, string keyname2, string keyans) {
	return this_wafer_json->setjson_keyvalue(keyname1, keyname2, keyans);
}



bool clpcommon::wafer_log_write(int mode, string ans) {//����ʲô��ʲô
	int i = 0;
	for (i = 0; i < ans.size(); i++) {
		if (ans[i] == '\0') ans = ans.substr(0, i);
	}
	string ming = ans + "',";
	//ans += "',";
	string wafer_log_write_this;
	string xing = "'" + lp_dbcommon_rs->wafer_pgsql_select("SELECT current_timestamp") + "'";
	string mingxing = ming + xing + ");";
	switch (mode) {
	case LOG_Send:
		wafer_log_write_this = "'Send','loadport','";
		ans = "INSERT INTO wafer_log VALUES(" + wafer_log_write_this + mingxing;
		break;
	case LOG_Receive:
		wafer_log_write_this = "'Receive','loadport','";
		ans = "INSERT INTO wafer_log VALUES(" + wafer_log_write_this + mingxing;
		break;
	case LOG_ERROR:
		wafer_log_write_this = "'ERROR','loadport','";
		ans = "INSERT INTO wafer_log VALUES(" + wafer_log_write_this + mingxing;
		break;
	default:
		return false;
		break;
	}
	lp_dbcommon_rs->wafer_pgsql_Excsql(ans);
	return true;
}
bool clpcommon::clpcommon_serial_start(char* com) {
	this->lp_serial_port = new Wafer_Cserial(com);//��ʼ�������ӿڣ���ʱ��Ҫ�̶��ģ�������Ϊ�鿴��
	return this->lp_serial_port->wafer_serial_open();//�������ڲ�������ȷ���
}//���ڿ���
bool clpcommon::wafer_error() {//��error��Ϣ��������ȼ�����Ҫ�����Ļ�����Ҫȫ����ʼ�������¼��
	string wafer_log_write_this;
	string xing = "'" + lp_dbcommon_rs->wafer_pgsql_select("SELECT current_timestamp") + "'";
	wafer_log_write_this = "'Send','loadport','";
	string ans = "INSERT INTO wafer_log VALUES(" + wafer_log_write_this + "unkownerror'," + xing + ");";
	lp_dbcommon_rs->wafer_pgsql_Excsql(ans);
	return true;
}
bool clpcommon::clpcommon_dbrs_start() {
	if (this->wafer_db_start()) {//���ݿ⿪���˲��ܲ���
		lp_dbcommon_rs = new Wafer_Pgsql_Record(&(this->this_wafer_pgsql->wafer_db));
		db_connected = true;
		return lp_dbcommon_rs->wafer_pgsql_BindToDB();
	}
	else return false;
}
bool clpcommon::clpcommon_serial_send(string ans) {
	if (!lp_serial_port->wafer_serial_isopen()) return false;//��û������
	this->wafer_log_write(LOG_Send, ans);
	return lp_serial_port->wafer_serial_Send(ans);
}//������Ϣ����Ҫ�������豸������,���ص���Ϣ����������
bool clpcommon::clpcommon_set_varible(string variable_name, string variable) {
	string ans = "UPDATE cloadport_vari SET lp_variable ="
		+ variable + " WHERE lp_variable_name = '" + variable_name + "';";
	this->this_wafer_pgsql->wafer_pgsql_Excsql(ans);
	return true;
}
vector<string> clpcommon::lp_getcommand_fromdb(string ans) {
	vector<string> this_variable_name;
	string ans1="";
	if (db_connected == true) {//���ݿ������˵
		ans1 += "SELECT* FROM cloadport_conv WHERE funciton_name = '" + ans + "';";
		this_variable_name = this->lp_dbcommon_rs->wafer_pgsql_select_row(ans1);
	}
	return this_variable_name;
}
string clpcommon::wafer_json_common_getkey(string keyname) {
	return this_wafer_json->getjson_keyvalue(keyname);
}
vector<int> clpcommon::wafer_json_common_getarray(string keyname) {
	return this_wafer_json->getjson_array(keyname);
}
string clpcommon::wafer_json_common_getarray(string keyname, int key) {
	vector<int> ming = this_wafer_json->getjson_array(keyname);
	return to_string(ming[key]);
}
bool clpcommon::wafer_json_common_setarray(string keyname, int key, string keyans) {
	return this_wafer_json->setjson_arrayvalue(keyname, key, keyans);
}
bool clpcommon::wafer_json_common_setkey(string keyname, string keyans) {
	return this_wafer_json->setjson_keyvalue(keyname, keyans);
}
bool clpcommon::wafer_json_common_setkey(string keyname1, string keyname2, string keyans) {
	return this_wafer_json->setjson_keyvalue(keyname1, keyname2, keyans);
}