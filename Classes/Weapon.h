#ifndef _Weapon_H_
#define _Weapon_H_

#include "cocos2d.h"
#include "Entity.h"
USING_NS_CC;

class Weapon :public Sprite {
	CC_SYNTHESIZE(int, _attack, Attack);						//攻击力
	CC_SYNTHESIZE(float, _attackRadius, attackRadius);			//攻击半径
	CC_SYNTHESIZE(int, _mpConsumption, MpConsume);				//单次攻击能量消耗
	CC_SYNTHESIZE(int, _bulletSpeed, bulletSpeed);

public:
	void hide();												//隐藏当前不使用的武器
	void show();												//切换武器时使用
	void trackTouch();											//使武器指向和鼠标指向一致
	virtual void fire(Scene* _currentScene, const Vec2& pos, Entity* player);	//开火函数
};

#endif // !__Weapon_H__

