#pragma once
#include <afxwin.h>
#include <afx.h>
#include "machinecloadport.h"
#include "machinecrobot.h"
#include <unordered_map>
#include "wafer_json.h"
#define _machine_all_init 1
#define _machine_mov2dooropened 2
#define _machine_mov2aligned 3
#define _machine_mov2finished 4
#define _machine_mov2ended 5
#define _machine_getfromlp 6
#define _machine_getfromalign 7
#define _machine_put2lp 8
#define _machine_put2align 9
#define _machine_mapping 10
#define _machine_back2init 11
#define _machine_mov2mappinged 12
#define _machine_mov2getfromlp 13
#define _machine_mov2getfromal 14
#define _machine_all_fin 15
class FSMworkflow;
typedef int usernode;
typedef int estate;//表示状态的表示类型
class WaferStatus : public CCmdTarget //存放控制状态相关
{
public:
	virtual bool OnStateEnter() = 0;//进入了该状态
	virtual bool OnStateTick(int) = 0;//执行该状态
	virtual bool OnStateExit() = 0;//退出该状态
	WaferStatus(FSMworkflow* _fsm, machinecrobot* _thiscrobot,
		machinecrobot* _thiscalign,vector<machinecloadport*>* _thisclp){ 
		fsm = _fsm; 
		thiscrobot = _thiscrobot;
		thiscalign = _thiscalign;
		thisclp = _thisclp;
	}
	virtual ~WaferStatus() {}
protected:
	FSMworkflow* fsm = nullptr;
	machinecrobot* thiscrobot = nullptr;
	machinecrobot* thiscalign = nullptr;
	vector<machinecloadport*>* thisclp = nullptr;
	CString now;//展现出来状态转移可视化
};
/*  状态逻辑 : 需要体现不同的状态转换的内容  */
class ONInitpos :public WaferStatus {
public:
	ONInitpos(FSMworkflow* fsm, machinecrobot* _thiscrobot,
		machinecrobot* _thiscalign, vector<machinecloadport*>* _thisclp) 
		:WaferStatus(fsm, _thiscrobot, _thiscalign, _thisclp) {}
	virtual bool OnStateEnter() { return true; };//进入了该状态
	virtual bool OnStateTick(int);//执行该状态
	virtual bool OnStateExit() {
		now.Format(_T("所有设备的初始化完成！")); return true;
	}//退出该状态
	bool machine_all_init();//初始化所有的设备
	bool machine_all_move2init() {
		bool returnans;//下面的顺序不能错的
		returnans = this->thiscrobot->act_init_robot(); if (returnans == 0) return false;
		returnans = this->thiscalign->act_init_robot(); if (returnans == 0) return false;
		for (unsigned int i = 0; i < this->thisclp->size(); i++) {
			returnans = this->thisclp->at(i)->move2init(); if (returnans == 0) return false;
		}
		return true;
	}
private:
	int checkstage = 0;

};//未初始化状态，包含初始化的一些操作,刚放下盒子
class ONcassage :public WaferStatus {
public:
	ONcassage(FSMworkflow* fsm, machinecrobot* _thiscrobot,
		machinecrobot* _thiscalign, vector<machinecloadport*>* _thisclp)
		:WaferStatus(fsm, _thiscrobot, _thiscalign, _thisclp) {}
	virtual bool OnStateEnter();//进入了该状态
	virtual bool OnStateTick(int);//执行该状态
	virtual bool OnStateExit();//退出该状态
	bool machine_all_move2init() {
		bool returnans;//下面的顺序不能错的
		returnans = this->thiscrobot->act_init_robot(); if (returnans == 0) return false;
		returnans = this->thiscalign->act_init_robot(); if (returnans == 0) return false;
		for (unsigned int i = 0; i < this->thisclp->size(); i++) {
			returnans = this->thisclp->at(i)->move2init(); if (returnans == 0) return false;
		}
		return true;
	}
};//已初始化，LP未就绪的状态,需要mapping结束并取片才能转换状态
class ONaligner :public WaferStatus {
public:
	ONaligner(FSMworkflow* fsm, machinecrobot* _thiscrobot,
		machinecrobot* _thiscalign, vector<machinecloadport*>* _thisclp)
		:WaferStatus(fsm, _thiscrobot, _thiscalign, _thisclp) {}
	virtual bool OnStateEnter() { return true; };//进入了该状态
	virtual bool OnStateTick(int);//执行该状态
	virtual bool OnStateExit() { return true; }//退出该状态
	bool machine_all_move2init() {
		bool returnans;//下面的顺序不能错的
		returnans = this->thiscrobot->act_init_robot(); if (returnans == 0) return false;
		returnans = this->thiscalign->act_init_robot(); if (returnans == 0) return false;
		for (unsigned int i = 0; i < this->thisclp->size(); i++) {
			returnans = this->thisclp->at(i)->move2init(); if (returnans == 0) return false;
		}
		return true;
	}
};//已完成开门以及MAPPING操作（LP就绪），未寻位状态：需要放片然后寻位
class ONtext :public WaferStatus {
public:
	ONtext(FSMworkflow* fsm, machinecrobot* _thiscrobot,
		machinecrobot* _thiscalign, vector<machinecloadport*>* _thisclp)
		:WaferStatus(fsm, _thiscrobot, _thiscalign, _thisclp) {}
	virtual bool OnStateEnter() { return false; };//进入了该状态
	virtual bool OnStateTick(int) { return false; };//执行该状态
	virtual bool OnStateExit() { return false; };//退出该状态
};//已寻位，未完成检测
class ONfinished :public WaferStatus {
public:
	ONfinished(FSMworkflow* fsm, machinecrobot* _thiscrobot,
		machinecrobot* _thiscalign, vector<machinecloadport*>* _thisclp)
		:WaferStatus(fsm, _thiscrobot, _thiscalign, _thisclp) {}
	virtual bool OnStateEnter() { return false; };//进入了该状态
	virtual bool OnStateTick(int) ;//执行该状态
	virtual bool OnStateExit() { return false; };//退出该状态
	bool machine_all_move2init() {
		bool returnans;//下面的顺序不能错的
		returnans = this->thiscrobot->act_init_robot(); if (returnans == 0) return false;
		returnans = this->thiscalign->act_init_robot(); if (returnans == 0) return false;
		for (unsigned int i = 0; i < this->thisclp->size(); i++) {
			returnans = this->thisclp->at(i)->move2init(); if (returnans == 0) return false;
		}
		return true;
	}
};//已完成所有的步骤，该回到盒子里面，完成该状态下则全部结束
//该状态的再次转移需要查看是否需要循环机制


