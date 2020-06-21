#include"KnightStartMapScene.h"
#include"Player.h"
#include "SimpleMoveController.h"
#include"safetymapScene.h"
#include"setupScene.h"
#include"HelloWorldScene.h"
#include"audio.h"
USING_NS_CC;

Scene* KnightStartMap::createScene()
{
	return KnightStartMap::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool KnightStartMap::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}
	if (!Scene::initWithPhysics())
	{
		return false;
	}

	this->scheduleUpdate();

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	musicOff = true;
	std::string floor_layer_file = "safetymap.tmx";//地图文件
	_tiledmap = TMXTiledMap::create(floor_layer_file);
	_tiledmap->setAnchorPoint(Vec2::ZERO);
	_tiledmap->setPosition(Vec2::ZERO);

	//添加player
	Sprite* player_sprite = Sprite::create("turn right 1.png");

	Player* mplayer = Player::create();
	mplayer->bindSprite(player_sprite);
	mplayer->setTiledMap(_tiledmap);

	//加载对象层
	TMXObjectGroup* objGroup = _tiledmap->getObjectGroup("objects_knight");

	//加载玩家坐标对象
	ValueMap player_point_map = objGroup->getObject("knight");
	float playerX = player_point_map.at("x").asFloat();
	float playerY = player_point_map.at("y").asFloat();

	//设置玩家坐标
	mplayer->setPosition(Point(playerX, playerY));

	//创建玩家简单移动控制器
	SimpleMoveController* simple_move_controller = SimpleMoveController::create();

	//设置移动速度
	simple_move_controller->set_ixspeed(0);
	simple_move_controller->set_iyspeed(0);

	//将控制器添加到场景中让Upadate被调用
	this->addChild(simple_move_controller);

	//设置控制器到主角身上
	mplayer->set_controller(simple_move_controller);
	simple_move_controller->bind_sprite(player_sprite);
	simple_move_controller->bind_player(mplayer);

	auto knight_animate = Animation::create();
	char nameSize[30] = { 0 };
	for (int i = 1; i <= 2; i++)
	{
		sprintf(nameSize, "standing_right_%d_adjusted.png", i);
		knight_animate->addSpriteFrameWithFile(nameSize);
	}
	knight_animate->setDelayPerUnit(0.1f);//设置动画帧时间间隔
	knight_animate->setLoops(-1);
	knight_animate->setRestoreOriginalFrame(true);
	auto knight_animate_run = Animate::create(knight_animate);
	player_sprite->runAction(knight_animate_run);

	this->m_player = mplayer;
	_tiledmap->addChild(mplayer, 23);//10or23？


	this->addChild(_tiledmap);
	/*add the suspend button*/
	auto suspend_button = MenuItemImage::create(
		"suspend_button.png",
		"suspend_button.png",
		CC_CALLBACK_1(KnightStartMap::menuCloseCallback, this));
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

	/*add blood bar*/
	auto bloodbar = Sprite::create("bloodBar.png");
	bloodbar->setPosition(Vec2(origin.x + 55, visibleSize.height-5));
	this->addChild(bloodbar, 2);
	
	return true;
}

void KnightStartMap::menuCloseCallback(Ref* pSender)
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
		CC_CALLBACK_1(KnightStartMap::start_menuCloseCallback, this));
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
		CC_CALLBACK_1(KnightStartMap::home_menuCloseCallback, this));
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
		CC_CALLBACK_1(KnightStartMap::music_menuCloseCallback, this));
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

void KnightStartMap::update(float dt)
{

	auto player_x = this->m_player->getPositionX();
	auto player_y = this->m_player->getPositionY();
	int x = player_x * 1.8 / 32;
	int y = (1920 - player_y * 1.8) / 32;
	if (x <= 21 && x >= 18 && (y == 7))
	{

		Director::getInstance()->replaceScene(safetymap::createScene());
	}

	//加载玩家坐标对象
	_tiledmap->getLayer("weapon_information")->setVisible(false);
	if (x <= 15 && x >= 14 && y == 50)
	{
		_tiledmap->getLayer("weapon_information")->setVisible(true);
	}

	_tiledmap->getLayer("weapon_information")->setVisible(false);
	if (x <= 15 && x >= 14 && y == 50)
	{
		_tiledmap->getLayer("weapon_information")->setVisible(true);
	}
}

void KnightStartMap::start_menuCloseCallback(Ref* pSender)
{
	auto suspend_moveby = MoveBy::create(0.3f, Vec2(0, 300));
	suspend_scene->runAction(suspend_moveby);

	auto home_moveby = MoveBy::create(0.3f, Vec2(0, 300));
	home_button->runAction(home_moveby);

	auto start_moveby= MoveBy::create(0.3f, Vec2(0, 300));
	suspend_start->runAction(start_moveby);


	auto music_moveby = MoveBy::create(0.3f, Vec2(0, 300));
	music_button->runAction(music_moveby);

}

void KnightStartMap::home_menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->replaceScene(HelloWorld::createScene());
}

void KnightStartMap::music_menuCloseCallback(Ref* pSender)
{
	if (musicOff == true)
	{
		audio_begin->pauseBackgroundMusic();
		musicOff = false;
	}
	else
	{
		audio_begin->resumeBackgroundMusic();
		musicOff = true;
	}
}