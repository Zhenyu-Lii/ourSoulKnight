#include "Gun.h"
#include <math.h>

Gun::Gun() {
	//基本属性
	_attack = 2;
	_mpConsumption = 0;
	_attackRadius = 500;
	_bulletSpeed = 280;
	_bulletType = PISTOLBULLET;
}

Gun::~Gun() {

}

Gun* Gun::create(const std::string& filename)
{
	Gun* sprite = new Gun;
	if (sprite && sprite->init(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

bool Gun::init(const std::string& filename)
{
	if (!Sprite::initWithFile(filename)) {
		return false;
	}
	//修正图片缩放
	this->setScale(0.08);
	//武器位置
	this->setAnchorPoint(Vec2(0.1, 0.1));
	return true;
}

void Gun::fire(Scene* _currentScene, const Vec2& pos, Entity* player) {

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
	
	//创建子弹
	auto bullet = Bullet::create(_bulletType, this, direction, _currentScene);
	bullet->setScale(0.4);

	//修正子弹初始位置
	Vec2 bulletPosition = this->getParent()->getPosition();
	bulletPosition.x += 10;
	bulletPosition.y -= 7.5;
	bullet->setPosition(bulletPosition);
	
	player->getCurrentMap()->addChild(bullet);
	bullet->new_move();
}