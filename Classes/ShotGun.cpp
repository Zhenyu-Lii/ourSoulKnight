#include "ShotGun.h"

ShotGun::ShotGun() {
	//��������
	_attack = 2;
	_mpConsumption = 2;
	_attackRadius = 350;
	_bulletSpeed = 180;
	_bulletType = SHORTREMOTE;
}
ShotGun::~ShotGun() {

}

ShotGun* ShotGun::create(const std::string& filename)
{
	ShotGun* sprite = new ShotGun;
	if (sprite && sprite->init(filename))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

bool ShotGun::init(const std::string& filename)
{
	if (!Sprite::initWithFile(filename)) {
		return false;
	}
	//����ͼƬ����
	this->setScale(0.14);
	//����λ��
	this->setAnchorPoint(Vec2(0.3, 0.5));
	return true;
}

void ShotGun::fire(Scene* _currentScene, const Vec2& pos, Entity* player) {
	player->getSprite()->setFlippedY(true);
	this->setFlippedY(true);

	//����������������
	auto direction = pos - this->getParent()->getPosition();
	direction.normalize();
	if (direction.x >= 0) {
		float temp = (-1)*(180 / PI)* atan(direction.y / direction.x);
		this->setRotation(temp);
	}
	else if (direction.x < 0) {
		//player->getSprite()->flip
		//this->setFlippedY(true);
	}
	

	direction = reduceBy15Degree(reduceBy15Degree(direction));
	
	//�����ӵ�
	for (int i = 0; i < 5; i++, direction = increaseBy15Degree(direction)) {
		auto bullet = Bullet::create(_bulletType, this, direction, _currentScene);
		bullet->setScale(1.5);

		//�����ӵ���ʼλ��
		Vec2 bulletPosition = this->getParent()->getPosition();
		bulletPosition.x += 10;
		bulletPosition.y -= 12;
		bullet->setPosition(bulletPosition);

		player->getCurrentMap()->addChild(bullet);
		bullet->new_move();
	}
	player->getSprite()->setFlippedY(false);
	this->setFlippedY(false);
}

//�ı��ӵ����򣬾���ĽǶȸ����Ǻ������ֵ�ƿ�
Vec2 ShotGun::increaseBy15Degree(const Vec2& pos) {
	Vec2 new_pos(0, 0);
	new_pos.x = cos(PI / 12)*pos.x - sin(PI / 12)*pos.y;
	new_pos.y = sin(PI / 12)*pos.x + cos(PI / 12)*pos.y;
	return new_pos;

}
Vec2 ShotGun::reduceBy15Degree(const Vec2& pos) {
	Vec2 new_pos(0, 0);
	new_pos.x = cos(PI / 12)*pos.x + sin(PI / 12)*pos.y;
	new_pos.y = -sin(PI / 12)*pos.x + cos(PI / 12)*pos.y;
	return new_pos;
}
