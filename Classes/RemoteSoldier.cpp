#include "RemoteSoldier.h"

RemoteSoldier* RemoteSoldier::create(EActorType soldierType,Scene* currentScene)
{
	RemoteSoldier* sprite = new RemoteSoldier;
	if (sprite && sprite->init(soldierType, currentScene))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}
bool RemoteSoldier::init(EActorType soldierType, Scene* currentScene)
{
	if (soldierType == SHORTREMOTE)
	{
		_soldierType = "ShortRemote";
		_attack = SHORT_REMOTE_SOLDIER_ATTACK;
		_attackRadius = SHORT_REMOTE_SOLDIER_ATTACK_RADIUS;
	}
	else if (soldierType == LONGREMOTE)
	{
		_soldierType = "LongRemote";
		_attack = LONG_REMOTE_SOLDIER_ATTACK;
		_attackRadius = LONG_REMOTE_SOLDIER_ATTACK_RADIUS;
	}
	else
		return false;
	_attackInterval = SOLDIER_ATTACK_INTERVAL;
	_alreadyDead = false;
	_HP = REMOTE_SOLDIER_HP;
	_moveSpeed = REMOTE_SOLDIER_MOVE_SPEED;
	_attackTarget = nullptr;
	_currentScene = currentScene;
	_bulletSpeed = SOLDIER_BULLET_SPEED;
	setTag(-2);

	return true;
}

bool RemoteSoldier::bindSprite(Sprite*sprite) {
	this->m_sprite = sprite;
	if (m_sprite == nullptr)
	{
		printf("m_sprite in this entity is nullptr, check wether the file used to create the sprite in right dictionary.");
		return false;
	}
	else
	{
		this->addChild(m_sprite);
		/*设置Entity的大小和m_sprite的大小一致，否则碰撞模型会不对*/
		Size size = m_sprite->getContentSize();
		m_sprite->setPosition(Point(size.width*0.5f, size.height*0.5f));
		this->setContentSize(size);
		this->setAnchorPoint(Vec2(0.5, 0.5));

		auto physicsBody = PhysicsBody::createBox(size, PhysicsMaterial(0.0f, 0.0f, 0.0f));
		physicsBody->setDynamic(false);
		physicsBody->setCategoryBitmask(0x02);
		physicsBody->setContactTestBitmask(0x04);
		this->addComponent(physicsBody);

		return true;
	}
}

void RemoteSoldier::attack(Entity* attackTarget)
{
	_attackTarget = attackTarget;
	Vec2 attackPosition = _attackTarget->getPosition();
	Vec2 Distance = attackPosition - this->getPosition();
	Vec2 direction = Distance.getNormalized();
	if (_soldierType == "ShortRemote") 
	{
		auto bullet1 = Bullet::create(SHORTREMOTE, this, direction, _currentScene);
		direction.rotate(Vec2(0, 0), 0.5);
		auto bullet2 = Bullet::create(SHORTREMOTE, this, direction, _currentScene);
		direction.rotate(Vec2(0, 0), -1.0);
		auto bullet3 = Bullet::create(SHORTREMOTE, this, direction, _currentScene);
		bullet1->setScale(1.5);
		bullet2->setScale(1.5);
		bullet3->setScale(1.5);
		bullet1->setPosition(this->getPosition());
		bullet2->setPosition(this->getPosition());
		bullet3->setPosition(this->getPosition());
		attackTarget->getCurrentMap()->addChild(bullet1);
		attackTarget->getCurrentMap()->addChild(bullet2);
		attackTarget->getCurrentMap()->addChild(bullet3);
		bullet1->move();
		bullet2->move();
		bullet3->move();
	}
	else if (_soldierType == "LongRemote")
	{
		auto bullet = Bullet::create(LONGREMOTE, this, direction, _currentScene);
		bullet->setPosition(this->getPosition());
		attackTarget->getCurrentMap()->addChild(bullet);
		bullet->move();
	}
}
void RemoteSoldier::takeDamage(int damage)
{
	_HP -= damage;
	if (_HP <= 0)
	{
		_alreadyDead = true;
		this->die();
	}
}

void RemoteSoldier::die()
{
	srand((unsigned)time(nullptr));
	int random = rand() % 100;
	if(random > 90 && random % 2 == 0)
	{
		Items* item = Items::create(REDBOTTLE);
		item->setPosition(this->getPosition());
		_currentScene->addChild(item);
	}
	else if(random > 90 && random % 2 != 0)
	{
		Items* item = Items::create(BLUEBOTTLE);
		item->setPosition(this->getPosition());
		_currentScene->addChild(item);
	}
	this->setVisible(false);
	this->getPhysicsBody()->setCategoryBitmask(0x00);
	this->getPhysicsBody()->setContactTestBitmask(0x00);
}