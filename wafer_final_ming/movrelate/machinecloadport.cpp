#include "pch.h"
#include "machinecloadport.h"
#include <string>
//设备相关函数
int machinecloadport::machine_action(lp_act_data ans) {
	if (nowstatu == ans.next_status) return true;//表示当前状态符合要求
	if (nowstatu != ans.need_status) return false;//判断是否符合现在的状态,不符合则报错
	thislp_common->clpcommon_serial_send(ans.lp_command);//之后需要进入监听模式
	string thisans = "";
	while (thisans.size() == 0) {
		thisans = thislp_common->clpcommon_serial_receive();//得到接受窗口
	}
	if (thisans == ans.ack_lp_command) {//符合确认函数ACK
		//接下来是专属于连接部分的内容
		thisans = "";
		while (thisans.size() == 0) {
			thisans = thislp_common->clpcommon_serial_receive();//得到结束说明
		}
		if (thisans.substr(0, 3) == "INF") {
			thislp_common->clpcommon_serial_send(ans.fin_lp_command);
		}
		else thislp_common->wafer_error();
	}
	else { thislp_common->wafer_error(); }//不是确认消息则进入错误列表
	nowstatu = ans.next_status;
	return true;
}//该设备的相关流程
bool machinecloadport::set_db_parameter(vector<string> variable_name, vector<int> variable) {
	for (unsigned int i = 0; i < variable.size(); i++) {
		thislp_common->clpcommon_set_varible(variable_name[i], to_string(variable[i]));//变量表格的修改
	}
	return true;
}//该函数是进行变量修改的函数
//可以看出公共类的话可以有个machine_action的虚函数
//数据库的对接操作需要直接修改部分信息的函数
int machinecloadport::machine_check_action(lp_act_data ans) {
	vector<string> varible_name;
	unsigned int rig = 0, lef = 0;//下面是获得了各个需要修改的名字
	while (rig < ans.variable_name.size() && ans.variable_name[rig] != ';') {//该循环的意思在于分割命令
		while (rig < ans.variable_name.size() && ans.variable_name[rig] != ',' && ans.variable_name[rig] != ';') {
			rig++;
		}
		string now = ans.variable_name.substr(lef, rig - lef);
		lef = rig + 1;
		rig++;
		varible_name.push_back(now);//增添变量名
	}
	thislp_common->clpcommon_serial_send(ans.lp_command);//之后需要进入监听模式
	string thisans = "";
	vector<int> vari;
	while (thisans.size() == 0) {
		thisans = thislp_common->clpcommon_serial_receive();//得到接受窗口
	}
	if (thisans.substr(0, 3) == "ACK") {//符合确认函数ACK
		//接下来是专属于连接部分的内容
		unsigned int right = 14, left = 14;
		while (right < thisans.size() && thisans[right] != ';') {//该循环的意思在于分割命令
			vari.push_back(thisans[right] - '0');//增添变量名
			right++;
		}
		machinecloadport::set_db_parameter(varible_name, vari);//这个是更新状态信息的相关，不行就error
	}
	else { thislp_common->wafer_error(); }//不是确认消息则进入错误列表
	return true;
}
int machinecloadport::init() {
	bool ans = 0;
	ans = thislp_common->clpcommon_serial_start(com);//启动串口
	if (ans == 0) return ans;
	ans = thislp_common->clpcommon_dbrs_start();//启动
	return ans;
}//重写初始化虚函数，有关发送消息继承在串口类中，S00以及最后的换行符

bool machinecloadport::setlight01ON() {
	string name = "setlight01ON";
	vector<string> lp_message = thislp_common->lp_getcommand_fromdb(name);//发送执行信息,直接查表，更换设备直接在表里进行命令更换
															//放到公共连接的部分，部分变量放到该对象内部
								//依次是需要的状态、转换的状态、执行的命令、需要修改的变量
	thismessage.need_status = lp_message[1];
	thismessage.next_status = lp_message[2];
	thismessage.lp_command = lp_message[4];
	thismessage.ack_lp_command = lp_message[6];
	thismessage.variable_name = lp_message[3];//其他部分都是需要修改的变量
	int result = machine_check_action(thismessage);//执行相关了流程
	return result;
}//设置警示灯为开
bool machinecloadport::setlight01OFF() {
	string name = "setlight01OFF";
	vector<string> lp_message = thislp_common->lp_getcommand_fromdb(name);//发送执行信息,直接查表，更换设备直接在表里进行命令更换
															//放到公共连接的部分，部分变量放到该对象内部
								//依次是需要的状态、转换的状态、执行的命令、需要修改的变量
	thismessage.need_status = lp_message[1];
	thismessage.next_status = lp_message[2];
	thismessage.lp_command = lp_message[4];
	thismessage.ack_lp_command = lp_message[6];
	thismessage.variable_name = lp_message[3];//其他部分都是需要修改的变量
	int result = machine_check_action(thismessage);//执行相关了流程
	return result;
}//设置警示灯为关
bool machinecloadport::setcassege_exist_ON() {
	string name = "setcassege_exist_ON";
	vector<string> lp_message = thislp_common->lp_getcommand_fromdb(name);//发送执行信息,直接查表，更换设备直接在表里进行命令更换
															//放到公共连接的部分，部分变量放到该对象内部
								//依次是需要的状态、转换的状态、执行的命令、需要修改的变量
	thismessage.need_status = lp_message[1];
	thismessage.next_status = lp_message[2];
	thismessage.lp_command = lp_message[4];
	thismessage.ack_lp_command = lp_message[6];
	thismessage.variable_name = lp_message[3];//其他部分都是需要修改的变量
	int result = machine_check_action(thismessage);//执行相关了流程
	return result;
}//设置晶圆盒存在指示灯为开
bool machinecloadport::setcassege_exist_OFF() {
	string name = "setcassege_exist_OFF";
	vector<string> lp_message = thislp_common->lp_getcommand_fromdb(name);//发送执行信息,直接查表，更换设备直接在表里进行命令更换
															//放到公共连接的部分，部分变量放到该对象内部
								//依次是需要的状态、转换的状态、执行的命令、需要修改的变量
	thismessage.need_status = lp_message[1];
	thismessage.next_status = lp_message[2];
	thismessage.lp_command = lp_message[4];
	thismessage.ack_lp_command = lp_message[6];
	thismessage.variable_name = lp_message[3];//其他部分都是需要修改的变量
	int result = machine_check_action(thismessage);//执行相关了流程
	return result;
}//设置晶圆盒存在指示灯为关

