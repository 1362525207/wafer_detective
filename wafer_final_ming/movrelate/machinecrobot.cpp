
#include "pch.h"
#include "machinecrobot.h"
#include "machinecommon.h"
/*实现功能：
* 1、逻辑子函数（基本动作函数）
* 2、状态变量更新
* 3、Log,Error更新
* "$, 1,"
* ", INIT, 1, 1, G,"
*/
using namespace std;
volatile bool checkOnReceive = false;
bool machinecrobot::set_db_parameter(vector<string> variable_name, vector<int> variable) {
	for (unsigned int i = 0; i < variable.size(); i++) {
		thisrobot_common->robotcommon_set_varible(variable_name[i], to_string(variable[i]));//变量表格的修改
	}
	return true;
}
int machinecrobot::machine_action(robot_act_data ans) {
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
	thisrobot_common->robotcommon_tcp_send(ans.robot_command);//之后需要进入监听模式
	string thisans = "";
	vector<int> vari;
	while (thisans.size() == 0) {
		while(!checkOnReceive){ }
		checkOnReceive = false;
		if (thisrobot_common->ismessageget()) {
			thisans = thisrobot_common->robotcommon_tcp_receive();
		}
	}//得到接受窗口
	if (thisans.substr(10, 4) == "0000") {//符合确认函数ACK
		//接下来是专属于连接部分的内容
		thisans = "";
		while (thisans.size() == 0) {
			if (thisrobot_common->ismessageget()) {
				thisans = thisrobot_common->robotcommon_tcp_receive();
			}
		}
		if (thisans.substr(0, 5) == "!0000") {//返回的错误代码没有报错
			unsigned int right = 5, left = 5;
			while (right < thisans.size() && thisans[right] != ';') {//该循环的意思在于分割命令
				while (right < thisans.size() && thisans[right] != ',' && thisans[right] != ';') {
					right++;
				}
				string now = thisans.substr(left, right - left);
				left = right + 1;
				right++;
				vari.push_back(stoi(now));//增添变量名
			}
			set_db_parameter(varible_name, vari);//这个是更新状态信息的相关，不行就error
		}
		else thisrobot_common->wafer_error();

	}
	else { thisrobot_common->wafer_error(); }//不是确认消息则进入错误列表
	//else thislp_common->error;
	return true;
}//该设备的相关流程
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
	vector<int> now = thisrobot_common->wafer_json_common_getarray(getcass);//槽的数组
	int hhh = -1;
	for (unsigned int i = 0; i <= now.size(); i++) {
		if (i == now.size()) {
			return "-1";//表示已经查完了所有的片但都没了，！！！需要加入处理机制
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
	vector<int> now = thisrobot_common->wafer_json_common_getarray(getcass);//槽的数组
	for (unsigned int i = 0; i <= now.size(); i++) {
		if (i == now.size()) {
			return "-1";//表示已经查完了所有的片但都有了，！！！需要加入处理机制
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
	int result = machine_action(thismessage);//执行相关了流程
	seqno=(seqno+1)%100;
	return result;
};
int machinecrobot::act_getfromLP(int blade) {
	if (blade == 1) {
		if (isplade1on) return 1;//片1上有晶圆，不能操作
	}
	else { if (isplage2on) return 1; }//片2上有晶圆，不能操作
	thismessage.robot_command = "$,1," + to_string(seqno) + ",MTRS,G," 
		+ robot_get_getcassage()+","+ robot_get_getslot() + ",A,"+to_string(blade)+",Gb; ";
	thismessage.ack_robot_command = "$,1," + to_string(seqno) +
		+ "0000" +  ",MTRS";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",MTRS";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//执行相关了流程
	seqno = (seqno + 1) % 100;
	if (blade == 1) {
		isplade1on = true;
	}
	else { isplage2on = true; }//标志有晶圆了
	return result;
};
int machinecrobot::act_getfromaligner(int blade) {
	if (blade == 1) {
		if (isplade1on) return 1;//片1上有晶圆，不能操作
	}
	else { if (isplage2on) return 1; }//片2上有晶圆，不能操作
	if (!this->initOK) return 0;
	thismessage.robot_command = "$,1," + to_string(seqno) + ",MTRS,G,"
		+ "P01,00,A,"+ to_string(blade) + ",Gb; ";
	thismessage.ack_robot_command = "$,1," + to_string(seqno) +
		+"0000" + ",MTRS";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",MTRS";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//执行相关了流程
	seqno = (seqno + 1) % 100;
	this->isonalign = false;
	if (blade == 1) {
		isplade1on = true;
	}
	else { isplage2on = true; }//标志有晶圆了
	return result;
}
int machinecrobot::act_put2LP(int blade) {
	if (blade == 1) {
		if (!isplade1on) return 0;//片1上没有晶圆，不能操作
	}
	else { if (!isplage2on) return 0; }//片2上没有晶圆，不能操作
	if (!this->initOK) return 0;
	thismessage.robot_command = "$,1," + to_string(seqno) + ",MTRS,P,"
		+ robot_get_destcassage() + "," + robot_get_destslot()
		+ ",A,"+ to_string(blade) + ", Gb; ";
	thismessage.ack_robot_command = "$,1," + to_string(seqno) +
		+"0000" + ",MTRS";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",MTRS";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//执行相关了流程
	seqno = (seqno + 1) % 100;
	if (blade == 1) {
		isplade1on = false;
	}
	else { isplage2on = false; }//标志没有晶圆了
	return result;
}
int machinecrobot::act_put2aligner(int blade) {
	if (this->isonalign) return 1;//判断是否已经在上面了
	if (blade == 1) {
		if (!isplade1on) return 0;//片1上没有晶圆，不能操作
	}
	else { if (!isplage2on) return 0; }//片2上没有晶圆，不能操作
	if (!this->initOK) return 0;
	thismessage.robot_command = "$,1," + to_string(seqno) + ",MTRS,P,"
		+ "P01,00,A,"+ to_string(blade) + ",Gb; ";
	thismessage.ack_robot_command = "$,1," + to_string(seqno) +
		+"0000" + ",MTRS";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",MTRS";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//执行相关了流程
	seqno = (seqno + 1) % 100;
	this->isonalign = true;
	if (blade == 1) {
		isplade1on = false;
	}
	else { isplage2on = false; }//标志没有晶圆了
	return result;
}
int machinecrobot::act_test_robot() { 
	//标注不能
	if (machinetype == 1 || !this->initOK) return 0;
	thismessage.robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ ",MALN,0," + thisrobot_common->wafer_json_common_getkey("alignerAngle")
		+ ",;";
	thismessage.ack_robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",MALN";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",MALN";
	thismessage.variable_name = "axis_position;";//这里还有很多东西！！！
	int result = machine_action(thismessage);//执行相关了流程
	seqno = (seqno + 1) % 100;
	return result;
}
int machinecrobot::act_robot_mapping() {
	//暂定
	if (machinetype == 1 || !this->initOK) return 0;
	thismessage.robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ ",MADL,0," + thisrobot_common->wafer_json_common_getkey("alignerAngle")
		+ ",;";
	thismessage.ack_robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",MADL";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",MADL";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//执行相关了流程
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
	thismessage.variable_name = "axis_position;";//这里还有很多东西！！！
	int result = machine_action(thismessage);//执行相关了流程
	seqno = (seqno + 1) % 100;
	return result;
}

//machinecrobot::control Commands
int machinecrobot::con_emergence_stop() { 
	//暂定
	if (machinetype == 1 || !this->initOK) return 0;
	thismessage.robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ ",CONV,0," + thisrobot_common->wafer_json_common_getkey("alignerAngle")
		+ ",;";
	thismessage.ack_robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",CONV";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",CONV";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//执行相关了流程
	seqno = (seqno + 1) % 100;
	return result;
};
int machinecrobot::con_decelar_stop() {
	//暂定
	if (machinetype == 1 || !this->initOK) return 0;
	thismessage.robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ ",STOP,0," + thisrobot_common->wafer_json_common_getkey("alignerAngle")
		+ ",;";
	thismessage.ack_robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",STOP";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",STOP";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//执行相关了流程
	seqno = (seqno + 1) % 100;
	return result;
};
int machinecrobot::con_servo_power() {
	//暂定
	if (machinetype == 1 || !this->initOK) return 0;
	thismessage.robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ ",POWE,0," + thisrobot_common->wafer_json_common_getkey("alignerAngle")
		+ ",;";
	thismessage.ack_robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",POWE";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",POWE";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//执行相关了流程
	seqno = (seqno + 1) % 100;
	return result;
};
int machinecrobot::con_clear_error() { 
	//暂定
	if (machinetype == 1 || !this->initOK) return 0;
	thismessage.robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ ",CLCS,0," + thisrobot_common->wafer_json_common_getkey("alignerAngle")
		+ ",;";
	thismessage.ack_robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",CLCS";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",CLCS";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//执行相关了流程
	seqno = (seqno + 1) % 100;
	return result;
};

//machinecrobot::setting Commands///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int machinecrobot::set_robot_speed(int SeqNo, int Speed) {
	//暂定
	if (machinetype == 1 || !this->initOK) return 0;
	thismessage.robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ ",SETS,0," + thisrobot_common->wafer_json_common_getkey("alignerAngle")
		+ ",;";
	thismessage.ack_robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",SETS";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",SETS";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//执行相关了流程
	seqno = (seqno + 1) % 100;
	return result;
};
int machinecrobot::set_seppd_level() {
	//暂定
	if (machinetype == 1 || !this->initOK) return 0;
	thismessage.robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ ",SETL,0," + thisrobot_common->wafer_json_common_getkey("alignerAngle")
		+ ",;";
	thismessage.ack_robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",SETL";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",SETL";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//执行相关了流程
	seqno = (seqno + 1) % 100;
	return result;
}
int machinecrobot::set_current2transfer() { 
	//暂定
	if (machinetype == 1 || !this->initOK) return 0;
	thismessage.robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ ",CURR,1," + thisrobot_common->wafer_json_common_getkey("alignerAngle")
		+ ",;";
	thismessage.ack_robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",CURR";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",CURR";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//执行相关了流程
	seqno = (seqno + 1) % 100;
	return result;
}
int machinecrobot::set_coordinate2transfer() { 
	//暂定
	if (machinetype == 1 || !this->initOK) return 0;
	thismessage.robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ ",COOR,1," + thisrobot_common->wafer_json_common_getkey("alignerAngle")
		+ ",;";
	thismessage.ack_robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",COOR";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",COOR";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//执行相关了流程
	seqno = (seqno + 1) % 100;
	return result;
}
int machinecrobot::set_parameter_value() { 
	//暂定
	if (machinetype == 1 || !this->initOK) return 0;
	thismessage.robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ ",VALU,1," + thisrobot_common->wafer_json_common_getkey("alignerAngle")
		+ ",;";
	thismessage.ack_robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",VALU";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",VALU";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//执行相关了流程
	seqno = (seqno + 1) % 100;
	return result;
}
int machinecrobot::set_inner_lock() { 
	//暂定
	if (machinetype == 1 || !this->initOK) return 0;
	thismessage.robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ ",SETI,1," + thisrobot_common->wafer_json_common_getkey("alignerAngle")
		+ ",;";
	thismessage.ack_robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",SETI";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",SETI";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//执行相关了流程
	seqno = (seqno + 1) % 100;
	return result;
}
int machinecrobot::set_curpoi2referposition() { 
	//暂定
	if (machinetype == 1 || !this->initOK) return 0;
	thismessage.robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ ",SETP,1," + thisrobot_common->wafer_json_common_getkey("alignerAngle")
		+ ",;";
	thismessage.ack_robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",SETP";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",SETP";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//执行相关了流程
	seqno = (seqno + 1) % 100;
	return result;
}

//Reference Commands///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
string machinecrobot::read_robot_speed() {
	//暂定
	if (machinetype == 1 || !this->initOK) return 0;
	thismessage.robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ ",RDSP,1," + thisrobot_common->wafer_json_common_getkey("alignerAngle")
		+ ",;";
	thismessage.ack_robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",RDSP";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",RDSP";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//执行相关了流程
	seqno = (seqno + 1) % 100;
	return to_string(result);
};
string machinecrobot::read_speed_level(int SeqNo) {
	//暂定
	if (machinetype == 1 || !this->initOK) return 0;
	thismessage.robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ ",RDSL,1," + thisrobot_common->wafer_json_common_getkey("alignerAngle")
		+ ",;";
	thismessage.ack_robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",RDSL";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",RDSL";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//执行相关了流程
	seqno = (seqno + 1) % 100;
	return to_string(result);
};

string machinecrobot::read_current_position() {
	//暂定
	if (machinetype == 1 || !this->initOK) return 0;
	thismessage.robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ ",RDCP,1," + thisrobot_common->wafer_json_common_getkey("alignerAngle")
		+ ",;";
	thismessage.ack_robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",RDCP";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",RDCP";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//执行相关了流程
	seqno = (seqno + 1) % 100;
	return to_string(result);
};

string machinecrobot::read_mapping_result() {
	//暂定
	if (machinetype == 1 || !this->initOK) return 0;
	thismessage.robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ ",RDRE,1," + thisrobot_common->wafer_json_common_getkey("alignerAngle")
		+ ",;";
	thismessage.ack_robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",RDRE";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",RDRE";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//执行相关了流程
	seqno = (seqno + 1) % 100;
	return to_string(result);
};

string machinecrobot::read_align_result() {
	//暂定
	if (machinetype == 1 || !this->initOK) return 0;
	thismessage.robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ ",RDAL,1," + thisrobot_common->wafer_json_common_getkey("alignerAngle")
		+ ",;";
	thismessage.ack_robot_command = "$," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",RDAL";
	thismessage.fin_robot_command = "!," + to_string(machinetype) + "," + to_string(seqno)
		+ "0000" + ",RDAL";
	thismessage.variable_name = "axis_position;";
	int result = machine_action(thismessage);//执行相关了流程
	seqno = (seqno + 1) % 100;
	return to_string(result);
};
