#ifndef _KnightSTARTMAP_SCENE_H_
#define _KnightSTARTMAP_SCENE_H_
#include"Player.h"
#include "cocos2d.h"
using namespace cocos2d;

class  KnightStartMap : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	void menuCloseCallback(Ref* pSender);
	CREATE_FUNC(KnightStartMap);
	void start_menuCloseCallback(Ref* pSender);
	void home_menuCloseCallback(Ref* pSender);
	void music_menuCloseCallback(Ref* pSender);
	void update(float dt);
private:
	cocos2d::TMXTiledMap *_tiledmap;//地图类中相关地图
	cocos2d::TMXLayer *_background;//背景层
	Player* m_player;
	MenuItemImage* music_button;
	bool musicOff;
	Sprite* suspend_scene;
	MenuItemImage* suspend_start;
	MenuItemImage* home_button;

};


#endif //KnightstartmapScene.h !
