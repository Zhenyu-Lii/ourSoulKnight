#include "Player.h"
#include "safetymapScene.h"

Player::Player()
{
	_HP = 5;
	_MP = 100;
	_AC = 5;
	_alreadyDead = false;
	_lockedTarget = NULL;
}

Player::~Player() {

}
bool Player::init()
{
	return true;
}

bool Player::bindSprite(Sprite*sprite) {
	this->m_sprite = sprite;
	if (m_sprite == nullptr)
	{
		printf("m_sprite in this entity is nullptr, check wether the file used to create the sprite in right dictionary.");
		return false;
	}
	else
	{
		this->addChild(m_sprite);
		Size size = m_sprite->getContentSize();
		m_sprite->setPosition(Point(size.width*0.5f, size.height*0.5f));
		this->setContentSize(size);
		this->setAnchorPoint(Vec2(0.5, 0.5));

		//���������ײģ��
		auto physicsBody = PhysicsBody::createBox(size, PhysicsMaterial(0.0f, 0.0f, 0.0f));
		physicsBody->setDynamic(false);
		physicsBody->setCategoryBitmask(0x01);
		physicsBody->setContactTestBitmask(0x04);
		this->addComponent(physicsBody);

		return true;
	}
}
bool Player::bindInitWeapon(Weapon* weapon) {
	if (weapon == nullptr) {
		printf("_currentWeapon in this player is nullptr, check wether the file used to create the weapon in right dictionary.");
		return false;
	}
	else {
		_weaponBag.pushBack(weapon);
		_currentWeapon = weapon;
		_attack = _currentWeapon->getAttack();

		//����������λ��
		Size size = m_sprite->getContentSize();
		_currentWeapon->setPosition(Vec2(size.width*getWpPos().x, size.height*getWpPos().y));
		this->addChild(_currentWeapon);

		return true;
	}
}

//和角色武器相关的函数
bool Player::bindWeapon(Weapon* weapon) {
	if (weapon == nullptr) {
		printf("_currentWeapon in this player is nullptr, check wether the file used to create the weapon in right dictionary.");
		return false;
	}
	else {
		this->_weaponBag.pushBack(weapon);
	
		//����������λ��
		Size size = m_sprite->getContentSize();
		_weaponBag.back()->setPosition(Vec2(size.width*getWpPos().x, size.height*getWpPos().y));//*getWpPos().x
		this->addChild(_weaponBag.back());
		_weaponBag.back()->setVisible(false);
		return true;
	}
}

void Player::attack(Scene* currentScene,const Vec2& pos) {
	if (_MP - _currentWeapon->getMpConsume() >= 0) {
		_MP -= _currentWeapon->getMpConsume();
		this->_currentWeapon->fire(currentScene, pos, this);
		log("player pos:(%f,%f)", this->getPositionX(), this->getPositionY());
	}
}

void Player::rotateWeapon(const Vec2& pos) {
	auto direction = pos - this->getPosition();
	float x = direction.x;
	float y = direction.y;
	if (x > 0 && y > 0) {
		this->_currentWeapon->setRotation(-45.0f);
	}
	else if (x > 0 && y < 0) {
		this->_currentWeapon->setRotation(+45.0f);
	}
}

void Player::resetWeaponPos() {
	this->_currentWeapon->setRotation(0.0f);
}

void Player::switchWeapon() {
	_currentWeapon->setVisible(false);
	for (auto weapon : _weaponBag) {
		if (_currentWeapon != weapon) {
			_currentWeapon = weapon;
			break;
		}
	}
	_currentWeapon->setVisible(true);
}

void Player::skill() {

}

void Player::takeDamage(int damage)
{
	if (_AC > 0)
	{
		_AC -= damage;
	}
	else 
	{
		_HP -= damage;
		if (_HP <= 0)
		{
			_alreadyDead = true;
			this->die();
		}
	}
}

void Player::die()
{

	this->setVisible(false);
	
}

