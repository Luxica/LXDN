#include "LoginLoad.h"
#include "cocostudio/CocoStudio.h"
#include "../../../../GUISystem/WWSceneManager.h"
#include "../../../Scene/HallScene/QHallScene.h"
#include "../../../../net/LordNetManger.h"

USING_NS_CC;

using namespace cocostudio::timeline;

LoginLoad::LoginLoad()
{
	//log("Enter...QLoginScene");
}

LoginLoad::~LoginLoad()
{
	//log("exit....QLoginScene");
}

bool LoginLoad::init()
{

	auto rootNode = CSLoader::createNode("LoginLoadLayer.csb");
	if (rootNode)
	{
		addChild(rootNode);
		auto circle = dynamic_cast<Sprite*>(rootNode->getChildByName("Sprite_Loading"));

		auto animation = Animation::create();
		for (int i = 1; i < 12; i++)
		{
			char szName[100] = { 0 };
			sprintf(szName, "LoginLoad/1-%d.png", i);
			animation->addSpriteFrameWithFile(szName);
		}
		// 控制帧动画的每帧间的播放间隔
		animation->setDelayPerUnit(0.1f);
		// 是否在播放后重置成第一张图片
		animation->setRestoreOriginalFrame(false);
		auto action_Load = Animate::create(animation);

		circle->runAction(RepeatForever::create(action_Load));
	}
	
	this->setName("loginload");

	return true;
}
void LoginLoad::stopCallBack()
{
	WWSceneManager::getInstance()->openScene(QHallScene::create());
}



