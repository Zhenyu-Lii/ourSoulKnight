#ifndef __ADVENTUREMAP_SCENE_H__
#define __ADVENTUREMAP_SCENE_H__

#include "cocos2d.h"
#include  "allHeadFiles.h"
using namespace cocos2d;

class  adventuremap : public Scene
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();
	CREATE_FUNC(adventuremap);
	void scheduleBlood(float delta);
	virtual void update(float dt);
	RemoteSoldierManager* m_remoteSoldierManager;

private:
	TMXTiledMap *_tiledmap;									//地图类中相关地图
	TMXLayer *_background;									//背景层
	Sprite *_player;
	Player* m_player;
	RemoteSoldier* m_monster;								//测试武器用的monster
	ProgressTimer* bloodProgress;
	ProgressTimer* MPProgress;
	ProgressTimer* ACProgress;
	Knight* mplayer;

	std::string bloodNum;
	char bloodChar[3];
	Label* bloodLabel;

	std::string  ACNum;
	char ACChar[3];
	Label* ACLabel;

	std::string MPNum;
	char MPChar[7];
	Label* MPLabel;
protected:
	virtual bool onContactBegin(PhysicsContact& contact);	//碰撞后的回调
	virtual bool onTouchBegin(Touch* touch, Event* event);	//点击鼠标后的回调
	void menuCloseCallback(Ref* pSender);
	void start_menuCloseCallback(Ref* pSender);
	void home_menuCloseCallback(Ref* pSender);
	void music_menuCloseCallback(Ref* pSender);
	bool musicOnOff;
	Sprite* suspend_scene;
	MenuItemImage* suspend_start;
	MenuItemImage* home_button;
	MenuItemImage* music_button;
};


#endif // __ADVENTUREMAP_SCENE_H__
#pragma once

/*
bool safetymap::onContactBegin(PhysicsContact& contact) {

	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();

	if (nodeA && nodeB)
	{
		if (nodeA->getTag() == 10)
		{
			nodeB->removeFromParentAndCleanup(true);
		}
		else if (nodeB->getTag() == 10)
		{
			nodeA->removeFromParentAndCleanup(true);
		}
	}

	return true;
}
*/

/*
创建contactListener
auto contactListener = EventListenerPhysicsContact::create();
contactListener->onContactBegin = CC_CALLBACK_1(safetymap::onContactBegin, this);
_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
*/

/*在场景中绑定player和monster,并设置碰撞掩码
this->m_player = mplayer;
this->m_monster = monster;
m_player->getPhysicsBody()->setCategoryBitmask(0x01);
m_player->getPhysicsBody()->setContactTestBitmask(0x04);
m_monster->getPhysicsBody()->setCategoryBitmask(0x03);
m_monster->getPhysicsBody()->setContactTestBitmask(0x04);*/