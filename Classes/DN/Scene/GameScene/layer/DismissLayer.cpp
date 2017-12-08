#include "DismissLayer.h"
#include "cocostudio/CocoStudio.h"
#include "../../../../GUISystem/WWSceneManager.h"
#include "../../../Comm/request/MsgGameRequest.h"
#include "../../LoginScene/QLoginScene.h"
#include "../QGameLayer.h"
#include "GameAudioEngine.h"
#include "WWMacros.h"
#include "ScriptDataManager.h"
#include "GameConfig.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#include "LordNetManger.h"
#else
#include "net/LordNetManger.h"
#endif
USING_NS_CC;
using namespace cocos2d::experimental;

DismissLayer* DismissLayer::create(int curNum)
{
	DismissLayer *pRet = new DismissLayer();
	if (pRet && pRet->init(curNum))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

bool DismissLayer::init(int curNum)
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}
	m_dismissLayout_1 = nullptr;
	m_dismissLayout_2 = nullptr;
	m_tsLayout = nullptr;
	m_failLayout = nullptr;
	listView = nullptr;
	m_tempLayout = nullptr;

	auto systemExitLayer = CSLoader::createNode("DismissLayer.csb");
	addChild(systemExitLayer);
	m_dismissLayout_1 = dynamic_cast<Layout*>(systemExitLayer->getChildByName("Panel_1"));
	if (m_dismissLayout_1)
	{
		auto confirmButton = dynamic_cast<Button*>(m_dismissLayout_1->getChildByName("Button_Confirm"));
		if (confirmButton)
		{
			confirmButton->addTouchEventListener(CC_CALLBACK_2(DismissLayer::touchTrueEvent, this));
		}
		auto cancelButton = dynamic_cast<Button*>(m_dismissLayout_1->getChildByName("Button_Cancel"));
		if (cancelButton)
		{
			cancelButton->addTouchEventListener(CC_CALLBACK_2(DismissLayer::touchFalseEvent, this));
		}
	}
	m_dismissLayout_2 = dynamic_cast<Layout*>(systemExitLayer->getChildByName("Panel_2"));
	if (m_dismissLayout_2)
	{
		auto confirmButton = dynamic_cast<Button*>(m_dismissLayout_2->getChildByName("Button_Confirm"));
		if (confirmButton)
		{
			confirmButton->addTouchEventListener(CC_CALLBACK_2(DismissLayer::touchTrueEvent, this));
		}
		auto cancelButton = dynamic_cast<Button*>(m_dismissLayout_2->getChildByName("Button_Cancel"));
		if (cancelButton)
		{
			cancelButton->addTouchEventListener(CC_CALLBACK_2(DismissLayer::touchFalseEvent, this));
		}
	}
	if (curNum == 0)
	{
		m_dismissLayout_1->setVisible(false);
		m_dismissLayout_2->setVisible(true);
	}
	else
	{
		m_dismissLayout_1->setVisible(true);
		m_dismissLayout_2->setVisible(false);
	}
	m_tsLayout = dynamic_cast<Layout*>(systemExitLayer->getChildByName("Panel_TS"));
	if (m_tsLayout)
	{
		listView = dynamic_cast<ListView*>(m_tsLayout->getChildByName("ListView_Dismiss"));
		m_tempLayout = dynamic_cast<Layout*>(m_tsLayout->getChildByName("Panel_Dismiss"));
	}
	m_failLayout = dynamic_cast<Layout*>(systemExitLayer->getChildByName("Panel_Fail"));



	//listView = dynamic_cast<ListView*>(m_systemLayout->getChildByName("ListView_Agree"));
	//m_tempLayout = dynamic_cast<Layout*>(m_systemLayout->getChildByName("Panel_Info"));
	//m_tempLayout->setVisible(false);

	//触碰事件  (单点触碰)
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(DismissLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(DismissLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(DismissLayer::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	return true;
}

//触碰事件  （单点触碰）
bool DismissLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	return true;
}
void DismissLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{

}
void DismissLayer::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{

}

void DismissLayer::UpdateAgreeState(QUserData* player[], int index)
{
	listView->removeAllItems();
	m_dismissLayout_1->setVisible(false);
	m_dismissLayout_2->setVisible(false);
	m_tsLayout->setVisible(true);
	auto panel_title = dynamic_cast<Layout*>(m_tsLayout->getChildByName("Panel_Title"));
	if (panel_title)
	{
		auto panel_name = dynamic_cast<Layout*>(panel_title->getChildByName("Panel_Name"));
		if (panel_name)
		{
			auto text_name = dynamic_cast<Text*>(panel_name->getChildByName("Text_Name"));
			if (text_name)
			{
				text_name->setString(player[index]->getNickName());
			}
		}
	}
	for (int i = 0; i < GAMECONFIG.m_max_num; i++)
	{
		if (i == index || player[i] == nullptr)
			continue;
		auto single = (Layout*)m_tempLayout->clone();
		single->setVisible(true);
		single->setTag(i);
		listView->pushBackCustomItem(single);
		auto panel_name = dynamic_cast<Layout*>(single->getChildByName("Panel_Name"));
		if (panel_name)
		{
			auto text_name = dynamic_cast<Text*>(panel_name->getChildByName("Text_Name"));
			if (text_name)
			{
				text_name->setString(player[i]->getNickName());
			}
		}
	}
	auto confirmBtn = dynamic_cast<Button*>(m_tsLayout->getChildByName("Button_Confirm"));
	if (confirmBtn)
	{
		confirmBtn->addTouchEventListener(CC_CALLBACK_2(DismissLayer::touchAgreeEvent, this));
	}
	auto cancelBtn = dynamic_cast<Button*>(m_tsLayout->getChildByName("Button_Cancel"));
	if (cancelBtn)
	{
		cancelBtn->addTouchEventListener(CC_CALLBACK_2(DismissLayer::touchRefuseEvent, this));
	}
	if (index == 0)
	{
		if (confirmBtn)	confirmBtn->setVisible(false);
		if (cancelBtn)	cancelBtn->setVisible(false);
	}
	else
	{
		if (confirmBtn)	confirmBtn->setVisible(true);
		if (cancelBtn)	cancelBtn->setVisible(true);
	}
	m_exitTime = 30.0f;
	char szCountNum[4] = { 0 };
	sprintf(szCountNum, "%02d", m_exitTime);
	auto timeText = dynamic_cast<Text*>(m_tsLayout->getChildByName("Text_Time"));
	timeText->setVisible(true);
	timeText->setString(szCountNum);
	unschedule(CC_SCHEDULE_SELECTOR(DismissLayer::resetTime));
	this->schedule(CC_SCHEDULE_SELECTOR(DismissLayer::resetTime), 1.0f);
}

void DismissLayer::UpdateDismissResult(QUserData* player[], int cPass)
{
	if (cPass == 0)
	{
		m_dismissLayout_1->setVisible(false);
		m_dismissLayout_2->setVisible(false);
		m_tsLayout->setVisible(false);
		m_failLayout->setVisible(true);
		auto confirmBtn = dynamic_cast<Button*>(m_failLayout->getChildByName("Button_Confirm"));
		if (confirmBtn)
		{
			confirmBtn->addTouchEventListener(CC_CALLBACK_2(DismissLayer::touchFalseEvent, this));
		}
	}
	else
	{
		//this->removeFromParentAndCleanup(true);
	}
}

void DismissLayer::setAgreeState(QUserData* player, int index, int agree)
{
	auto items=listView->getItems();
	for (int i = 0; i < items.size(); i++)
	{
		auto single = (Layout*)items.at(i);
		if (index == single->getTag())
		{
			auto text_State = dynamic_cast<Text*>(single->getChildByName("Text_State"));
			char dzAgree[100] = { 0 };
			sprintf(dzAgree, "AgreeMent%d", agree);
			text_State->setString(GetGameString(dzAgree));
		}
	}
}

//确认
void DismissLayer::touchTrueEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	MsgGameRequest::sendRoomCancel();
	m_dismissLayout_1->setVisible(false);
}

