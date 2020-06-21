#ifndef _AX_H_
#define _Ax_H_

#include "Sword.h"
class Ax : public Sword {
	CC_SYNTHESIZE(EActorType, _bulletType, BulletType);			//����ʹ�õ��ӵ�����,��ս���������ӵ�

public:
	Ax();										//��ʼ����ʱ�������ײģ��
	~Ax();

	static Ax* create(const std::string& filename);
	virtual bool init(const std::string& filename);
	virtual void fire(Scene* _currentScene, const Vec2& pos, Entity* player);

private:
	Animate* attackAnimate;							//��ս������������

};

#endif // !_Sword_H_

#pragma once
#pragma once
