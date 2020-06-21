#include "Knight.h"


Knight::Knight()
{
	_HP = 5;
	_MP = 180;
	_AC = 5;
	_weaponPosition = Vec2(0.6, 0.25);
	_usingSkill = false;
}

Knight::~Knight() {

}
bool Knight::init()
{
	return true;
}

void Knight::skill() {
	if (_usingSkill == true) {
		for (auto weapon : _weaponBag) {
			weapon->setVisible(true);
		}
	}
	else {
		for (auto weapon : _weaponBag)
		{
			if (weapon != _currentWeapon) {
				weapon->setVisible(false);
			}
		}
	}
}