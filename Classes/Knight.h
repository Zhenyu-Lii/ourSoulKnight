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
};

#endif
#pragma once