//获得命令指令
int machinecloadport::get_lp_state() {
	return {};
}//获得相关的状态，定义errorcode对应不同的错误
int machinecloadport::get_lp_mapping() {
	string name = "get_lp_mapping";
	vector<string> lp_message = thislp_common->lp_getcommand_fromdb(name);//发送执行信息,直接查表，更换设备直接在表里进行命令更换
															//放到公共连接的部分，部分变量放到该对象内部
								//依次是需要的状态、转换的状态、执行的命令、需要修改的变量
	thismessage.need_status = lp_message[1];
	thismessage.next_status = lp_message[2];
	thismessage.lp_command = lp_message[4];
	thismessage.ack_lp_command = lp_message[6];
	thismessage.variable_name = lp_message[3];//其他部分都是需要修改的变量
	int result = machine_check_action(thismessage);//执行相关了流程
	return result;
}//获得晶圆存在信息

//运动指令
bool machinecloadport::move2dooropened() {
	string name = "move2dooropened";
	vector<string> lp_message = thislp_common->lp_getcommand_fromdb(name);//发送执行信息,直接查表，更换设备直接在表里进行命令更换
	thismessage.need_status = lp_message[1];
	thismessage.next_status = lp_message[2];
	thismessage.lp_command = lp_message[4];
	thismessage.ack_lp_command = lp_message[6];
	thismessage.fin_lp_command = lp_message[7];
	thismessage.variable_name = lp_message[3];//其他部分都是需要修改的变量
	bool result = machine_action(thismessage);//执行相关了流程
	return result;
}//运动到开门位置
bool machinecloadport::move2mappinged() {
	string name = "move2mappinged";
	vector<string> lp_message = thislp_common->lp_getcommand_fromdb(name);//发送执行信息,直接查表，更换设备直接在表里进行命令更换
	thismessage.need_status = lp_message[1];
	thismessage.next_status = lp_message[2];
	thismessage.lp_command = lp_message[4];
	thismessage.ack_lp_command = lp_message[6];
	thismessage.fin_lp_command = lp_message[7];
	thismessage.variable_name = lp_message[3];//其他部分都是需要修改的变量
	bool result = machine_action(thismessage);//执行相关了流程
	return result;
}//mapping
bool machinecloadport::move2init() {
	string name = "set_init_robot";
	vector<string> lp_message = thislp_common->lp_getcommand_fromdb(name);//发送执行信息,直接查表，更换设备直接在表里进行命令更换
															//放到公共连接的部分，部分变量放到该对象内部
								//依次是需要的状态、转换的状态、执行的命令、需要修改的变量
	thismessage.need_status = lp_message[1];
	thismessage.next_status = lp_message[2];
	thismessage.lp_command = lp_message[4];
	thismessage.ack_lp_command = lp_message[6];
	thismessage.fin_lp_command = lp_message[7];
	thismessage.variable_name = lp_message[3];//其他部分都是需要修改的变量
	int result = machine_action(thismessage);//执行相关了流程
	return result;
}//回归初始位置
bool machinecloadport::move2clamped() {
	string name = "move2clamped";
	vector<string> lp_message = thislp_common->lp_getcommand_fromdb(name);//发送执行信息,直接查表，更换设备直接在表里进行命令更换
															//放到公共连接的部分，部分变量放到该对象内部
								//依次是需要的状态、转换的状态、执行的命令、需要修改的变量
	thismessage.need_status = lp_message[1];
	thismessage.next_status = lp_message[2];
	thismessage.lp_command = lp_message[4];
	thismessage.ack_lp_command = lp_message[6];
	thismessage.fin_lp_command = lp_message[7];
	thismessage.variable_name = lp_message[3];//其他部分都是需要修改的变量
	int result = machine_action(thismessage);//执行相关了流程
	return result;
}//单步：运动到钳锁位置，后续的单步操作还没写
bool machinecloadport::back2init() {
	string name = "back2init";
	vector<string> lp_message = thislp_common->lp_getcommand_fromdb(name);//发送执行信息,直接查表，更换设备直接在表里进行命令更换
															//放到公共连接的部分，部分变量放到该对象内部
								//依次是需要的状态、转换的状态、执行的命令、需要修改的变量
	thismessage.need_status = lp_message[1];
	thismessage.next_status = lp_message[2];
	thismessage.lp_command = lp_message[4];
	thismessage.ack_lp_command = lp_message[6];
	thismessage.fin_lp_command = lp_message[7];
	thismessage.variable_name = lp_message[3];//其他部分都是需要修改的变量
	int result = machine_action(thismessage);//执行相关了流程
	return result;
}
machinecloadport::~machinecloadport() {

}