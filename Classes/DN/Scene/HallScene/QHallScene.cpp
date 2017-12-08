#include "QHallScene.h"
#include "data/QHallManager.h"
#include "cocostudio/CocoStudio.h"
#include "../../../GUISystem/WWSceneManager.h"
#include "../../Scene/HallScene/MainMenu.h"
#include "layer/ShopLayer.h"
#include "../../Comm/request/MsgHallRequest.h"
#include "../../Scene/GlobalLayer/Load.h"
#include "../../../net/LordNetManger.h"
#include "GameConfig.h"
#include "../../Scene/GameScene/data/QGameManager.h"

USING_NS_CC;
using namespace cocostudio::timeline;

QHallScene::QHallScene()
{
	LordNetManger::getInstance()->onDisconnect(WWNetwork::eGameNet);
	setName("QHallScene");
	//加个转圈圈的
	log("Enter...QHallScene");
	//WWSceneManager::getInstance()->openGlobalUI(Load::create());
}

QHallScene::~QHallScene()
{
	log("exit....QHallScene");
}

void QHallScene::onEnter()
{
	Scene::onEnter();
	MsgHallRequest::sendAskRoom();
}

void QHallScene::onExit()
{
	Scene::onExit();
}

bool QHallScene::init()
{
	Layer *pLayer = MainMenu::create();
    pLayer->setName("menu");
	addChild(pLayer);
	if (GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL)
	{
		QHallManager::getInstance()->setPlayMode(5);
		QHallManager::getInstance()->setZMode(0);
		QHallManager::getInstance()->setCostMode(0);
	}
	return true;
}

void QHallScene::setHallRoomCard()
{
    MainMenu * menu = dynamic_cast<MainMenu*>(this->getChildByName("menu"));
    if (menu != nullptr)
    {
        menu->setHallRoomCard();
		ShopLayer* shop = dynamic_cast<ShopLayer*>(menu->getChildByName("shop"));
		if (shop)
		{
			shop->setRoomCard();
		}
    }
}

void QHallScene::setInviteShow()
{
	MainMenu * menu = dynamic_cast<MainMenu*>(this->getChildByName("menu"));
	if (menu != nullptr)
	{
		menu->setInviteShow();
	}
}

void QHallScene::showShop()
{
	MainMenu * menu = dynamic_cast<MainMenu*>(this->getChildByName("menu"));
	if (menu != nullptr)
	{
		menu->showShop();
	}
}

void QHallScene::showVIP()
{
    MainMenu * menu = dynamic_cast<MainMenu*>(this->getChildByName("menu"));
    if (menu != nullptr)
    {
        menu->showVIP();
    }
}

void QHallScene::setHint(bool isShow)
{
	MainMenu * menu = dynamic_cast<MainMenu*>(this->getChildByName("menu"));
	if (menu != nullptr)
	{
		menu->setHint(isShow);
	}
}

void QHallScene::updateOnlineNum(unsigned int num)
{
	MainMenu * menu = dynamic_cast<MainMenu*>(this->getChildByName("menu"));
	if (menu != nullptr)
	{
		menu->updateOnlineNum(num);
	}
}

