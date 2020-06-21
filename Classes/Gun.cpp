#include "Gun.h"
#include <math.h>

Gun::Gun() {
	//��������
	_attack = 2;
	_mpConsumption = 0;
	_attackRadius = 500;
	_bulletSpeed = 280;
	_bulletType = PISTOLBULLET;
	
	//����λ��
	this->setAnchorPoint(Vec2(0.1, 0.1));
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
	this->setScale(0.08);
	return true;
}

void Gun::fire(Scene* _currentScene, const Vec2& pos, Entity* player) {

	//��������
	auto direction = pos - this->getParent()->getPosition();
	direction.normalize();

	//�����ӵ�
	auto bullet = Bullet::create(_bulletType, this, direction, _currentScene);
	bullet->setScale(0.4);

	//�����ӵ���ʼλ��
	Vec2 bulletPosition = this->getParent()->getPosition();
	bulletPosition.x += 10;
	bulletPosition.y -= 7.5;
	bullet->setPosition(bulletPosition);
	
	player->getCurrentMap()->addChild(bullet);
	bullet->new_move();
}