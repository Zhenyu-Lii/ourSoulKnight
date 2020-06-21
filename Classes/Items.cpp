#include "Items.h"

Items* Items::create(EItemsType ItemsType)
{
	Items* item = new Items;
	if (item && item->init(ItemsType))
	{
		item->autorelease();
		return item;
	}
	CC_SAFE_DELETE(item);
	return nullptr;
}

bool Items::init(EItemsType ItemsType)
{
	if (ItemsType == REDBOTTLE)
	{
		setTexture("RedBottle.png");
		_itemName = "redBottle";
		_effect = 2;

		auto physicsBody = PhysicsBody::createBox(this->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
		physicsBody->setDynamic(false);
		physicsBody->setCategoryBitmask(0x04);
		physicsBody->setContactTestBitmask(0x01);
		this->addComponent(physicsBody);

		return true;
	}
	else if (ItemsType == BLUEBOTTLE)
	{
		setTexture("BlueBottle.png");
		_itemName = "blueBottle";
		_effect = 50;

		auto physicsBody = PhysicsBody::createBox(this->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
		physicsBody->setDynamic(false);
		physicsBody->setCategoryBitmask(0x04);
		physicsBody->setContactTestBitmask(0x01);
		this->addComponent(physicsBody);

		return true;
	}
	else
		return false;
}