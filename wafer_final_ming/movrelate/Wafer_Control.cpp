#include "pch.h"
#include "Wafer_Control.h"
bool Wafer_Control::wafercontrol_con_init() {
	if (islp_connect == false || istcp_connect == false) {
		AfxMessageBox(_T("请先启动网口和串口！"));
		return false;
	}
	nowlp.push_back(wafer_control_lp1);
	nowlp.push_back(wafer_control_lp2);//将设备放入设备池
	alllp = &nowlp;
	//将所有的状态放入哈希表，每一个哈希表维护一个状态机的各个状态
	ONInitpos* intpos1 = new ONInitpos(wafer_control_fsm_blade1, wafer_control_robot, wafer_control_align, alllp);
	ONInitpos* intpos2 = new ONInitpos(wafer_control_fsm_blade2, wafer_control_robot, wafer_control_align, alllp);
	ONcassage* caspos1 = new ONcassage(wafer_control_fsm_blade1, wafer_control_robot, wafer_control_align, alllp);
	ONcassage* caspos2 = new ONcassage(wafer_control_fsm_blade2, wafer_control_robot, wafer_control_align, alllp);
	ONaligner* alipos1 = new ONaligner(wafer_control_fsm_blade1, wafer_control_robot, wafer_control_align, alllp);
	ONaligner* alipos2 = new ONaligner(wafer_control_fsm_blade2, wafer_control_robot, wafer_control_align, alllp);
	ONfinished* finpos1 = new ONfinished(wafer_control_fsm_blade1, wafer_control_robot, wafer_control_align, alllp);
	ONfinished* finpos2 = new ONfinished(wafer_control_fsm_blade2, wafer_control_robot, wafer_control_align, alllp);
	wafer_control_status1[0] = intpos1; wafer_control_status2[0] = intpos2;
	wafer_control_status1[1] = caspos1; wafer_control_status2[1] = caspos2;
	wafer_control_status1[2] = alipos1; wafer_control_status2[2] = alipos2;
	wafer_control_status1[3] = finpos1; wafer_control_status2[3] = finpos2;

	isthisinit = true;
	return true;
}//表示后台开始工作，让所有的对象初始化
bool Wafer_Control::wafercontrol_con_serialcon(int port, string com) {
	return true;
}//让所有的串口连接
bool Wafer_Control::wafercontrol_con_tcpcon() {
	return true;
}//让所有的网口连接
bool Wafer_Control::wafercontrol_con_fsmrestart() {
	if (isthisinit == false) {
		AfxMessageBox(_T("请先初始化！"));
		return false;
	}
	wafer_control_fsm_blade1->Clear();
	bool ans=true;
	int cnt = 0;
	for (int i = 0; i < wafer_control_status1.size(); i++) {
		if (i == 2 && jsontext->getjson_keyvalue("isalign") == "0") continue;
		ans=wafer_control_fsm_blade1->Register(cnt, wafer_control_status1[i]);
		ans=wafer_control_fsm_blade2->Register(cnt, wafer_control_status2[i]);
		cnt++;
	}//所有的状态机进行状态的重新更新
	wafer_control_fsm_blade1->SetInitState(0);
	wafer_control_fsm_blade2->SetInitState(0);
	return ans;
}//让状态机更新
bool  Wafer_Control::wafercontrol_con_fsmconver() {
	wafer_control_fsm_blade1->SetInitState(1);
	wafer_control_fsm_blade2->SetInitState(1);
	return true;
}



