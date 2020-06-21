#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "cocos2d.h"
#include "Entity.h"
#include "Weapon.h"
#include "RemoteSoldier.h"
#include "Bullet.h"
USING_NS_CC;


class Player :public Entity {
	CC_SYNTHESIZE(int, _MP, MP);		
	CC_SYNTHESIZE(int, _AC, AC);
	CC_SYNTHESIZE(Vec2, _weaponPosition, WpPos);				//武器固定在人物上的相对位�?默认值为player的中�?
	CC_SYNTHESIZE(Weapon*, _currentWeapon, CurrentWeapon);
	CC_SYNTHESIZE(Vector<Weapon*> , _weaponBag, WeaponBag);
	CC_SYNTHESIZE(RemoteSoldier*, _lockedTarget, LockedTarget);
	CC_SYNTHESIZE(bool, _usingSkill, UsingSkill);


public:
	Player();
	~Player();
	CREATE_FUNC(Player);
	virtual bool init();

	//功能模块
	bool bindSprite(Sprite* sprite);							//绑定精灵对象
	bool bindWeapon(Weapon* Weapon);							//�󶨺���������
	bool bindInitWeapon(Weapon* Weapon);						//�󶨳�ʼ����

	virtual void die();											//角色死亡，删除角色并返回安全地图
	virtual void takeDamage(int damage);						//受击判定，并掉血
	void attack(Scene* currentScene, const Vec2& pos);			//攻击函数
	void rotateWeapon(const Vec2& pos);							//武器跟随攻击方向
	void resetWeaponPos();
	virtual void switchWeapon();								//接口，切换武器，角色不同切换武器的效果不�?
	void pickWeapon();											//接口，拾取武�?
	virtual void skill();										//技�?

	void setViewPointByPlayer();
	virtual void set_tag_position(int x, int y);
	void setTiledMap(TMXTiledMap* map);
	void bind_scene(Scene* scene);
	void activateSkill();
private:
	Weapon* m_weapon;											//Player当前使用的武�?
	TMXLayer* meta;												//检测碰撞的地图�?
	Point tileCoordForPosition(Point pos);						//将像素坐标转化为地图格子坐标
};

#endif
#pragma once