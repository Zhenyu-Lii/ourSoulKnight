#include "SimpleMoveController.h"
#include <map>
#include "safetymapScene.h"

bool SimpleMoveController::init()
{
	this->m_ixspeed = 0;
	this->m_iyspeed = 0;


	registeKeyBoardEvent();//ע����̿����¼�
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
	//���̿���ʵ��
	//�����ƶ������X����ֵ
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

//���̿���ʵ��

/*α����
*/
void SimpleMoveController::registeKeyBoardEvent()
{
	auto keyBoardListener = EventListenerKeyboard::create();
	keyBoardListener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event* event) {
		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_J://�����
		{
			if (m_player->getLockedTarget() == NULL ||
				m_player->getLockedTarget()->getalreadyDead()) {

				//�½�һ��target������ָ������Ļ��soldier
				RemoteSoldier* target = NULL;

				//����soldiermanager,�ҳ�����Ļ��soldier
				for (auto soldier : m_scene->m_remoteSoldierManager->getSoldierArr()) {
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
					m_player->attack(m_scene, pos);
				}

				//���û�ҵ�������Ŀ�꣬����ǰ������
				else {
					m_player->resetWeaponPos();
					m_player->attack(m_scene, Vec2(m_player->getPositionX() + 1, m_player->getPositionY()));
				}
			}

			//��������ķ��棬����������Ŀ���Ҹ�Ŀ���ǻ��ŵ�
			else {
				//ֱ�ӹ�����Ŀ��
				Vec2 pos = m_player->getLockedTarget()->getPosition();
				m_player->rotateWeapon(pos);
				m_player->attack(m_scene, pos);
			}
			break;
		}
		case EventKeyboard::KeyCode::KEY_K://�����л���
		{
			m_player->switchWeapon();
			break;
		}
		case EventKeyboard::KeyCode::KEY_W://�Ϸ����;
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
				knight_animate->setDelayPerUnit(0.08f);//���ö���֡ʱ����
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
				knight_animate->setDelayPerUnit(0.08f);//���ö���֡ʱ����
				knight_animate->setLoops(-1);
				knight_animate->setRestoreOriginalFrame(true);
				knight_animate_run = Animate::create(knight_animate);
				m_player->getSprite()->runAction(knight_animate_run);
			}
			set_iyspeed(2);


			break;
		}
		case EventKeyboard::KeyCode::KEY_A://�����;
		{
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
			knight_animate->setDelayPerUnit(0.08f);//���ö���֡ʱ����
			knight_animate->setLoops(-1);
			knight_animate->setRestoreOriginalFrame(true);
			knight_animate_run = Animate::create(knight_animate);
			m_player->getSprite()->runAction(knight_animate_run);

			left_right_tag = -1;
			break;

		}

		case EventKeyboard::KeyCode::KEY_S://�·����;
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
				knight_animate->setDelayPerUnit(0.08f);//���ö���֡ʱ����
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
				knight_animate->setDelayPerUnit(0.08f);//���ö���֡ʱ����
				knight_animate->setLoops(-1);
				knight_animate->setRestoreOriginalFrame(true);
				knight_animate_run = Animate::create(knight_animate);
				m_player->getSprite()->runAction(knight_animate_run);
			}

			set_iyspeed(-2);
			break;
		}
		case EventKeyboard::KeyCode::KEY_D://�ҷ����;
		{
			key_d = true;
			m_player->getSprite()->stopAllActions();
			knight_animate = Animation::create();
			char nameSize[30] = { 0 };
			for (int i = 1; i <= 4; i++)
			{
				sprintf(nameSize, "running_right_%d.png", i);
				knight_animate->addSpriteFrameWithFile(nameSize);
			}
			knight_animate->setDelayPerUnit(0.08f);//���ö���֡ʱ����
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
					knight_animate->setDelayPerUnit(0.1f);//���ö���֡ʱ����
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
					knight_animate->setDelayPerUnit(0.1f);//���ö���֡ʱ����
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
		case EventKeyboard::KeyCode::KEY_A://�����;
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
				knight_animate->setDelayPerUnit(0.1f);//���ö���֡ʱ����
				knight_animate->setLoops(-1);
				knight_animate->setRestoreOriginalFrame(true);
				knight_animate_run = Animate::create(knight_animate);
				m_player->getSprite()->runAction(knight_animate_run);
			}
			key_a = false;
			set_ixspeed(0);
			break;
		}
		case EventKeyboard::KeyCode::KEY_S://�·����;
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
					knight_animate->setDelayPerUnit(0.1f);//���ö���֡ʱ����
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
					knight_animate->setDelayPerUnit(0.1f);//���ö���֡ʱ����
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

		case EventKeyboard::KeyCode::KEY_D://�ҷ����;
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
				knight_animate->setDelayPerUnit(0.1f);//���ö���֡ʱ����
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

void SimpleMoveController::bind_scene(safetymap* scene)
{
	m_scene = scene;
}