bool Wafer_Control::wafercontrol_set_serialport(string com1, string com2, int br) {
	char* nowchar = const_cast<char*>(com1.c_str());
	wafer_control_lp1 = new machinecloadport(this->wafer_pgsql, nowchar);
	nowchar = const_cast<char*>(com2.c_str());
	wafer_control_lp2 = new machinecloadport(this->wafer_pgsql, nowchar);
	islp_connect = true;//重新设置连接
	return true;
}//设置串口信息包括端口和波特率
bool Wafer_Control::wafercontrol_set_tcpipport(string ip, int port1, int port2) {
	wafer_control_robot = new machinecrobot(1, wafer_pgsql, ip, port1);
	wafer_control_align = new machinecrobot(2, wafer_pgsql, ip, port2);
	istcp_connect = true;
	return true;
}//设置网口的连接ip和串口
bool Wafer_Control::wafercontrol_set_robotvel() {
	if (isthisinit == false) {
		AfxMessageBox(_T("请先初始化！"));
		return false;
	}
	return wafer_control_robot->set_seppd_level();
}//设置机械臂速度
bool Wafer_Control::wafercontrol_set_alirad(int rad) {
	string ans = to_string(rad)+"000";
	int len = 8-ans.size();
	while (len--) ans = "0" + ans;
	return jsontext->setjson_keyvalue("alignerAngle",ans); //像这种函数就可以通过json进行过度
} //设置寻位角度
bool Wafer_Control::wafercontrol_set_errorclc() {
	wafer_control_robot->con_clear_error();
	wafer_control_align->con_clear_error();
	AfxMessageBox(_T("所有设备错误清楚完毕！"));
	return true;
}//所有的错误清除
bool Wafer_Control::wafercontrol_set_destcassage(string cas) {
	return jsontext->setjson_keyvalue("Dest Cassette stage", cas);
}//设置目标晶圆盒
bool Wafer_Control::wafercontrol_set_srccassage(string cas) {
	return jsontext->setjson_keyvalue("Source Cassette stage", cas);
}//设置源晶圆盒

bool Wafer_Control::wafercontrol_getsourceslot() {
	string cas = wafercontrol_get_srccassage(); cas += "Slot";
	vector<int> ans = jsontext->getjson_array(cas);//获得晶片相关的储存信息
	for (auto i : ans) if (i == 1) return false;
	return true;
}
bool Wafer_Control::wafercontrol_getdestcaslot() {
	string cas = wafercontrol_get_destcassage(); cas += "Slot";
	vector<int> ans = jsontext->getjson_array(cas);//获得晶片相关的储存信息
	for (auto i : ans) if (i == 0) return false;
	return true;
}
bool Wafer_Control::wafercontrol_get_lpison() {
	return true;
}//获得晶圆盒子是否已经安上了
string Wafer_Control::wafercontrol_get_mappinged(string cas) {
	cas += "Slot";
	string back = "";
	vector<int> ans=jsontext->getjson_array(cas);//获得晶片相关的储存信息
	for (auto i : ans) back += to_string(i);
	return back;
}//获得mapping信息
int Wafer_Control::wafercontrol_get_movevel() {
	return stoi(wafer_control_robot->read_speed_level(1));
}//获得运动速度
string Wafer_Control::wafercontrol_get_nowstatus(int blade) {
	vector<string> ming = { "initpos","onlppos","alipos","finpos" };
	int nowstate = 0;
	if (blade == 1)
	{
		WaferStatus* now = wafer_control_fsm_blade1->all_states[wafer_control_fsm_blade1->curState];
		for (auto i : wafer_control_status1) {
			if (i.second == now) return ming[i.first];
		}
	}
	else if (blade == 2) {
		WaferStatus* now = wafer_control_fsm_blade2->all_states[wafer_control_fsm_blade2->curState];
		for (auto i : wafer_control_status2) {
			if (i.second == now) return ming[i.first];
		}
	}
	return "error";
}//获得现在的状态
string Wafer_Control::wafercontrol_get_nextstatus() {
	return "";
}//获得将来的状态
string Wafer_Control::wafercontrol_get_destcassage() {
	return jsontext->getjson_keyvalue("Dest Cassette stage");
}//获得目标晶圆盒
string Wafer_Control::wafercontrol_get_srccassage() {
	return jsontext->getjson_keyvalue("Source Cassette stage");
}//获得源晶圆盒
int Wafer_Control::wafercontrol_get_alirad() {
	string ans = jsontext->getjson_keyvalue("alignerAngle");
	int start = 0;
	for (unsigned int i = 0; i < ans.size(); i++) {
		if (ans[i] == '0') continue;
		else start = i;
	}
	return stoi(ans.substr(start, 5 - start));
}//获得寻位角度

