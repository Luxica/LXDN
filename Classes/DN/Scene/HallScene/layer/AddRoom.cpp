#include "AddRoom.h"
#include "cocostudio/CocoStudio.h"
#include "../../../../GUISystem/WWSceneManager.h"
#include "../../../Scene/GameScene/QGameScene.h"
#include "../../../Scene/HallScene/data/QHallManager.h"
#include "../../../../StringTool.h"
#include "../../../Comm/request/MsgHallRequest.h"
#include "../../QDNEvent.h"
#include "../../../Scene/GlobalLayer/Load.h"
#include "GameAudioEngine.h"
#include "WWMacros.h"
#include <stdlib.h>
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "iOSAdapterUtility.hpp"
#endif
#include "AdmobJni.h"
USING_NS_CC;
using namespace cocos2d::experimental;
// on "init" you need to initialize your instance
bool AddRoom::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto systemExitLayer = CSLoader::createNode("checkInLayer.csb");
	this->addChild(systemExitLayer);
	auto systemLayout = dynamic_cast<Layout*>(systemExitLayer->getChildByName("Panel"));

	auto closeButton = dynamic_cast<Button*>(systemLayout->getChildByName("Button_Close"));
	if (closeButton)
	{
		closeButton->addTouchEventListener(CC_CALLBACK_2(AddRoom::touchCloseEvent, this));
	}

	auto pasteButton = dynamic_cast<Button*>(systemLayout->getChildByName("Button_Paste"));
	if (pasteButton)
	{
		pasteButton->addTouchEventListener(CC_CALLBACK_2(AddRoom::touchPasteEvent, this));
	}
	
	auto button_1 = dynamic_cast<Button*>(systemLayout->getChildByName("Button_1"));
	if (button_1)
	{
		button_1->addTouchEventListener(CC_CALLBACK_2(AddRoom::touchKeyEvent, this));
	}	

	auto button_2 = dynamic_cast<Button*>(systemLayout->getChildByName("Button_2"));
	if (button_2)
	{
		button_2->addTouchEventListener(CC_CALLBACK_2(AddRoom::touchKeyEvent, this));
	}

	auto button_3 = dynamic_cast<Button*>(systemLayout->getChildByName("Button_3"));
	if (button_3)
	{
		button_3->addTouchEventListener(CC_CALLBACK_2(AddRoom::touchKeyEvent, this));
	}

	auto button_4 = dynamic_cast<Button*>(systemLayout->getChildByName("Button_4"));
	if (button_4)
	{
		button_4->addTouchEventListener(CC_CALLBACK_2(AddRoom::touchKeyEvent, this));
	}

	auto button_5 = dynamic_cast<Button*>(systemLayout->getChildByName("Button_5"));
	if (button_5)
	{
		button_5->addTouchEventListener(CC_CALLBACK_2(AddRoom::touchKeyEvent, this));
	}

	auto button_6 = dynamic_cast<Button*>(systemLayout->getChildByName("Button_6"));
	if (button_6)
	{
		button_6->addTouchEventListener(CC_CALLBACK_2(AddRoom::touchKeyEvent, this));
	}

	auto button_7 = dynamic_cast<Button*>(systemLayout->getChildByName("Button_7"));
	if (button_7)
	{
		button_7->addTouchEventListener(CC_CALLBACK_2(AddRoom::touchKeyEvent, this));
	}

	auto button_8 = dynamic_cast<Button*>(systemLayout->getChildByName("Button_8"));
	if (button_8)
	{
		button_8->addTouchEventListener(CC_CALLBACK_2(AddRoom::touchKeyEvent, this));
	}

	auto button_9 = dynamic_cast<Button*>(systemLayout->getChildByName("Button_9"));
	if (button_9)
	{
		button_9->addTouchEventListener(CC_CALLBACK_2(AddRoom::touchKeyEvent, this));
	}

	auto button_0 = dynamic_cast<Button*>(systemLayout->getChildByName("Button_0"));
	if (button_0)
	{
		button_0->addTouchEventListener(CC_CALLBACK_2(AddRoom::touchKeyEvent, this));
	}

	auto button_Clear = dynamic_cast<Button*>(systemLayout->getChildByName("Button_Clear"));
	if (button_Clear)
	{
		button_Clear->addTouchEventListener(CC_CALLBACK_2(AddRoom::touchKeyEvent, this));
	}

	auto button_Back = dynamic_cast<Button*>(systemLayout->getChildByName("Button_Back"));
	if (button_Back)
	{
		button_Back->addTouchEventListener(CC_CALLBACK_2(AddRoom::touchKeyEvent, this));
	}

	for (int index = 0; index < 6; index++)
	{
		char textNum[50] = {};
		sprintf(textNum, "Text_Num_%d", index);
		m_textNum[index] = dynamic_cast<Text*>(systemLayout->getChildByName(textNum));
	}
	//触碰事件  (单点触碰)
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(AddRoom::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(AddRoom::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(AddRoom::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	auto pListener = EventListenerCustom::create(EVENT_ENTEROOM_FILURE, CC_CALLBACK_1(AddRoom::onHideWaitLayer, this));
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

    return true;
}

void AddRoom::onHideWaitLayer(EventCustom *pEvent)
{
	log("111111111");
	//把等待界面去掉
	//WWSceneManager::getInstance()->closeGlobalUIByName("load");
	//WWSceneManager::getInstance()->closeGlobalUI(Load::create());
}

//触碰事件  （单点触碰）
bool AddRoom::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	return true;
}
void AddRoom::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{

}
void AddRoom::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	
}

