#include "pch.h"
#include "machinecommon.h"
string* machinecommon::act_send(string message) {
	string data[2] = { "data1","data2" };//data=是否正确，完整信息
	return data;
}

string* machinecommon::read_send(string message) {
	string data[3] = { "data1","data2","data3" };//data=是否正确，需要结果，以及返回完全结果等
	return data;
}
/*

int machinecommon::action_set_parameter(act_data act_d)
{
	string tatus = read_db_parametr(act_d.need_read_status);//从储存介质中读取需要状态参数（同步读取更安全？）
	if (tatus == act_d.required_status) {//判定状态参数是否满足条件
		string* acceptmessage = act_send(act_d.text);//发送执行指令
		//string acceptmessage = get();//获取返回指令
		if (acceptmessage[0] == act_d.right_condition) {//判断返回指令是否正常
			set_db_parameter(act_d.variable_name, act_d.variable_value);//更新状态参数
			Log(act_d.event_name, Success_act);//更新log成功日志
			return(0);
		}
		else {
			Log(act_d.event_name, Fail_act);//更新log失败日志
			Error(act_d.event_name, Fail_act, acceptmessage[1]);//如果错误更新error
			return(1);
		}
	}
	else {
		Log(act_d.event_name, Fail_condition);//更新log失败日志
		Error(act_d.event_name, Fail_condition, "");//如果错误更新error
		return(1);
	}
}

string machinecommon::read(read_data_type read_d)
{
	string tatus = read_db_parametr(read_d.need_read_status);//读取需要状态参数
	if (tatus == read_d.required_status) {//判定状态参数是否满足条件
		string* acceptmessage = read_send(read_d.text);//发送执行指令
		//string acceptmessage = get();//获取返回指令
		if (acceptmessage[0] == read_d.right_condition) {//判断返回指令是否正常
			Log(read_d.event_name, Success_act);//更新log成功日志
			return(acceptmessage[1]);
		}
		else {
			Log(read_d.event_name, Fail_act);//更新log失败日志
			Error(read_d.event_name, Fail_act, acceptmessage[2]);//如果错误更新error
			return(read_feedback_result_fail);
		}
	}
	else {
		Log(read_d.event_name, Fail_condition);//更新log失败日志
		Error(read_d.event_name, Fail_condition, "");//如果错误更新error
		return(read_feedback_result_fail);
	}
}
*/