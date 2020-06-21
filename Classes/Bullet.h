#ifndef __BULLET_H__
#define __BULLET_H__

#include "cocos2d.h"
#include "RemoteSoldier.h"
#include "Constant.h"
#include "Weapon.h"
#include <string.h>
USING_NS_CC;

class Bullet : public cocos2d::Sprite
{
	CC_SYNTHESIZE(Entity*, _attackSource, attackSource);
	CC_SYNTHESIZE(Weapon*, _attackWeapon, attackWeapon);
	CC_SYNTHESIZE(Vec2, _attackDirection,attackDirection);
	CC_SYNTHESIZE(int, _damage, damage);
	CC_SYNTHESIZE(std::string, _bulletType, bulletType);
	CC_SYNTHESIZE(Scene*, _currentScene, currentScene);
public:
	static Bullet* create(EActorType actorType,Entity* attackSource, Vec2 attackDirection, Scene* currentScene);
	bool init(EActorType actorType, Entity* attackSource, Vec2 attackDirection, Scene* currentScene);
	static Bullet* create(EActorType actorType, Weapon* attackWeapon, Vec2 attackDirection, Scene* currentScene);
	bool init(EActorType actorType, Weapon* attackWeapon, Vec2 attackDirection, Scene* currentScene);
	void move();
	void new_move();

};

#endif 
#pragma once