void AddRoom::touchKeyEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	auto button_Num = (Button*)sender;

	auto iBeginNum = m_RoomNum.begin();
	auto iEndNum = m_RoomNum.end();

	auto iBeginNumShow = m_vecRoomNum.begin();
	auto iEndNumShow = m_vecRoomNum.end();

	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	if (button_Num->getTag() >= 0 && button_Num->getTag() <= 9)
	{
		if (m_RoomNum.length() == 6)
		{
			return;
		}
		m_RoomNum.append(StringTool::intToString(button_Num->getTag()));
		m_vecRoomNum.push_back(StringTool::intToString(button_Num->getTag()));
	}
	switch (button_Num->getTag())
	{
	case 10:
	{
		while (iBeginNum != iEndNum)
		{
			m_RoomNum.erase(iBeginNum);
			iBeginNum = m_RoomNum.begin();
			iEndNum = m_RoomNum.end();
		}
		if (iBeginNumShow != iEndNumShow)
		{
			m_vecRoomNum.clear();
		}
	}

		break;
	case 11:
	{
		if (iBeginNum != iEndNum)
		{
			int len = m_RoomNum.length();
			m_RoomNum.erase(iBeginNum + len - 1);
			iBeginNum = m_RoomNum.begin();
			iEndNum = m_RoomNum.end();
		}
		if (iBeginNumShow != iEndNumShow)
		{
			m_vecRoomNum.pop_back();
		}

	}
		break;
	default:
		break;
	}
	for (int index = 0; index < 6; index++)
	{
		if (index < m_vecRoomNum.size())
		{
			m_textNum[index]->setString(m_vecRoomNum.at(index));
			m_textNum[index]->setVisible(true);
		}
		else
		{
			m_textNum[index]->setVisible(false);
		}
	}

	if (m_RoomNum.length() == 6)
	{
		m_RoomNumSend = StringTool::stringToint(m_RoomNum);

		QHallManager::getInstance()->setRoomNum(m_RoomNumSend);
		QHallManager::getInstance()->setJoinType(1);
		//this->removeFromParentAndCleanup(true);
		//WWSceneManager::getInstance()->openScene(QGameScene::create());
		//deng dai
		WWSceneManager::getInstance()->openGlobalUI(Load::create());
		MsgHallRequest::sendEnterRoom(m_RoomNumSend & 0x00FFFFFF);

		return;
	}
}


//关闭
void AddRoom::touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)//按钮按下在按钮上抬起
		return;

	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	this->removeFromParentAndCleanup(true);

}

void AddRoom::touchPasteEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)//按钮按下在按钮上抬起
		return;

	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
//	string roomStr = "";
//#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//	roomStr = AdmobJni::pasteFromClipboard();
//#endif
//#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
//	roomStr = iOSAdapterUtility::getInstance()->pasteFromClipboard();
//#endif
//	
//	int roomNum = StringTool::stringToint(roomStr);
//	log("======PasteRoomNum:%d=======", roomNum);
//	if (roomNum)
//	{
//		QHallManager::getInstance()->setRoomNum(roomNum);
//		QHallManager::getInstance()->setJoinType(1);
//
//		WWSceneManager::getInstance()->openGlobalUI(Load::create());
//		MsgHallRequest::sendEnterRoom(roomNum & 0x00FFFFFF);
//	}
}


