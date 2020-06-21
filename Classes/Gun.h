#ifndef __Gun_H__
#define __Gun_H__

#include "Player.h"
#include "Weapon.h"
#include "Bullet.h"
#include "Constant.h"
USING_NS_CC;

class Gun :public Weapon {
	CC_SYNTHESIZE(EActorType, _bulletType, BulletType);			//武器使用的子弹类型，近战则没有

public:
	Gun();													//用于初始化武器的属性，之后放到各个武器的类里面去
	~Gun();													//析构函数好像没啥用，先放在这
	static Gun* create(const std::string& filename);
	virtual bool init(const std::string& filename);
	virtual void fire(Scene* _currentScene, const Vec2& pos, Entity* player);

};

#endif
#pragma once