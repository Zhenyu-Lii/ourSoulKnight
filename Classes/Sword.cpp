#include "Sword.h"


Sword::Sword() {
	//基本属性
	_attack = 6;
	_mpConsumption = 0;
	_attackRadius = 35;
	_bulletSpeed = 300;
	_bulletType = PISTOLBULLET;

}

Sword::~Sword() {

}

Sword* Sword::create(const std::string& filename) {
	Sword* sprite = new Sword;
	if (sprite && sprite->init(filename)) {
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

bool Sword::init(const std::string& filename) {
	if (!Sprite::initWithFile(filename)) {
		return false;
	}
	this->setRotation(0.0f);
	this->setScale(0.13);
	this->setAnchorPoint(Vec2(0.1, 0.5));
	this->setTag(_attack);

	/*auto physicsBody = PhysicsBody::createBox(this->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);

	//不攻击的时候，通过碰撞掩码将其设置成无法与怪物碰撞
	physicsBody->setCategoryBitmask(0x00);
	physicsBody->setContactTestBitmask(0x00);
	this->setTag(_attack);
	this->addComponent(physicsBody);*/

	//创建攻击动画
	return true;
}

void Sword::fire(Scene* _currentScene, const Vec2& pos, Entity* player) {
	
	//设置武器攻击方向
	auto direction = pos - this->getParent()->getPosition();
	direction.normalize();
	if (direction.x >= 0 && this->isFlippedX() == false) {
		float temp = (-1)*(180 / PI)* atan(direction.y / direction.x);
		this->setRotation(temp);
	}
	else if (direction.x < 0 && this->isFlippedX() == true) {
		float temp = (180 / PI)* atan(direction.y / direction.x);
		this->setRotation(temp);
	}

	//创建近战武器攻击范围
	auto bullet = Bullet::create(_bulletType, this, direction, _currentScene);
	bullet->setScale(0.4);

	//调整近战武器攻击范围
	bullet->setAnchorPoint(Vec2(0.6, 0.6));
	bullet->setRotation(0.0f);
	bullet->setVisible(false);

	//修正近战武器攻击范围的初始位置
	Vec2 bulletPosition = this->getParent()->getPosition();
	bulletPosition.x += 12;
	bulletPosition.y -= 7.5;
	bullet->setPosition(bulletPosition);

	player->getCurrentMap()->addChild(bullet);
	bullet->new_move();
	
	//角色突进动作
	auto rush = MoveBy::create(0.2f, direction * _attackRadius);
	this->getParent()->runAction(rush);

	//武器动作要修改一下，改成向前突刺
	auto attackMovement = MoveBy::create(0.1f, direction*_attackRadius / 3);
	auto resetMove = MoveBy::create(0.1f, -direction * _attackRadius / 3);
	this->runAction(Sequence::create(attackMovement, resetMove, NULL));

	

	//this->getPhysicsBody()->setCategoryBitmask(0x00);
	//this->getPhysicsBody()->setContactTestBitmask(0x00);
	
}