bool Wafer_Control::wafercontrol_mov_allinit() {
	bool ans = wafer_control_fsm_blade1->Tick(_machine_all_init);//该设备由于是对lp进行初始化
	if (ans == 0) {
		AfxMessageBox(_T("运动初始化失败！"));
	}
	else {
		AfxMessageBox(_T("所有设备伺服启动！"));
	}
	wafer_control_fsm_blade2->TransStatus(1);//直接跳到下一个状态
	return true;
}//运动的初始化
bool Wafer_Control::wafercontrol_mov_allfor1() {
	wafer_control_fsm_blade1->Tick(_machine_mov2getfromlp);
	wafer_control_fsm_blade1->Tick(_machine_mov2getfromal);
	wafer_control_fsm_blade2->Tick(_machine_mov2getfromlp);
	wafer_control_fsm_blade2->Tick(_machine_mov2getfromal);
	wafer_control_fsm_blade1->Tick(_machine_mov2finished);
	wafer_control_fsm_blade2->Tick(_machine_mov2finished);
	return true;
}//一次完整的循环一步
bool Wafer_Control::wafercontrol_mov_align(int blade) {
	if(blade==1) return wafer_control_fsm_blade1->Tick(_machine_mov2aligned);
	if (blade == 2) return wafer_control_fsm_blade2->Tick(_machine_mov2aligned);
	return true;
}//寻位
bool Wafer_Control::wafercontrol_mov_2mappinged(int blade) {
	if (blade == 1) return wafer_control_fsm_blade1->Tick(_machine_mov2mappinged);
	if (blade == 2) return wafer_control_fsm_blade2->Tick(_machine_mov2mappinged);
}//lp移动到门开启
bool Wafer_Control::wafercontrol_mov_2lpinit(int blade) {
	if (blade == 1) return wafer_control_fsm_blade1->Tick(_machine_all_fin);
	if (blade == 2) return wafer_control_fsm_blade2->Tick(_machine_all_fin);
	return true;
}//lp移动关闭位置
bool Wafer_Control::wafercontrol_mov_getfromlp(int blade) {
	if (blade == 1) return wafer_control_fsm_blade1->Tick(_machine_mov2getfromlp);
	if (blade == 2) return wafer_control_fsm_blade2->Tick(_machine_mov2getfromlp);
	return true;
}//机械臂从lp取片
bool Wafer_Control::wafercontrol_mov_getfromalig(int blade) {
	if (blade == 1) return wafer_control_fsm_blade1->Tick(_machine_mov2getfromal);
	if (blade == 2) return wafer_control_fsm_blade2->Tick(_machine_mov2getfromal);
	return true;
}//机械臂从align取片
bool Wafer_Control::wafercontrol_mov_put2alig(int blade) {
	return false;
}//机械臂align放片
bool Wafer_Control::wafercontrol_mov_put2lp(int blade) {
	if (wafercontrol_getsourceslot()) {
		if (blade == 1) return wafer_control_fsm_blade1->Tick(_machine_all_fin);
		if (blade == 2) return wafer_control_fsm_blade2->Tick(_machine_all_fin);
	}
	else {
		if (blade == 1) return wafer_control_fsm_blade1->Tick(_machine_mov2finished);
		if (blade == 2) return wafer_control_fsm_blade2->Tick(_machine_mov2finished);
	}
	return true;
}//机械臂lp放片
bool Wafer_Control::wafercontrol_mov_onestep(int blade) {
	if (wafercontrol_getsourceslot() || wafercontrol_getdestcaslot()) {
		if (blade == 1) return wafer_control_fsm_blade1->Tick(_machine_all_fin);
		if (blade == 2) return wafer_control_fsm_blade2->Tick(_machine_all_fin);
	}
	else {
		if (blade == 1) return wafer_control_fsm_blade1->Tick(0);
		if (blade == 2) return wafer_control_fsm_blade2->Tick(0);
	}
	return true;
}