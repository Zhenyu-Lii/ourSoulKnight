#include "HelloWorldScene.h"
#include "MyHelloWorldScene.h"
#include "audio.h"
#include "safetymapScene.h"
#include"adventuremapScene.h"
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
	// 鍒濆鍖朠hysics
	if (!Scene::initWithPhysics())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	musicOnOff = true;
	/*add information bar*/
	auto information_bar = Sprite::create("data_bars_blank.png");
	information_bar->setPosition(Vec2(origin.x + 55, visibleSize.height - 10));
	this->addChild(information_bar, 1);
	this->scheduleUpdate();
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
	schedule(CC_SCHEDULE_SELECTOR(safetymap::scheduleBlood), 0.1f);

	auto suspend_button = MenuItemImage::create(
		"suspend_button.png",
		"suspend_button.png",
		CC_CALLBACK_1(safetymap::menuCloseCallback, this));
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
	audio_home->stopBackgroundMusic();
	audio_game->playBackgroundMusic("game_music.mp3", true);

	std::string floor_layer_file = "myfirstmap2.tmx";//地图文件

	_tiledmap = TMXTiledMap::create(floor_layer_file);
	_tiledmap->setAnchorPoint(Vec2::ZERO);
	_tiledmap->setPosition(Vec2::ZERO);

	log("map size:(%f, %f)", _tiledmap->getContentSize().width,_tiledmap->getContentSize().height);

	//娣诲姞player骞剁粦瀹氭鍣?

	Sprite* player_sprite = Sprite::create("turn right 1.png");

	mplayer = Knight::create();
	//Knight* mplayer = Knight::create();
	Gun* initialWeapon = Gun::create("BrokenPistol.png");
	Sword* secondWeapon = Sword::create("LightSaber.png");

	/*Sword* initialWeapon = Sword::create("BroadSword.png");
	ShotGun* secondWeapon = ShotGun::create("GoblinShotGun.png");
	*/
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
	exitAnimation->setDelayPerUnit(0.1f);//设置动画帧时间间�?
	exitAnimation->setLoops(-1);
	exitAnimation->setRestoreOriginalFrame(true);
	auto exitAnimate = Animate::create(exitAnimation);
	next_map_sprite->runAction(exitAnimate);
	//璁剧疆鐜╁鍧愭�?
	mplayer->setPosition(Point(playerX,playerY));

	//娣诲姞涓�涓祴璇曠敤鐨刴onster
	Sprite* monster_sprite = Sprite::create("LongRemoteSoldier.png");
	RemoteSoldier* monster = RemoteSoldier::create(LONGREMOTE,this);

	monster->bindSprite(monster_sprite);
	//monster->setTiledMap(_tiledmap);

	TMXObjectGroup* bulletGroup = _tiledmap->getObjectGroup("bullet");


	//鍒涘缓鎬墿
	RemoteSoldierManager* remoteSoldierManager = RemoteSoldierManager::create(this, mplayer, _tiledmap);
	this->m_remoteSoldierManager = remoteSoldierManager;
	_tiledmap->addChild(remoteSoldierManager, 4);

	//鍒涘缓鐜╁绠�鍗曠Щ鍔ㄦ帶鍒跺櫒
	SimpleMoveController* simple_move_controller = SimpleMoveController::create();

	//璁剧疆绉诲姩閫熷�?
	simple_move_controller->set_ixspeed(0);
	simple_move_controller->set_iyspeed(0);

	//灏嗘帶鍒跺櫒娣诲姞鍒板満鏅腑璁︰padate琚皟鐢?
	this->addChild(simple_move_controller);

	//璁剧疆鎺у埗鍣ㄥ埌涓昏韬笂
	mplayer->set_controller(simple_move_controller);
	simple_move_controller->bind_player(mplayer);
	simple_move_controller->bind_scene(this);
  
	//璁剧疆纰版挒鎺╃�?
	this->m_player = mplayer;
	this->m_monster = monster;

	m_monster->getPhysicsBody()->setCategoryBitmask(0x02);
	m_monster->getPhysicsBody()->setContactTestBitmask(0x04);
	

	this->addChild(_tiledmap);

	_tiledmap->addChild(mplayer,2);
	_tiledmap->addChild(monster, 2);
	


	
	//鍒涘缓EventListener
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(safetymap::onTouchBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	
	//鍒涘缓contactListener
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
	//如果当前没有锁定的怪物，或者锁定的怪物已经死了
	if (m_player->getLockedTarget() == NULL ||
		m_player->getLockedTarget()->getalreadyDead()) {
		
		//新建一个target，用于指向最近的活的soldier
		RemoteSoldier* target = NULL;
		
		//遍历soldiermanager,找出最近的活的soldier
		for (auto soldier : this->m_remoteSoldierManager->getSoldierArr()){	
			//如果是死的，跳过
			if (soldier->getalreadyDead()) {
				continue;
			}

			//这个soldier没死的话，计算出soldier和player的距�?
			Vec2 direction = soldier->getPosition() - m_player->getPosition();
			float distance = sqrt(direction.x*direction.x + direction.y*direction.y);
			static float minDistance = distance;

			//如果当前soldier距离是最近的，那么把target设置为这个soldier
			if (minDistance >= distance) {
				target = soldier;
			}

			//如果都死完了，那么target不会变，一直是NULL
		}

		//如果找到了锁定目标，那么锁定并攻�?
		if (target != NULL) {
			m_player->setLockedTarget(target);
			Vec2 pos = m_player->getLockedTarget()->getPosition();
			m_player->rotateWeapon(pos);
			m_player->attack(this, pos);
		}
		
		//如果没找到锁定的目标，就向前方开�?
		else {
			m_player->resetWeaponPos();
			if (m_player->getCurrentWeapon()->isFlippedX() == false) {
				m_player->attack(this, Vec2(m_player->getPositionX() + 1, m_player->getPositionY()));
			}
			else {
				m_player->attack(this, Vec2(m_player->getPositionX() - 1, m_player->getPositionY()));

			}
		}
	}
	
	//上述情况的反面，就是有锁定目标且该目标是活着�?
	else {
		//直接攻击该目�?
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
				//找出是哪个怪物和子弹发生了碰撞
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


		//�ж��ǲ���Ѫƿ������ƿ
		else if (nodeA->getTag() <= (-10)) {
			if (nodeA->getTag()== -10){
				this->m_player->setHP(m_player->getHP() + 2);
			}
			else if (nodeA->getTag() == -20) {
				this->m_player->setMP(m_player->getMP() + 50);
				this->m_player->getCurrentWeapon()->setAttack(m_player->getCurrentWeapon()->getAttack() * 2);

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

		else if (nodeB->getTag() <= (-10)) {
			if (nodeB->getTag() == -10) {
				this->m_player->setHP(m_player->getHP() + 2);
			}
			else if (nodeB->getTag() == -20) {
				this->m_player->setMP(m_player->getMP() + 50);
				this->m_player->getCurrentWeapon()->setAttack(m_player->getCurrentWeapon()->getAttack() * 2);

			}
			nodeB->removeFromParentAndCleanup(true);
		}

	}
		return true;
}

/*void safetymap::add_player(TMXTiledMap* map)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	//锟斤拷锟斤拷一锟斤拷锟斤拷
	Sprite* player_sprite = Sprite::create("player.png");
	Player* mplayer = Player::create();
	mplayer->bind_sprite(player_sprite);
	mplayer->run();

    //锟斤拷锟斤拷锟斤拷锟斤拷锟斤�?
	mplayer->setPosition(Vec2(100, visibleSize.height / 2));

	_tiledmap->addChild(mplayer);
}*/


void safetymap::scheduleBlood(float delta)
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
		this->unschedule(CC_SCHEDULE_SELECTOR(safetymap::scheduleBlood));
	}

}

void safetymap::update(float dt)
{
	auto player_x = this->m_player->getPositionX();
	auto player_y = this->m_player->getPositionY();
	int x = player_x * 1.8 / 32;
	int y = (2560 - player_y * 1.8) / 32;

	if (x <= 9 && x >= 7 && y <= 64 && y >= 63)
	{
		//get to the next map
		Director::getInstance()->replaceScene(adventuremap::createScene());
	}
}

void safetymap::menuCloseCallback(Ref* pSender)
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
		CC_CALLBACK_1(safetymap::start_menuCloseCallback, this));
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
		CC_CALLBACK_1(safetymap::home_menuCloseCallback, this));
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
		CC_CALLBACK_1(safetymap::music_menuCloseCallback, this));
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

void safetymap::start_menuCloseCallback(Ref* pSender)
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

void safetymap::home_menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->replaceScene(HelloWorld::createScene());
}

void safetymap::music_menuCloseCallback(Ref* pSender)
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