//和键盘控制相关的函数
void Player::setViewPointByPlayer()
{
	if (m_sprite == NULL)
		return;
	Layer* parent = (Layer*)getParent();

	//��ͼ������
	Size mapTiledNum = m_map->getMapSize();

	//��ͼ�������Ӵ�С
	Size tiledSize = m_map->getTileSize();

	//��ͼ��С
	Size mapSize = Size(mapTiledNum.width*tiledSize.width, mapTiledNum.height*tiledSize.height);

	//��Ļ��С
	auto visibleSize = Director::getInstance()->getVisibleSize();

	//��������
	Point spritePos = getPosition();

	float x = std::max(spritePos.x,visibleSize.width/2);
	float y = std::max(spritePos.y, visibleSize.height / 2);

	//����������곬����Χ����ȡ��������
	x = std::min(x, mapSize.width - visibleSize.width / 2);
	y = std::min(y, mapSize.height - visibleSize.height / 2);

	//Ŀ���
	Point desPos = Point(x, y);

	Point centPos = Point(visibleSize.width / 2, visibleSize.height / 2);

	Point viewPos = centPos - desPos;

	parent->setPosition(viewPos);

}

void Player::set_tag_position(int x, int y)
{
	/*�ж�ǰ���Ƿ񲻿�ͨ��*/
	Size spriteSize = m_sprite->getContentSize();
	Point dstPos = Point(x+spriteSize.width/2, y);
	Point dstPos_y = Point(x + spriteSize.width / 2, y - spriteSize.height / 2);
	//������ΪPlayer��Ӧ����ƫ�µ�λ�ã�Ϊ���ж�Player�·��Ľ�����

	/*�������ǰ����ͼ����λ��*/
	Point tiledPos = tileCoordForPosition(Point(dstPos.x, dstPos.y));
	Point tiledPos_right = tileCoordForPosition(Point(dstPos.x + spriteSize.width / 2, dstPos.y));
	Point tiledPos_bottom = tileCoordForPosition(Point(dstPos.x, dstPos.y- spriteSize.height / 2));
	//�Ըþ������ڸ��ӵ�ǰ���жϣ�

	/*��õ�ͼ���ӵ�Ψһ��ʶ*/  
	int tileGid = meta->getTileGIDAt(tiledPos);
	int tiledGid_right = meta->getTileGIDAt(tiledPos_right); 
	int tiledGid_bottom = meta->getTileGIDAt(tiledPos_bottom);
	//int tileGid = meta->getTileGIDAt(Point(16,12));
	if (tileGid != 0)
	{
		Value properties = m_map->getPropertiesForGID(tileGid);

		Value proper = properties.asValueMap().at("Collidable");

		if (proper.asString().compare("true") == 0)
		{
			return;
		}
	}
	if(tiledGid_right != 0)
	{
		Value properties = m_map->getPropertiesForGID(tiledGid_right);

		Value proper = properties.asValueMap().at("Collidable");

		if (proper.asString().compare("true") == 0)
			return;
	}
	if (tiledGid_bottom != 0)
	{
		Value properties = m_map->getPropertiesForGID(tiledGid_bottom);

		Value proper = properties.asValueMap().at("Collidable");

		if (proper.asString().compare("true") == 0)
			return;
	}
	
	Entity::set_tag_position(x, y);

	setViewPointByPlayer();
}

void Player::setTiledMap(TMXTiledMap* map)
{
	this->m_map = map;

	this->meta = m_map->getLayer("meta");
	this->meta->setVisible(false);
}

Point Player::tileCoordForPosition(Point pos) {
	Size mapTiledNum = m_map->getMapSize();
	Size tiledSize = m_map->getTileSize();

	int x, y;
    x = (pos.x*1.8) / tiledSize.width;

    /*y坐标需要转换一下，因为坐标系和tiled不同*/
	y = (mapTiledNum.height*tiledSize.height - pos.y*1.8) / tiledSize.height;

	/*���Ӵ��㿪ʼ*/
	if (x > 0)
		x--;
	if (y > 0)
		y-=0;

	return Point(x, y);
}


