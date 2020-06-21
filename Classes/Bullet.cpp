#include "Bullet.h"

//用于怪物攻击时创建子弹
Bullet* Bullet::create(EActorType actorType, Entity* attackSource, Vec2 attackDirection, Scene* currentScene)
{
	Bullet* bullet = new Bullet;
	if (bullet && bullet->init(actorType, attackSource,attackDirection, currentScene))
	{
		bullet->autorelease();
		return bullet;
	}
	CC_SAFE_DELETE(bullet);
	return nullptr;
}

bool Bullet::init(EActorType actorType, Entity* attackSource, Vec2 attackDirection, Scene* currentScene)
{
	if (actorType == SHORTREMOTE)
	{
		_bulletType = "ShortRemote";
		setTexture("ShortRemoteBullet.png"); 
	}
	else if (actorType == LONGREMOTE)
	{
		_bulletType = "LongRemote";
		setTexture("LongRemoteBullet.png"); 
	}
	else
		return false;
	_attackSource = attackSource;
	_damage = attackSource->getattack();
	_attackDirection = attackDirection;
	_currentScene = currentScene;

	//设置子弹的碰撞模型
	auto physicsBody = PhysicsBody::createBox(this->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setCategoryBitmask(0x04);
	physicsBody->setContactTestBitmask(0x01);
	this->setTag(_damage);
	this->addComponent(physicsBody);

	return true;
}

void Bullet::move()
{
	Vec2 shoutAmount = _attackDirection * _attackSource->getattackRadius();
	auto moveby = MoveBy::create(_attackSource->getattackRadius() / _attackSource->getbulletSpeed(), shoutAmount);
	auto actionRemove = RemoveSelf::create();
	this->runAction(Sequence::create(moveby, actionRemove, nullptr));
}


//用于玩家攻击时创建子弹
Bullet* Bullet::create(EActorType actorType, Weapon* attackSource, Vec2 attackDirection, Scene* currentScene)
{
	Bullet* bullet = new Bullet;
	if (bullet && bullet->init(actorType, attackSource, attackDirection, currentScene))
	{
		bullet->autorelease();
		return bullet;
	}
	CC_SAFE_DELETE(bullet);
	return nullptr;
}

bool Bullet::init(EActorType actorType, Weapon* attackWeapon, Vec2 attackDirection, Scene* currentScene)
{
	if (actorType == SHORTREMOTE)
	{
		_bulletType = "ShortRemote";
		setTexture("ShortRemoteBullet.png");
	}
	else if (actorType == LONGREMOTE)
	{
		_bulletType = "LongRemote";
		setTexture("LongRemoteBullet.png");
	}
	else if (actorType == PISTOLBULLET)
	{
		_bulletType = "PISTOLBULLET";
		setTexture("PistolBullet.png");
	}
	else
		return false;
	_attackWeapon = attackWeapon;
	_damage = attackWeapon->getAttack();
	_attackDirection = attackDirection;
	_currentScene = currentScene;

	//设置子弹的碰撞模型
	auto physicsBody = PhysicsBody::createBox(this->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setCategoryBitmask(0x04);
	physicsBody->setContactTestBitmask(0x02);
	this->setTag(_damage);
	this->addComponent(physicsBody);

	return true;
}
void Bullet::new_move()
{
	Vec2 shoutAmount = _attackDirection * _attackWeapon->getattackRadius();
	auto moveby = MoveBy::create(_attackWeapon->getattackRadius() / _attackWeapon->getbulletSpeed(), shoutAmount);
	auto actionRemove = RemoveSelf::create();
	this->runAction(Sequence::create(moveby, actionRemove, nullptr));
	/*
	Vec2 destination = _attackDirection * 2000;
	auto moveby = MoveBy::create(2.0f, destination);
	auto actionRemove = RemoveSelf::create();
	this->runAction(Sequence::create(moveby, actionRemove, nullptr));
	*/
}
