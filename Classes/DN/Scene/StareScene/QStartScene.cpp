#include "QStartScene.h"
#include "cocostudio/CocoStudio.h"

#include "../../../GUISystem/WWSceneManager.h"
#include "../../Scene/LoginScene/QLoginScene.h"

USING_NS_CC;

QStartScene::QStartScene()
{

}

QStartScene::~QStartScene()
{

}

bool QStartScene::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Scene::init());

		auto rootNode = CSLoader::createNode("StartScene.csb");
		if (rootNode)
		{
			addChild(rootNode);
		}

		auto action_dian = Sequence::create(DelayTime::create(2.0f), CallFunc::create([]() {
			WWSceneManager::getInstance()->openScene(QLoginScene::create());
		}), nullptr);
		runAction(action_dian);

		bRet = true;
	} while (0);
	return bRet;
}
