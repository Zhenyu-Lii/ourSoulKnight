#include "HelloWorldScene.h"
#include "MyHelloWorldScene.h"
#include "audio.h"
#include"adventuremapScene.h"
#include"KnightStartMapScene.h"
Scene* adventuremap::createScene()
{
	return adventuremap::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool adventuremap::init()
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
	this->scheduleUpdate();
	musicOnOff = true;
	/*add information bar*/
	auto information_bar = Sprite::create("data_bars_blank.png");
	information_bar->setPosition(Vec2(origin.x + 55, visibleSize.height - 10));
	this->addChild(information_bar, 1);

	/*add blood bar
	auto bloodLoadingBar = LoadingBar::create("blood_bar_full.png");
	bloodLoadingBar->setDirection(LoadingBar::Direction::LEFT);
	bloodLoadingBar->setPosition(Vec2(origin.x + 55, visibleSize.height - 10));
	bloodLoadingBar->setPercent(100);
	this->addChild(bloodLoadingBar,2);*/

	/*second method*/
	auto bloodEmpty = Sprite::create("blood_bar_empty.png");
	bloodEmpty->setPosition(Vec2(origin.x + 55, visibleSize.height + 5));
	this->addChild(bloodEmpty, 2);
	auto bloodFull = Sprite::create("blood_bar_full.png");
	bloodProgress = ProgressTimer::create(bloodFull);
	bloodProgress->setType(ProgressTimer::Type::BAR);//type:bar
	bloodProgress->setPosition(Vec2(origin.x + 55, visibleSize.height + 5));
	bloodProgress->setMidpoint(Point(0, 0.5));//from right to left
	bloodProgress->setBarChangeRate(Point(1, 0));
	//bloodProgress->setTag(BLOOD_BAR);//
	this->addChild(bloodProgress, 2);

	/*add HP text*/
	bloodNum = "5/5";
	bloodLabel = Label::createWithTTF(bloodNum, "fonts/Marker Felt.ttf", 7);
	bloodLabel->setPosition(Vec2(origin.x + 55, visibleSize.height + 5));
	this->addChild(bloodLabel, 2);

	/*add MP bar*/
	auto MPEmpty = Sprite::create("mana_bar_empty.png");
	MPEmpty->setPosition(Vec2(origin.x + 55, visibleSize.height - 22));
	this->addChild(MPEmpty, 2);
	auto MPFull = Sprite::create("mana_bar_full.png");
	MPProgress = ProgressTimer::create(MPFull);
	MPProgress->setType(ProgressTimer::Type::BAR);//type:bar
	MPProgress->setPosition(Vec2(origin.x + 55, visibleSize.height - 22));
	MPProgress->setMidpoint(Point(0, 0.5));//from right to left
	MPProgress->setBarChangeRate(Point(1, 0));
	//bloodProgress->setTag(BLOOD_BAR);//
	this->addChild(MPProgress, 2);

	/*add MP text*/
	MPNum = "180/180";
	MPLabel = Label::createWithTTF(MPNum, "fonts/Marker Felt.ttf", 7);
	MPLabel->setPosition(Vec2(origin.x + 55, visibleSize.height - 22));
	this->addChild(MPLabel, 3);

	/*add AC text*/
	ACNum = "5/5";
	ACLabel = Label::createWithTTF(ACNum, "fonts/Marker Felt.ttf", 7);
	ACLabel->setPosition(Vec2(origin.x + 55, visibleSize.height - 10));
	this->addChild(ACLabel, 3);

	/*add AC bar*/
	auto ACEmpty = Sprite::create("shield_bar_empty.png");
	ACEmpty->setPosition(Vec2(origin.x + 55, visibleSize.height - 10));
	this->addChild(ACEmpty, 2);
	auto ACFull = Sprite::create("shield_bar_full.png");
	ACProgress = ProgressTimer::create(ACFull);
	ACProgress->setType(ProgressTimer::Type::BAR);//type:bar
	ACProgress->setPosition(Vec2(origin.x + 55, visibleSize.height - 10));
	ACProgress->setMidpoint(Point(0, 0.5));//from right to left
	ACProgress->setBarChangeRate(Point(1, 0));
	//bloodProgress->setTag(BLOOD_BAR);//
	this->addChild(ACProgress, 2);
	schedule(CC_SCHEDULE_SELECTOR(adventuremap::scheduleBlood), 0.1f);

	auto suspend_button = MenuItemImage::create(
		"suspend_button.png",
		"suspend_button.png",
		CC_CALLBACK_1(adventuremap::menuCloseCallback, this));
	if (suspend_button == nullptr ||
		suspend_button->getContentSize().width <= 0 ||
		suspend_button->getContentSize().height <= 0)
	{
		problemLoading("'suspend_button.png' and 'suspend_button.png'");
	}
	else
	{
		suspend_button->setPosition(Vec2(visibleSize.width + origin.x - 20, visibleSize.height + origin.y - 20));
	}
	auto menu2 = Menu::create(suspend_button, NULL);
	menu2->setPosition(Vec2::ZERO);
	this->addChild(menu2, 1);//just a virtual button which is unvisible
	/*play game music*/

	std::string floor_layer_file = "adventuremap2.tmx";//��ͼ�ļ�

	_tiledmap = TMXTiledMap::create(floor_layer_file);
	_tiledmap->setAnchorPoint(Vec2::ZERO);
	_tiledmap->setPosition(Vec2::ZERO);

	log("map size:(%f, %f)", _tiledmap->getContentSize().width, _tiledmap->getContentSize().height);

	//添加player并绑定武�?

	Sprite* player_sprite = Sprite::create("turn right 1.png");
	mplayer = Knight::create();
	Sword* initialWeapon = Sword::create("BroadSword.png");
	ShotGun* secondWeapon = ShotGun::create("GoblinShotGun.png");
	mplayer->bindSprite(player_sprite);
	mplayer->bindInitWeapon(initialWeapon);
	mplayer->bindWeapon(secondWeapon);

	mplayer->setTiledMap(_tiledmap);


	TMXObjectGroup* objGroup = _tiledmap->getObjectGroup("objects");

	ValueMap player_point_map = objGroup->getObject("PlayerPoint");
	float playerX = player_point_map.at("x").asFloat();
	float playerY = player_point_map.at("y").asFloat();

	/*add next map sprite*/
	TMXObjectGroup* NextGroup = _tiledmap->getObjectGroup("exit");

	ValueMap next_map = NextGroup->getObject("exit1");
	float exitX = next_map.at("x").asFloat();
	float exitY = next_map.at("y").asFloat();
	auto next_map_sprite = Sprite::create("next_pass_1.png");
	next_map_sprite->setScale(0.3);
	next_map_sprite->setPosition(Point(exitX, exitY));
	_tiledmap->addChild(next_map_sprite, 1);
	auto exitAnimation = Animation::create();
	char nameSize[20] = { 0 };
	for (int i = 1; i <= 8; i++)
	{
		sprintf(nameSize, "next_pass_%d.png", i);
		exitAnimation->addSpriteFrameWithFile(nameSize);
	}
	exitAnimation->setDelayPerUnit(0.1f);//���ö���֡ʱ����
	exitAnimation->setLoops(-1);
	exitAnimation->setRestoreOriginalFrame(true);
	auto exitAnimate = Animate::create(exitAnimation);
	next_map_sprite->runAction(exitAnimate);
	//设置玩家坐标
	mplayer->setPosition(Point(playerX, playerY));

	//添加一个测试用的monster
	Sprite* monster_sprite = Sprite::create("LongRemoteSoldier.png");
	RemoteSoldier* monster = RemoteSoldier::create(LONGREMOTE, this);

	monster->bindSprite(monster_sprite);
	//monster->setTiledMap(_tiledmap);


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

	//设置碰撞掩码
	this->m_player = mplayer;
	this->m_monster = monster;

	m_monster->getPhysicsBody()->setCategoryBitmask(0x02);
	m_monster->getPhysicsBody()->setContactTestBitmask(0x04);


	this->addChild(_tiledmap);

	_tiledmap->addChild(mplayer, 2);
	_tiledmap->addChild(monster, 2);

	//this->addChild(monster,2);
	//this->addChild(mplayer,2);

	//创建EventListener
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(adventuremap::onTouchBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	//创建contactListener
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(adventuremap::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	return true;
}

bool adventuremap::onTouchBegin(Touch* touch, Event* event) {
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
		for (auto soldier : this->m_remoteSoldierManager->getSoldierArr()) {
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

bool adventuremap::onContactBegin(PhysicsContact& contact) {

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


void adventuremap::scheduleBlood(float delta)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	float HpRate = (float)mplayer->get_HP() / 5.0f;
	bloodProgress->setPercentage(HpRate * 100);


	sprintf(bloodChar, "%d/5", mplayer->get_HP() >= 0 ? mplayer->get_HP() : 0);
	bloodNum = bloodChar;
	bloodLabel->setString(bloodNum);


	sprintf(ACChar, "%d/5", mplayer->get_AC() >= 0 ? mplayer->get_AC() : 0);
	ACNum = ACChar;
	ACLabel->setString(ACNum);

	sprintf(MPChar, "%d/180", mplayer->get_MP() >= 0 ? mplayer->get_MP() : 0);
	MPNum = MPChar;
	MPLabel->setString(MPNum);

	float MPRate = (float)mplayer->get_MP() / 180.0f;
	MPProgress->setPercentage(MPRate * 100);

	float ACRate = (float)mplayer->get_AC() / 5.0f;
	ACProgress->setPercentage(ACRate * 100);
	if (ACProgress->getPercentage() < 0 && MPProgress->getPercentage() < 0 && bloodProgress->getPercentage() < 0)
	{
		this->unschedule(CC_SCHEDULE_SELECTOR(adventuremap::scheduleBlood));
	}

}

void adventuremap::update(float dt)
{
	auto player_x = this->mplayer->getPositionX();
	auto player_y = this->m_player->getPositionY();
	int x = player_x * 1.8 / 32;
	int y = (2560 - player_y * 1.8) / 32;

	if (x <= 70 && x >= 68 && y <= 44 && y >= 42)
	{
		//get to the next map
		Director::getInstance()->replaceScene(KnightStartMap::createScene());
	}
}

void adventuremap::menuCloseCallback(Ref* pSender)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	suspend_scene = Sprite::create("suspend_scene.png");
	suspend_scene->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 300));
	this->addChild(suspend_scene, 1);
	auto suspend_scene_moveBy = MoveBy::create(0.3, Vec2(0, -300));
	suspend_scene->runAction(suspend_scene_moveBy);

	suspend_start = MenuItemImage::create(
		"suspend_start.png",
		"suspend_start.png",
		CC_CALLBACK_1(adventuremap::start_menuCloseCallback, this));
	if (suspend_start == nullptr ||
		suspend_start->getContentSize().width <= 0 ||
		suspend_start->getContentSize().height <= 0)
	{
		problemLoading("'suspend_button.png' and 'suspend_button.png'");
	}
	else
	{

		suspend_start->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 40 + 300));
		auto suspend_start_moveBy = MoveBy::create(0.3, Vec2(0, -300));
		suspend_start->runAction(suspend_start_moveBy);
	}
	auto menu2 = Menu::create(suspend_start, NULL);
	menu2->setPosition(Vec2::ZERO);
	this->addChild(menu2, 1);//just a virtual button which is unvisible
	/*create home button*/
	home_button = MenuItemImage::create(
		"home_button1.png",
		"home_button2.png",
		CC_CALLBACK_1(adventuremap::home_menuCloseCallback, this));
	if (suspend_start == nullptr ||
		suspend_start->getContentSize().width <= 0 ||
		suspend_start->getContentSize().height <= 0)
	{
		problemLoading("'home_button1' and 'home_button2.png'");
	}
	else
	{

		home_button->setPosition(Vec2(visibleSize.width / 2 - 60, visibleSize.height / 2 - 40 + 300));
		auto home_button_moveBy = MoveBy::create(0.3, Vec2(0, -300));
		home_button->runAction(home_button_moveBy);
	}
	auto menu3 = Menu::create(home_button, NULL);
	menu3->setPosition(Vec2::ZERO);
	this->addChild(menu3, 1);//just a virtual button which is unvisible


	music_button = MenuItemImage::create(
		"volume_on.png",
		"volume_off.png",
		CC_CALLBACK_1(adventuremap::music_menuCloseCallback, this));
	if (music_button == nullptr ||
		music_button->getContentSize().width <= 0 ||
		music_button->getContentSize().height <= 0)
	{
		problemLoading("'music_button'");
	}
	else
	{

		music_button->setPosition(Vec2(visibleSize.width / 2 + 60, visibleSize.height / 2 - 40 + 300));
		auto music_button_moveBy = MoveBy::create(0.3, Vec2(0, -300));
		music_button->runAction(music_button_moveBy);
		music_button->setScale(0.4);
	}
	auto menu4 = Menu::create(music_button, NULL);
	menu4->setPosition(Vec2::ZERO);
	this->addChild(menu4, 1);//just a virtual button which is unvisible
}

void adventuremap::start_menuCloseCallback(Ref* pSender)
{
	auto suspend_moveby = MoveBy::create(0.3f, Vec2(0, 300));
	suspend_scene->runAction(suspend_moveby);

	auto home_moveby = MoveBy::create(0.3f, Vec2(0, 300));
	home_button->runAction(home_moveby);

	auto start_moveby = MoveBy::create(0.3f, Vec2(0, 300));
	suspend_start->runAction(start_moveby);


	auto music_moveby = MoveBy::create(0.3f, Vec2(0, 300));
	music_button->runAction(music_moveby);
}

void adventuremap::home_menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->replaceScene(HelloWorld::createScene());
}

void adventuremap::music_menuCloseCallback(Ref* pSender)
{
	if (musicOnOff == true)
	{
		audio_begin->pauseBackgroundMusic();
		musicOnOff = false;
	}
	else
	{
		audio_begin->resumeBackgroundMusic();
		musicOnOff = true;
	}

}



