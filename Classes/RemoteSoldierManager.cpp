#include "RemoteSoldierManager.h"

RemoteSoldierManager* RemoteSoldierManager::create(Scene* currentScene, Entity* player, TMXTiledMap* map)
{
	RemoteSoldierManager* remoteSoldierManager = new RemoteSoldierManager;
	if (remoteSoldierManager && remoteSoldierManager->init(currentScene, player, map))
	{
		remoteSoldierManager->autorelease();
		return remoteSoldierManager;
	}
	CC_SAFE_DELETE(remoteSoldierManager);
	return nullptr;
}
bool RemoteSoldierManager::init(Scene* currentScene, Entity* player, TMXTiledMap* map)
{
	_currentScene = currentScene;
	_player = player;
	_map = map;
	createMonsters();
	this->scheduleUpdate();
	this->schedule(CC_SCHEDULE_SELECTOR(RemoteSoldierManager::attackUpdate), 2.0f);
	return true;
}

void RemoteSoldierManager::createMonsters()
{
	RemoteSoldier* remoteSoldier = nullptr;
	Sprite* sprite = nullptr;
	srand((unsigned)time(nullptr));
	for (int i = 0; i < MAX_REMOTE_SOLDIER_NUM; i++)
	{
		int random = rand() % 2;
		log("random:%d", random);
		if (random == 0) 
		{
			remoteSoldier = RemoteSoldier::create(LONGREMOTE, _currentScene);
			sprite = Sprite::create("LongRemoteSoldier.png");
			remoteSoldier->bindSprite(sprite);
			remoteSoldier->setMonsterID(i);
		}
		else
		{
			remoteSoldier = RemoteSoldier::create(SHORTREMOTE, _currentScene);
			sprite = Sprite::create("ShortRemoteSoldier.png");
			remoteSoldier->bindSprite(sprite);
			remoteSoldier->setMonsterID(i);

		}
		remoteSoldier->setPosition(this->setSoldierPosition(i));
		this->addChild(remoteSoldier);
		m_remoteSoldierArr.pushBack(remoteSoldier);
	}
}

void RemoteSoldierManager::update(float dt)
{
	int i = 0;
	for (auto remoteSoldier : m_remoteSoldierArr)
	{
		srand((unsigned)time(nullptr));
		if(!(remoteSoldier->getalreadyDead()))
		{
			i++;
			int random = (rand()+ i) % 4;
			//log("random:%d", random);
			//todo:关于地图边界的问题
			if (random == 0) //up
			{
				remoteSoldier->setPositionY(remoteSoldier->getPositionY() + 1);
			}
			else if (random == 1) //down
			{
				remoteSoldier->setPositionY(remoteSoldier->getPositionY() - 1);
			}
			else if (random == 2) //left
			{
				remoteSoldier->setPositionX(remoteSoldier->getPositionX() - 1);
			}
			else //right
			{
				remoteSoldier->setPositionX(remoteSoldier->getPositionX() + 1);
			}
			//log("remoteSoldier:x=%f, y=%f", remoteSoldier->getPositionX(), remoteSoldier->getPositionY());
		}
	}
}

void RemoteSoldierManager::attackUpdate(float dt)
{
	for (auto remoteSoldier : m_remoteSoldierArr)
	{
		if (!(remoteSoldier->getalreadyDead()))
		{
			float difX = remoteSoldier->getPositionX() - _player->getPositionX();
			float difY = remoteSoldier->getPositionY() - _player->getPositionY();
			if(difX * difX + difY * difY <= LONG_REMOTE_SOLDIER_ATTACK_RADIUS* LONG_REMOTE_SOLDIER_ATTACK_RADIUS)
				remoteSoldier->attack(_player);
		}
	}
}

Vec2 RemoteSoldierManager::setSoldierPosition(int num)
{
	TMXObjectGroup* objGroup = _map->getObjectGroup("bullet");
	float remoteSoldierX, remoteSoldierY;
	if (num == 0) 
	{
		ValueMap remoteSoldierMap = objGroup->getObject("remote_bullet1");
		remoteSoldierX = remoteSoldierMap.at("x").asFloat();
		remoteSoldierY = remoteSoldierMap.at("y").asFloat();
		//log("startremoteSoldier:x=%f, y=%f", remoteSoldierX, remoteSoldierY);
	}
	else if (num == 1)
	{
		ValueMap remoteSoldierMap = objGroup->getObject("remote_bullet2");
		remoteSoldierX = remoteSoldierMap.at("x").asFloat();
		remoteSoldierY = remoteSoldierMap.at("y").asFloat();
	}
	else if(num == 2)
	{
		ValueMap remoteSoldierMap = objGroup->getObject("remote_bullet3");
		remoteSoldierX = remoteSoldierMap.at("x").asFloat();
		remoteSoldierY = remoteSoldierMap.at("y").asFloat();
	}
	else if (num == 3)
	{
		ValueMap remoteSoldierMap = objGroup->getObject("remote_bullet4");
		remoteSoldierX = remoteSoldierMap.at("x").asFloat();
		remoteSoldierY = remoteSoldierMap.at("y").asFloat();
	}
	else
	{
		ValueMap remoteSoldierMap = objGroup->getObject("remote_bullet5");
		remoteSoldierX = remoteSoldierMap.at("x").asFloat();
		remoteSoldierY = remoteSoldierMap.at("y").asFloat();
	}
	return Vec2(remoteSoldierX, remoteSoldierY);
}