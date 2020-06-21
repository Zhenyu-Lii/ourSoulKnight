#include "SimpleMoveController.h"
#include <map>
#include "adventuremapScene.h"
#include "myScene.h"

bool SimpleMoveController::init()
{
	this->m_ixspeed = 0;
	this->m_iyspeed = 0;

	
	registeKeyBoardEvent();//注册键盘控制事件
	this->scheduleUpdate();

	key_w = false;
	key_a = false;
	key_s = false;
	key_d = false;

	return true;
}

void SimpleMoveController::update(float dt)
{
	if (m_controllerListener == NULL)
		return;
	//键盘控制实现
	//增加移动对象的X坐标值
	Point pos = m_controllerListener->get_tag_position();
	pos.x += m_ixspeed;
	pos.y += m_iyspeed;
	m_controllerListener->set_tag_position(pos.x, pos.y);

}

void SimpleMoveController::set_speed(int ispeed)
{
	this->m_ixspeed = ispeed;
}

void SimpleMoveController::set_ixspeed(int ispeed)
{
	this->m_ixspeed = ispeed;
}

void SimpleMoveController::set_iyspeed(int ispeed)
{
	this->m_iyspeed = ispeed;
}

//键盘控制实现

/*伪代码
*/
void SimpleMoveController::registeKeyBoardEvent()
{
	auto keyBoardListener = EventListenerKeyboard::create();
	keyBoardListener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event* event) {
		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_P://技能键
		{
			m_player->setHP(5);
			break;
		}

		case EventKeyboard::KeyCode::KEY_O://技能键
		{
			if (m_player->getCurrentWeapon() != NULL) {
				m_player->getCurrentWeapon()->setAttack(10 * m_player->getCurrentWeapon()->getAttack());
				break;
			}
		}

		case EventKeyboard::KeyCode::KEY_L://技能键
		{
			m_player->activateSkill();
			m_player->skill();
			break;
		}

		case EventKeyboard::KeyCode::KEY_J://开火键
		{
			if (m_scene != nullptr) {
				if (m_player->getLockedTarget() == NULL ||
					m_player->getLockedTarget()->getalreadyDead()) {

					//新建一个target，用于指向最近的活的soldier
					RemoteSoldier* target = NULL;

					//遍历soldiermanager,找出最近的活的soldier
					for (auto soldier : m_scene->m_remoteSoldierManager->getSoldierArr()) {
						//如果是死的，跳过
						if (soldier->getalreadyDead()) {
							continue;
						}

						//这个soldier没死的话，计算出soldier和player的距离
						Vec2 direction = soldier->getPosition() - m_player->getPosition();
						float distance = sqrt(direction.x*direction.x + direction.y*direction.y);
						static float minDistance = distance;

						//如果当前soldier距离是最近的，那么把target设置为这个soldier
						if (minDistance >= distance) {
							target = soldier;
						}

						//如果都死完了，那么target不会变，一直是NULL
					}

					//如果找到了锁定目标，那么锁定并攻击
					if (target != NULL) {
						m_player->setLockedTarget(target);
						Vec2 pos = m_player->getLockedTarget()->getPosition();
						m_player->attack(m_scene, pos);
					}

					//如果没找到锁定的目标，就向前方开火
					else {
						m_player->resetWeaponPos();
						if (m_player->getCurrentWeapon()->isFlippedX() == false) {
							m_player->attack(m_scene, Vec2(m_player->getPositionX() + 1, m_player->getPositionY()));
						}
						else {
							m_player->attack(m_scene, Vec2(m_player->getPositionX() - 1, m_player->getPositionY()));

						}
					}
				}

				//上述情况的反面，就是有锁定目标且该目标是活着的
				else {
					//直接攻击该目标
					Vec2 pos = m_player->getLockedTarget()->getPosition();
					m_player->attack(m_scene, pos);
				}
			}
			break;
		}
		case EventKeyboard::KeyCode::KEY_K://武器切换键
		{
			m_player->switchWeapon();
			break;
		}
		case EventKeyboard::KeyCode::KEY_W://上方向键;
		{
			key_w = true;
			m_player->getSprite()->stopAllActions();
			if (left_right_tag == -1)
			{
				knight_animate = Animation::create();
				char nameSize[30] = { 0 };
				for (int i = 1; i <= 4; i++)
				{
					sprintf(nameSize, "running_left_%d.png", i);
					knight_animate->addSpriteFrameWithFile(nameSize);
				}
				knight_animate->setDelayPerUnit(0.08f);//设置动画帧时间间隔
				knight_animate->setLoops(-1);
				knight_animate->setRestoreOriginalFrame(true);
				knight_animate_run = Animate::create(knight_animate);
				m_player->getSprite()->runAction(knight_animate_run);
			}
			if (left_right_tag == 1)
			{
				knight_animate = Animation::create();
				char nameSize[30] = { 0 };
				for (int i = 1; i <= 4; i++)
				{
					sprintf(nameSize, "running_right_%d.png", i);
					knight_animate->addSpriteFrameWithFile(nameSize);
				}
				knight_animate->setDelayPerUnit(0.08f);//设置动画帧时间间隔
				knight_animate->setLoops(-1);
				knight_animate->setRestoreOriginalFrame(true);
				knight_animate_run = Animate::create(knight_animate);
				m_player->getSprite()->runAction(knight_animate_run);
			}
			set_iyspeed(2);


			break;
		}
		case EventKeyboard::KeyCode::KEY_A://左方向键;
		{
			//人物面向左边时，翻转武器
			auto weaponBag = m_player->getWeaponBag();
			for (auto weapon : weaponBag) {
				weapon->setFlippedX(true);
				weapon->setRotation(0.0f);
				Size size = m_player->getSprite()->getContentSize();
				float coefX = 1 - m_player->getWpPos().x;
				float coefY = m_player->getWpPos().y;
				weapon->setPosition(Vec2(0, size.height*coefY));
			}

			set_ixspeed(-2);
			key_a = true;
			m_player->getSprite()->stopAllActions();
			knight_animate = Animation::create();
			char nameSize[30] = { 0 };
			for (int i = 1; i <= 4; i++)
			{
				sprintf(nameSize, "running_left_%d.png", i);
				knight_animate->addSpriteFrameWithFile(nameSize);
			}
			knight_animate->setDelayPerUnit(0.08f);//设置动画帧时间间隔
			knight_animate->setLoops(-1);
			knight_animate->setRestoreOriginalFrame(true);
			knight_animate_run = Animate::create(knight_animate);
			m_player->getSprite()->runAction(knight_animate_run);

			left_right_tag = -1;
			break;

		}

		case EventKeyboard::KeyCode::KEY_S://下方向键;
		{
			key_s = true;
			m_player->getSprite()->stopAllActions();
			if (left_right_tag == -1)
			{
				knight_animate = Animation::create();
				char nameSize[30] = { 0 };
				for (int i = 1; i <= 4; i++)
				{
					sprintf(nameSize, "running_left_%d.png", i);
					knight_animate->addSpriteFrameWithFile(nameSize);
				}
				knight_animate->setDelayPerUnit(0.08f);//设置动画帧时间间隔
				knight_animate->setLoops(-1);
				knight_animate->setRestoreOriginalFrame(true);
				knight_animate_run = Animate::create(knight_animate);
				m_player->getSprite()->runAction(knight_animate_run);
			}
			if (left_right_tag == 1)
			{
				knight_animate = Animation::create();
				char nameSize[30] = { 0 };
				for (int i = 1; i <= 4; i++)
				{
					sprintf(nameSize, "running_right_%d.png", i);
					knight_animate->addSpriteFrameWithFile(nameSize);
				}
				knight_animate->setDelayPerUnit(0.08f);//设置动画帧时间间隔
				knight_animate->setLoops(-1);
				knight_animate->setRestoreOriginalFrame(true);
				knight_animate_run = Animate::create(knight_animate);
				m_player->getSprite()->runAction(knight_animate_run);
			}

			set_iyspeed(-2);
			break;
		}
		case EventKeyboard::KeyCode::KEY_D://右方向键;
		{
			//人物面向右边时，武器不翻转
			auto weaponBag = m_player->getWeaponBag();
			for (auto weapon : weaponBag) {
				weapon->setFlippedX(false);
				weapon->setRotation(0.0f);

				Size size = m_player->getSprite()->getContentSize();
				float coefX = m_player->getWpPos().x;
				float coefY = m_player->getWpPos().y;
				weapon->setPosition(Vec2(size.width*coefX, size.height*coefY));

			}

			key_d = true;
			m_player->getSprite()->stopAllActions();
			knight_animate = Animation::create();
			char nameSize[30] = { 0 };
			for (int i = 1; i <= 4; i++)
			{
				sprintf(nameSize, "running_right_%d.png", i);
				knight_animate->addSpriteFrameWithFile(nameSize);
			}
			knight_animate->setDelayPerUnit(0.08f);//设置动画帧时间间隔
			knight_animate->setLoops(-1);
			knight_animate->setRestoreOriginalFrame(true);
			knight_animate_run = Animate::create(knight_animate);
			m_player->getSprite()->runAction(knight_animate_run);

			left_right_tag = 1;
			set_ixspeed(2);
			break;
		}

		}
	};

	keyBoardListener->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event* event) {
		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_W:
		{
			if (key_a == false && key_s == false && key_d == false)
			{
				m_player->getSprite()->stopAllActions();
				if (left_right_tag == -1)
				{
					knight_animate = Animation::create();
					char nameSize[30] = { 0 };
					for (int i = 1; i <= 2; i++)
					{
						sprintf(nameSize, "standing_left_%d.png", i);
						knight_animate->addSpriteFrameWithFile(nameSize);
					}
					knight_animate->setDelayPerUnit(0.1f);//设置动画帧时间间隔
					knight_animate->setLoops(-1);
					knight_animate->setRestoreOriginalFrame(true);
					knight_animate_run = Animate::create(knight_animate);
					m_player->getSprite()->runAction(knight_animate_run);
				}
				if (left_right_tag == 1)
				{
					knight_animate = Animation::create();
					char nameSize[30] = { 0 };
					for (int i = 1; i <= 2; i++)
					{
						sprintf(nameSize, "standing_right_%d_adjusted.png", i);
						knight_animate->addSpriteFrameWithFile(nameSize);
					}
					knight_animate->setDelayPerUnit(0.1f);//设置动画帧时间间隔
					knight_animate->setLoops(-1);
					knight_animate->setRestoreOriginalFrame(true);
					knight_animate_run = Animate::create(knight_animate);
					m_player->getSprite()->runAction(knight_animate_run);
				}
			}
			key_w = false;
			set_iyspeed(0);
			break;
		}
		case EventKeyboard::KeyCode::KEY_A://左方向键;
		{
			if (key_w == false && key_s == false && key_d == false)
			{
				m_player->getSprite()->stopAllActions();
				knight_animate = Animation::create();
				char nameSize[30] = { 0 };
				for (int i = 1; i <= 2; i++)
				{
					sprintf(nameSize, "standing_left_%d.png", i);
					knight_animate->addSpriteFrameWithFile(nameSize);
				}
				knight_animate->setDelayPerUnit(0.1f);//设置动画帧时间间隔
				knight_animate->setLoops(-1);
				knight_animate->setRestoreOriginalFrame(true);
				knight_animate_run = Animate::create(knight_animate);
				m_player->getSprite()->runAction(knight_animate_run);
			}
			key_a = false;
			set_ixspeed(0);
			break;
		}
		case EventKeyboard::KeyCode::KEY_S://下方向键;
		{
			if (key_w == false && key_a == false && key_d == false)
			{
				m_player->getSprite()->stopAllActions();
				if (left_right_tag == -1)
				{
					knight_animate = Animation::create();
					char nameSize[30] = { 0 };
					for (int i = 1; i <= 2; i++)
					{
						sprintf(nameSize, "standing_left_%d.png", i);
						knight_animate->addSpriteFrameWithFile(nameSize);
					}
					knight_animate->setDelayPerUnit(0.1f);//设置动画帧时间间隔
					knight_animate->setLoops(-1);
					knight_animate->setRestoreOriginalFrame(true);
					knight_animate_run = Animate::create(knight_animate);
					m_player->getSprite()->runAction(knight_animate_run);
				}
				if (left_right_tag == 1)
				{
					knight_animate = Animation::create();
					char nameSize[30] = { 0 };
					for (int i = 1; i <= 2; i++)
					{
						sprintf(nameSize, "standing_right_%d_adjusted.png", i);
						knight_animate->addSpriteFrameWithFile(nameSize);
					}
					knight_animate->setDelayPerUnit(0.1f);//设置动画帧时间间隔
					knight_animate->setLoops(-1);
					knight_animate->setRestoreOriginalFrame(true);
					knight_animate_run = Animate::create(knight_animate);
					m_player->getSprite()->runAction(knight_animate_run);
				}
			}
			key_s = false;
			set_iyspeed(0);
			break;
		}

		case EventKeyboard::KeyCode::KEY_D://右方向键;
		{
			if (key_w == false && key_a == false && key_s == false)
			{
				m_player->getSprite()->stopAllActions();
				knight_animate = Animation::create();
				char nameSize[30] = { 0 };
				for (int i = 1; i <= 2; i++)
				{
					sprintf(nameSize, "standing_right_%d_adjusted.png", i);
					knight_animate->addSpriteFrameWithFile(nameSize);
				}
				knight_animate->setDelayPerUnit(0.1f);//设置动画帧时间间隔
				knight_animate->setLoops(-1);
				knight_animate->setRestoreOriginalFrame(true);
				knight_animate_run = Animate::create(knight_animate);
				m_player->getSprite()->runAction(knight_animate_run);
			}
			key_d = false;
			set_ixspeed(0);
			break;
		}
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyBoardListener, this);



}

void SimpleMoveController::bind_sprite(Sprite* sprite)
{
	m_sprite = sprite;
}

void SimpleMoveController::bind_player(Player* player)
{
	m_player = player;
}

void SimpleMoveController::bind_scene(myScene* scene)
{
	m_scene = scene;
}

/*void SimpleMoveController::bind_scene(adventuremap* scene)
{
	m_scene = scene;
}

/*void SimpleMoveController::bind_scene(safetymap* scene) {
	m_scene = adventuremap::create(scene);
}*/


