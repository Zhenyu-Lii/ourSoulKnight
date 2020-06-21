#ifndef _myScene_H_
#define _myScene_H_

#include "cocos2d.h"
USING_NS_CC;

class RemoteSoldierManager;
class myScene: public Scene
{
public:
	RemoteSoldierManager* m_remoteSoldierManager;
	/*static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(myScene);
	*/
};


#endif // !_myScene_H_
#pragma once