void DismissLayer::touchAgreeEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	MsgGameRequest::sendVoteCancel(1);
}

void DismissLayer::touchRefuseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	MsgGameRequest::sendVoteCancel(0);
}

//关闭
void DismissLayer::touchFalseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);

	if (this->isScheduled(CC_SCHEDULE_SELECTOR(DismissLayer::resetTime)))
	{
		unschedule(CC_SCHEDULE_SELECTOR(DismissLayer::resetTime));
	}
	//this->removeFromParentAndCleanup(true);
	m_parent->CloseDismissLayer();
}

void DismissLayer::resetTime(float dt)
{
	if (m_exitTime == 0)
	{
		MsgGameRequest::sendVoteCancel(1);
		auto trueButton = dynamic_cast<Button*>(m_tsLayout->getChildByName("Button_Confirm"));
		trueButton->setVisible(false);
		auto falseButton = dynamic_cast<Button*>(m_tsLayout->getChildByName("Button_Cancel"));
		falseButton->setVisible(false);
		unschedule(CC_SCHEDULE_SELECTOR(DismissLayer::resetTime));
		return;
	}
	m_exitTime--;
	char szCountNum[4] = { 0 };
	sprintf(szCountNum, "%02d", m_exitTime);
	auto timeText = dynamic_cast<Text*>(m_tsLayout->getChildByName("Text_Time"));
	timeText->setString(szCountNum);
}


