#include "pch.h"
#include "WaferLogic.h"
//δ��ʼ��״̬*********************************************
bool ONInitpos::OnStateTick(int function) {
	int returnans=0;
	switch (function)
	{
	case 0:
		//returnans = this->machine_all_init();//���е��豸�ĳ�ʼ��
		//if (returnans == false) return false;
		returnans = this->machine_all_move2init();//���е��豸�ĳ�ʼ��
		if (returnans == false) return false;
		fsm->TransStatus(fsm->curState + 1);//ִ���������ת��״̬
		break;
	case _machine_all_init:
		returnans = this->machine_all_init();//���е��豸�ĳ�ʼ��
		if (returnans == false) return false;
		returnans = this->machine_all_move2init();//���е��豸�ĳ�ʼ��
		if (returnans == false) return false;
		fsm->TransStatus(fsm->curState + 1);//ִ���������ת��״̬
		break;
	case _machine_all_fin:
		for (unsigned int i = 0; i < this->thisclp->size(); i++) {
			returnans = this->thisclp->at(i)->back2init();
			if (returnans == 0) return false;
		}
		break;
	default:
		return false;
		break;
	}
	return true;
}//ִ�и�״̬
bool ONInitpos::machine_all_init() {
	bool returnans;//�����˳���ܴ��
	returnans = this->thiscrobot->init(); if (returnans == 0) return false;
	returnans = this->thiscalign->init(); if (returnans == 0) return false;
	for (unsigned int i = 0; i < this->thisclp->size(); i++) {
		returnans = this->thisclp->at(i)->init(); if (returnans == 0) return false;
	}
	return true;
}//��ʼ�����е��豸

//�ں��ӷ�Χ��״̬*********************************************
bool ONcassage::OnStateTick(int function) {
	int returnans = 0;
	switch (function)
	{
	case 0:
		for (unsigned int i = 0; i < this->thisclp->size(); i++) {
			returnans = this->thisclp->at(i)->move2dooropened();
			if (returnans == 0) return false;
		}
		returnans = this->thiscrobot->act_getfromLP(this->fsm->blade);
		if (returnans == 0) return false;
		fsm->TransStatus(fsm->curState + 1);
		break;
	case _machine_mov2dooropened://�ִ￪�Ų�mappingλ��
		for (unsigned int i = 0; i < this->thisclp->size(); i++) {
			returnans = this->thisclp->at(i)->move2dooropened(); 
			if (returnans == 0) return false;
		}
		break;
	case _machine_back2init:
		this->machine_all_move2init();
		break;
	case _machine_mov2mappinged:
		for (int i = 0; i < this->thisclp->size(); i++) {
			returnans = this->thisclp->at(i)->move2dooropened();
			if (returnans == 0) return false;
		}
		break;
	case _machine_mov2getfromlp:
		for (unsigned int i = 0; i < this->thisclp->size(); i++) {
			returnans = this->thisclp->at(i)->move2dooropened();
			if (returnans == 0) return false;
		}
		returnans = this->thiscrobot->act_getfromLP(this->fsm->blade);
		if (returnans == 0) return false;
		fsm->TransStatus(fsm->curState + 1);
		break;
	case _machine_all_fin:
		for (unsigned int i = 0; i < this->thisclp->size(); i++) {
			returnans = this->thisclp->at(i)->back2init();
			if (returnans == 0) return false;
		}
		fsm->TransStatus(0);//�����ȫ���������򷵻�������Ϣ
		break;
	default:
		return false;
		break;
	}
	return true;
}//ִ�и�״̬
bool ONcassage::OnStateEnter() {
	return true;
}//�����˸�״̬
bool ONcassage::OnStateExit() {
	return true;//���Ը���һ�����еľ�Բ����
}

//��Ѱλ��Χ��״̬*********************************************
bool ONaligner::OnStateTick(int function) {
	int returnans = 0;
	switch (function)
	{
	case _machine_all_fin:
		for (unsigned int i = 0; i < this->thisclp->size(); i++) {
			returnans = this->thisclp->at(i)->back2init();
			if (returnans == 0) return false;
		}
		fsm->TransStatus(0);//�����ȫ���������򷵻�������Ϣ
		break;
	case 0:
		returnans = this->thiscrobot->act_put2aligner(this->fsm->blade); if (returnans == 0) return false;
		returnans = this->thiscalign->act_prtaligner(); if (returnans == 0) return false;
		returnans = this->thiscrobot->act_getfromaligner(this->fsm->blade); if (returnans == 0) return false;
		fsm->TransStatus(fsm->curState + 1);
		break;
	case _machine_mov2aligned://�ִ￪�Ų�mappingλ��
		returnans = this->thiscrobot->act_put2aligner(this->fsm->blade); if (returnans == 0) return false;
		returnans = this->thiscalign->act_prtaligner(); if (returnans == 0) return false;
		break;
	case _machine_back2init:
		this->machine_all_move2init();
		break;
	case _machine_mov2getfromal:
		returnans = this->thiscrobot->act_put2aligner(this->fsm->blade); if (returnans == 0) return false;
		returnans = this->thiscalign->act_prtaligner(); if (returnans == 0) return false;
		returnans = this->thiscrobot->act_getfromaligner(this->fsm->blade); if (returnans == 0) return false;
		fsm->TransStatus(fsm->curState + 1);
		break;
	default:
		return false;
		break;
	}
	return true;
}//ִ�и�״̬

//�������Χ��״̬*********************************************
bool ONfinished::OnStateTick(int function) {
	int returnans = 0;
	switch (function) {
	case 0:
		returnans = this->thiscrobot->act_put2LP(this->fsm->blade); if (returnans == 0) return false;
		//returnans = this->thiscrobot->act_init_robot();
		fsm->TransStatus(fsm->curState + 1);
		break;
	case _machine_mov2finished:
		returnans = this->thiscrobot->act_put2LP(this->fsm->blade); if (returnans == 0) return false;
		//returnans = this->thiscrobot->act_init_robot();
		fsm->TransStatus(fsm->curState + 1);
		break;
	case _machine_all_fin:
		this->thiscrobot->act_put2LP(this->fsm->blade);
		for (unsigned int i = 0; i < this->thisclp->size(); i++) {
			returnans = this->thisclp->at(i)->back2init();
			if (returnans == 0) return false;
		}
		fsm->TransStatus(0);//�����ȫ���������򷵻�������Ϣ
		break;
	default:
		return false;
		break;
	}
	return true;
}//ִ�и�״̬