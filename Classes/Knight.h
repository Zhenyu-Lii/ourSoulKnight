#ifndef _Knight_H_
#define _Knight_H_

#include "Player.h"
USING_NS_CC;

class Knight :public Player {
public:
	Knight();
	~Knight();
	CREATE_FUNC(Knight);
	virtual bool init();
	virtual void skill();//¼¼ÄÜ
	int get_HP(){ return _HP; }
	int  get_AC() { return _AC; }
	int get_MP() { return _MP; }
};

#endif
#pragma once
