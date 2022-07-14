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
typedef int estate;//��ʾ״̬�ı�ʾ����
class WaferStatus : public CCmdTarget //��ſ���״̬���
{
public:
	virtual bool OnStateEnter() = 0;//�����˸�״̬
	virtual bool OnStateTick(int) = 0;//ִ�и�״̬
	virtual bool OnStateExit() = 0;//�˳���״̬
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
	CString now;//չ�ֳ���״̬ת�ƿ��ӻ�
};
/*  ״̬�߼� : ��Ҫ���ֲ�ͬ��״̬ת��������  */
class ONInitpos :public WaferStatus {
public:
	ONInitpos(FSMworkflow* fsm, machinecrobot* _thiscrobot,
		machinecrobot* _thiscalign, vector<machinecloadport*>* _thisclp) 
		:WaferStatus(fsm, _thiscrobot, _thiscalign, _thisclp) {}
	virtual bool OnStateEnter() { return true; };//�����˸�״̬
	virtual bool OnStateTick(int);//ִ�и�״̬
	virtual bool OnStateExit() {
		now.Format(_T("�����豸�ĳ�ʼ����ɣ�")); return true;
	}//�˳���״̬
	bool machine_all_init();//��ʼ�����е��豸
	bool machine_all_move2init() {
		bool returnans;//�����˳���ܴ��
		returnans = this->thiscrobot->act_init_robot(); if (returnans == 0) return false;
		returnans = this->thiscalign->act_init_robot(); if (returnans == 0) return false;
		for (unsigned int i = 0; i < this->thisclp->size(); i++) {
			returnans = this->thisclp->at(i)->move2init(); if (returnans == 0) return false;
		}
		return true;
	}
private:
	int checkstage = 0;

};//δ��ʼ��״̬��������ʼ����һЩ����,�շ��º���
class ONcassage :public WaferStatus {
public:
	ONcassage(FSMworkflow* fsm, machinecrobot* _thiscrobot,
		machinecrobot* _thiscalign, vector<machinecloadport*>* _thisclp)
		:WaferStatus(fsm, _thiscrobot, _thiscalign, _thisclp) {}
	virtual bool OnStateEnter();//�����˸�״̬
	virtual bool OnStateTick(int);//ִ�и�״̬
	virtual bool OnStateExit();//�˳���״̬
	bool machine_all_move2init() {
		bool returnans;//�����˳���ܴ��
		returnans = this->thiscrobot->act_init_robot(); if (returnans == 0) return false;
		returnans = this->thiscalign->act_init_robot(); if (returnans == 0) return false;
		for (unsigned int i = 0; i < this->thisclp->size(); i++) {
			returnans = this->thisclp->at(i)->move2init(); if (returnans == 0) return false;
		}
		return true;
	}
};//�ѳ�ʼ����LPδ������״̬,��Ҫmapping������ȡƬ����ת��״̬
class ONaligner :public WaferStatus {
public:
	ONaligner(FSMworkflow* fsm, machinecrobot* _thiscrobot,
		machinecrobot* _thiscalign, vector<machinecloadport*>* _thisclp)
		:WaferStatus(fsm, _thiscrobot, _thiscalign, _thisclp) {}
	virtual bool OnStateEnter() { return true; };//�����˸�״̬
	virtual bool OnStateTick(int);//ִ�и�״̬
	virtual bool OnStateExit() { return true; }//�˳���״̬
	bool machine_all_move2init() {
		bool returnans;//�����˳���ܴ��
		returnans = this->thiscrobot->act_init_robot(); if (returnans == 0) return false;
		returnans = this->thiscalign->act_init_robot(); if (returnans == 0) return false;
		for (unsigned int i = 0; i < this->thisclp->size(); i++) {
			returnans = this->thisclp->at(i)->move2init(); if (returnans == 0) return false;
		}
		return true;
	}
};//����ɿ����Լ�MAPPING������LP��������δѰλ״̬����Ҫ��ƬȻ��Ѱλ
class ONtext :public WaferStatus {
public:
	ONtext(FSMworkflow* fsm, machinecrobot* _thiscrobot,
		machinecrobot* _thiscalign, vector<machinecloadport*>* _thisclp)
		:WaferStatus(fsm, _thiscrobot, _thiscalign, _thisclp) {}
	virtual bool OnStateEnter() { return false; };//�����˸�״̬
	virtual bool OnStateTick(int) { return false; };//ִ�и�״̬
	virtual bool OnStateExit() { return false; };//�˳���״̬
};//��Ѱλ��δ��ɼ��
class ONfinished :public WaferStatus {
public:
	ONfinished(FSMworkflow* fsm, machinecrobot* _thiscrobot,
		machinecrobot* _thiscalign, vector<machinecloadport*>* _thisclp)
		:WaferStatus(fsm, _thiscrobot, _thiscalign, _thisclp) {}
	virtual bool OnStateEnter() { return false; };//�����˸�״̬
	virtual bool OnStateTick(int) ;//ִ�и�״̬
	virtual bool OnStateExit() { return false; };//�˳���״̬
	bool machine_all_move2init() {
		bool returnans;//�����˳���ܴ��
		returnans = this->thiscrobot->act_init_robot(); if (returnans == 0) return false;
		returnans = this->thiscalign->act_init_robot(); if (returnans == 0) return false;
		for (unsigned int i = 0; i < this->thisclp->size(); i++) {
			returnans = this->thisclp->at(i)->move2init(); if (returnans == 0) return false;
		}
		return true;
	}
};//��������еĲ��裬�ûص��������棬��ɸ�״̬����ȫ������
//��״̬���ٴ�ת����Ҫ�鿴�Ƿ���Ҫѭ������


class FSMworkflow :
	public CCmdTarget
{
public:
	unordered_map<usernode, WaferStatus*> all_states;//״̬��������ÿһ��״̬��Ӧһ����š�����ִ�����ȼ������0->1->2->3->4��ֻ������״̬�ſ��Ե�0
													//��ͬ�����̿��ƣ����ö���״̬ת�������жϣ�
	estate curState = -1;//δ��ʼ��״̬
	estate statecount = 0;//״̬������
	int blade = 1;
public:
	FSMworkflow(int _blade) :blade(_blade) {}
	bool Register(estate e, WaferStatus* state) {
		all_states[e] = state;
		statecount++;
		return true;
	};//ע����״̬
	bool TransStatus(estate newstate) {
		if (newstate >= statecount) newstate = 1;//��֤ѭ����0������ǳ�ʼ�����ܽ���ѭ��
		all_states[curState]->OnStateExit();//��ǰ�˳�״̬����
		all_states[newstate]->OnStateEnter();//����״̬��Ҫִ�еĺ���
		curState = newstate;
		return true;
	};//ת��״̬
	void SetInitState(estate startstate)//��ʼ״̬
	{
		curState = startstate;
	}
	bool Tick(int excfunction) {//��Ҫ�������������ÿ��״̬����ľ���ִ�в���������󣬸���ÿ�������Ķ�����ж���
		return all_states[curState]->OnStateTick(excfunction);
	};//ִ��״̬
	void Clear() {
		all_states.clear();
	}
};
