#include "Ax.h"


Ax::Ax() {
	//��������
	_attack = 10;
	_mpConsumption = 0;
	_attackRadius = 14;
	_bulletSpeed = 300;
	_bulletType = PISTOLBULLET;

}

Ax::~Ax() {

}

Ax* Ax::create(const std::string& filename) {
	Ax* sprite = new Ax;
	if (sprite && sprite->init(filename)) {
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

bool Ax::init(const std::string& filename) {
	if (!Sprite::initWithFile(filename)) {
		return false;
	}
	this->setRotation(-50.0f);
	this->setScale(0.08);
	this->setAnchorPoint(Vec2(0.1, 0.5));
	this->setTag(_attack);

	/*auto physicsBody = PhysicsBody::createBox(this->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);

	//��������ʱ��ͨ����ײ���뽫�����ó��޷��������ײ
	physicsBody->setCategoryBitmask(0x00);
	physicsBody->setContactTestBitmask(0x00);
	this->setTag(_attack);
	this->addComponent(physicsBody);*/

	//������������
	return true;
}

void Ax::fire(Scene* _currentScene, const Vec2& pos, Entity* player) {
	//this->getPhysicsBody()->setCategoryBitmask(0x04);
	//this->getPhysicsBody()->setContactTestBitmask(0x02);
	auto direction = pos - this->getParent()->getPosition();
	direction.normalize();

	//������ս����������Χ
	auto bullet = Bullet::create(_bulletType, this, direction, _currentScene);
	bullet->setScale(1.5);

	//������ս����������Χ
	bullet->setAnchorPoint(Vec2(0.4, 0.5));
	bullet->setRotation(90.0f);
	bullet->setVisible(false);

	//������ս����������Χ�ĳ�ʼλ��
	Vec2 bulletPosition = this->getParent()->getPosition();
	bulletPosition.x;
	bulletPosition.y;
	bullet->setPosition(bulletPosition);

	player->getCurrentMap()->addChild(bullet);
	bullet->new_move();

	auto attackMovement = RotateBy::create(0.1f, 360.0f);
	auto resetMove = RotateTo::create(0.05f, -50.0f);
	this->runAction(Sequence::create(attackMovement, resetMove, NULL));
	
}
