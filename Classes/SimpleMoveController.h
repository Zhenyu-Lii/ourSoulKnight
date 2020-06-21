#ifndef _SIMPLEMOVECONTROLLER_H_
#define _SIMPLEMOVECONTROLLER_H_


#include "cocos2d.h"
#include "Controller.h"
using namespace cocos2d;

class Player;
class safetymap;

class SimpleMoveController :public My_Controller
{
public:
	CREATE_FUNC(SimpleMoveController);
	virtual bool init();
	virtual void update(float dt);

	void set_speed(int ispeed);//�����ƶ��ٶ�
	void set_ixspeed(int ispeed);
	void set_iyspeed(int ispeed);
	void bind_sprite(Sprite* sprite);
	void bind_player(Player* player);
	void bind_scene(safetymap* Scene);



private:
	int m_ixspeed;//x������ƶ��ٶ�
	int m_iyspeed;//y������ƶ��ٶ�
	Sprite* m_sprite;
	Player* m_player;
	safetymap* m_scene;
	void registeKeyBoardEvent();
	ValueMap   map;

	/*action management*/
	Animation* knight_animate;
	Animate* knight_animate_run;

	int left_right_tag;//to decide which action to use when press w or s

	bool key_w;
	bool key_a;
	bool key_s;
	bool key_d;
};

#endif
#pragma once