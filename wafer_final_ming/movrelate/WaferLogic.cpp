#include "pch.h"
#include "WaferLogic.h"
//未初始的状态*********************************************
bool ONInitpos::OnStateTick(int function) {
	int returnans=0;
	switch (function)
	{
	case 0:
		//returnans = this->machine_all_init();//所有的设备的初始化
		//if (returnans == false) return false;
		returnans = this->machine_all_move2init();//所有的设备的初始化
		if (returnans == false) return false;
		fsm->TransStatus(fsm->curState + 1);//执行完这个才转移状态
		break;
	case _machine_all_init:
		returnans = this->machine_all_init();//所有的设备的初始化
		if (returnans == false) return false;
		returnans = this->machine_all_move2init();//所有的设备的初始化
		if (returnans == false) return false;
		fsm->TransStatus(fsm->curState + 1);//执行完这个才转移状态
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
}//执行该状态
bool ONInitpos::machine_all_init() {
	bool returnans;//下面的顺序不能错的
	returnans = this->thiscrobot->init(); if (returnans == 0) return false;
	returnans = this->thiscalign->init(); if (returnans == 0) return false;
	for (unsigned int i = 0; i < this->thisclp->size(); i++) {
		returnans = this->thisclp->at(i)->init(); if (returnans == 0) return false;
	}
	return true;
}//初始化所有的设备

//在盒子范围的状态*********************************************
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
	case _machine_mov2dooropened://抵达开门并mapping位置
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
		fsm->TransStatus(0);//如果是全部结束，则返回最后的信息
		break;
	default:
		return false;
		break;
	}
	return true;
}//执行该状态
bool ONcassage::OnStateEnter() {
	return true;
}//进入了该状态
bool ONcassage::OnStateExit() {
	return true;//可以更新一下所有的晶圆方面
}

//在寻位范围的状态*********************************************
bool ONaligner::OnStateTick(int function) {
	int returnans = 0;
	switch (function)
	{
	case _machine_all_fin:
		for (unsigned int i = 0; i < this->thisclp->size(); i++) {
			returnans = this->thisclp->at(i)->back2init();
			if (returnans == 0) return false;
		}
		fsm->TransStatus(0);//如果是全部结束，则返回最后的信息
		break;
	case 0:
		returnans = this->thiscrobot->act_put2aligner(this->fsm->blade); if (returnans == 0) return false;
		returnans = this->thiscalign->act_prtaligner(); if (returnans == 0) return false;
		returnans = this->thiscrobot->act_getfromaligner(this->fsm->blade); if (returnans == 0) return false;
		fsm->TransStatus(fsm->curState + 1);
		break;
	case _machine_mov2aligned://抵达开门并mapping位置
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
}//执行该状态

//快结束范围的状态*********************************************
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
		fsm->TransStatus(0);//如果是全部结束，则返回最后的信息
		break;
	default:
		return false;
		break;
	}
	return true;
}//执行该状态