class FSMworkflow :
	public CCmdTarget
{
public:
	unordered_map<usernode, WaferStatus*> all_states;//状态管理器，每一个状态对应一个序号。表明执行优先级，序号0->1->2->3->4，只有最后的状态才可以到0
													//不同的流程控制：设置多种状态转移条件判断；
	estate curState = -1;//未开始的状态
	estate statecount = 0;//状态的数量
	int blade = 1;
public:
	FSMworkflow(int _blade) :blade(_blade) {}
	bool Register(estate e, WaferStatus* state) {
		all_states[e] = state;
		statecount++;
		return true;
	};//注册新状态
	bool TransStatus(estate newstate) {
		if (newstate >= statecount) newstate = 1;//保证循环，0代表的是初始化不能进入循环
		all_states[curState]->OnStateExit();//当前退出状态函数
		all_states[newstate]->OnStateEnter();//进入状态需要执行的函数
		curState = newstate;
		return true;
	};//转移状态
	void SetInitState(estate startstate)//初始状态
	{
		curState = startstate;
	}
	bool Tick(int excfunction) {//需要定义参数，决定每个状态里面的具体执行参数，传入后，根据每个函数的定义进行定制
		return all_states[curState]->OnStateTick(excfunction);
	};//执行状态
	void Clear() {
		all_states.clear();
	}
};
