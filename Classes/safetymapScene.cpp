#include "HelloWorldScene.h"
#include "MyHelloWorldScene.h"
#include "audio.h"
#include "safetymapScene.h"

Scene* safetymap::createScene()
{
	return safetymap::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool safetymap::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}
	// 初始化Physics
	if (!Scene::initWithPhysics())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/*play game music*/
	audio_home->stopBackgroundMusic();
	audio_game->playBackgroundMusic("game_music.mp3", true);

	std::string floor_layer_file = "myfirstmap2.tmx";//��ͼ�ļ�

	_tiledmap = TMXTiledMap::create(floor_layer_file);
	_tiledmap->setAnchorPoint(Vec2::ZERO);
	_tiledmap->setPosition(Vec2::ZERO);

	log("map size:(%f, %f)", _tiledmap->getContentSize().width,_tiledmap->getContentSize().height);

	//添加player并绑定武�?

	Sprite* player_sprite = Sprite::create("turn right 1.png");
	Knight* mplayer = Knight::create();
	Gun* initialWeapon = Gun::create("BrokenPistol.png");
	ShotGun* secondWeapon = ShotGun::create("GoblinShotGun.png");
	mplayer->bindSprite(player_sprite);
	mplayer->bindInitWeapon(initialWeapon);
	mplayer->bindWeapon(secondWeapon);

	mplayer->setTiledMap(_tiledmap);

	
	TMXObjectGroup* objGroup = _tiledmap->getObjectGroup("objects");

	ValueMap player_point_map = objGroup->getObject("PlayerPoint");
	float playerX = player_point_map.at("x").asFloat();
	float playerY = player_point_map.at("y").asFloat();

	//设置玩家坐标
	mplayer->setPosition(Point(playerX,playerY));

	//添加一个测试用的monster
	Sprite* monster_sprite = Sprite::create("LongRemoteSoldier.png");
	RemoteSoldier* monster = RemoteSoldier::create(LONGREMOTE,this);

	monster->bindSprite(monster_sprite);
	//monster->setTiledMap(_tiledmap);

	TMXObjectGroup* bulletGroup = _tiledmap->getObjectGroup("bullet");

	ValueMap monster_point_map = bulletGroup->getObject("bullet1");
	float monsterX = monster_point_map.at("x").asFloat();
	float monsterY = monster_point_map.at("y").asFloat();
	monster->setPosition(Point(monsterX, monsterY));
	
	//创建怪物
	RemoteSoldierManager* remoteSoldierManager = RemoteSoldierManager::create(this, mplayer, _tiledmap);
	this->m_remoteSoldierManager = remoteSoldierManager;
	_tiledmap->addChild(remoteSoldierManager, 4);

	//创建玩家简单移动控制器
	SimpleMoveController* simple_move_controller = SimpleMoveController::create();

	//设置移动速度
	simple_move_controller->set_ixspeed(0);
	simple_move_controller->set_iyspeed(0);

	//将控制器添加到场景中让Upadate被调�?
	this->addChild(simple_move_controller);

	//设置控制器到主角身上
	mplayer->set_controller(simple_move_controller);
	simple_move_controller->bind_player(mplayer);
	simple_move_controller->bind_scene(this);
  
	//设置碰撞掩码
	this->m_player = mplayer;
	this->m_monster = monster;

	m_monster->getPhysicsBody()->setCategoryBitmask(0x02);
	m_monster->getPhysicsBody()->setContactTestBitmask(0x04);
	

	this->addChild(_tiledmap);

	_tiledmap->addChild(mplayer,2);
	_tiledmap->addChild(monster, 2);
	
	//this->addChild(monster,2);
	//this->addChild(mplayer,2);
	log("player pos0:(%f, %f)", playerX, playerY);
	log("monster pos0:(%f, %f)", monsterX, monsterY);

	
	//创建EventListener
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(safetymap::onTouchBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	
	//创建contactListener
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(safetymap::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	return true;
}

bool safetymap::onTouchBegin(Touch* touch, Event* event) {
	/*
		Vec2 pos = m_player->getLockedTarget()->getPosition();
		m_player->rotateWeapon(pos);
		m_player->attack(this, pos);
	*/
	//�����ǰû�������Ĺ�����������Ĺ����Ѿ�����
	if (m_player->getLockedTarget() == NULL ||
		m_player->getLockedTarget()->getalreadyDead()) {
		
		//�½�һ��target������ָ������Ļ��soldier
		RemoteSoldier* target = NULL;
		
		//����soldiermanager,�ҳ�����Ļ��soldier
		for (auto soldier : this->m_remoteSoldierManager->getSoldierArr()){	
			//��������ģ�����
			if (soldier->getalreadyDead()) {
				continue;
			}

			//���soldierû���Ļ��������soldier��player�ľ���
			Vec2 direction = soldier->getPosition() - m_player->getPosition();
			float distance = sqrt(direction.x*direction.x + direction.y*direction.y);
			static float minDistance = distance;

			//�����ǰsoldier����������ģ���ô��target����Ϊ���soldier
			if (minDistance >= distance) {
				target = soldier;
			}

			//����������ˣ���ôtarget����䣬һֱ��NULL
		}

		//����ҵ�������Ŀ�꣬��ô����������
		if (target != NULL) {
			m_player->setLockedTarget(target);
			Vec2 pos = m_player->getLockedTarget()->getPosition();
			m_player->rotateWeapon(pos);
			m_player->attack(this, pos);
		}
		
		//���û�ҵ�������Ŀ�꣬����ǰ������
		else {
			m_player->resetWeaponPos();
			m_player->attack(this, Vec2(m_player->getPositionX() + 1, m_player->getPositionY()));
		}
	}
	
	//��������ķ��棬����������Ŀ���Ҹ�Ŀ���ǻ��ŵ�
	else {
		//ֱ�ӹ�����Ŀ��
		Vec2 pos = m_player->getLockedTarget()->getPosition();
		m_player->rotateWeapon(pos);
		m_player->attack(this, pos);
	}
	return true;
}

bool safetymap::onContactBegin(PhysicsContact& contact) {

	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();

	if (nodeA && nodeB)
	{
		if (nodeA->getTag() > 0)
		{
			if (nodeB->getTag() == -1)
			{
				this->m_player->takeDamage(nodeA->getTag());
			}
			else if (nodeB->getTag() == -2)
			{
				//this->m_monster->takeDamage(nodeA->getTag());
				//�ҳ����ĸ�������ӵ���������ײ
				for (auto Soldier : this->m_remoteSoldierManager->getSoldierArr())
				{
					if (nodeB->getMonsterID() == Soldier->getMonsterID())
					{
						Soldier->takeDamage(nodeA->getTag());
					}
				}
			}
			nodeA->removeFromParentAndCleanup(true);
		}

		//������Ĵ�����Ǿ���ģ���ΪnodeA��nodeB��֪����һ�����ӵ�
		else if (nodeB->getTag() > 0)
		{
			if (nodeA->getTag() == -1)
			{
				this->m_player->takeDamage(nodeB->getTag());
			}
			else if (nodeA->getTag() == -2)
			{
				//this->m_monster->takeDamage(nodeB->getTag());
				for (auto Soldier : this->m_remoteSoldierManager->getSoldierArr())
				{
					if (nodeA->getMonsterID() == Soldier->getMonsterID())
					{
						Soldier->takeDamage(nodeB->getTag());
					}
				}
			}
			nodeB->removeFromParentAndCleanup(true);
		}
	}
		return true;
}

/*void safetymap::add_player(TMXTiledMap* map)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	//����һ����
	Sprite* player_sprite = Sprite::create("player.png");
	Player* mplayer = Player::create();
	mplayer->bind_sprite(player_sprite);
	mplayer->run();

    //����������
	mplayer->setPosition(Vec2(100, visibleSize.height / 2));

	_tiledmap->addChild(mplayer);
}*/



