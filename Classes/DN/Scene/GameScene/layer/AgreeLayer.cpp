#include "AgreeLayer.h"
#include "cocostudio/CocoStudio.h"
#include "../../../../GUISystem/WWSceneManager.h"
#include "../../../Comm/request/MsgGameRequest.h"
#include "../../LoginScene/QLoginScene.h"
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

AgreeLayer* AgreeLayer::create(std::string sContent)
{
	AgreeLayer *pRet = new AgreeLayer();
	if (pRet && pRet->init(sContent))
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

bool AgreeLayer::init(std::string sContent)
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	auto systemExitLayer = CSLoader::createNode("System_Exit/AgreeMentLayer.csb");
	addChild(systemExitLayer);
	m_systemLayout = dynamic_cast<Layout*>(systemExitLayer->getChildByName("Panel"));
	auto trueButton = dynamic_cast<Button*>(m_systemLayout->getChildByName("Button_Ture"));
	auto falseButton = dynamic_cast<Button*>(m_systemLayout->getChildByName("Button_False"));

	trueButton->addTouchEventListener(CC_CALLBACK_2(AgreeLayer::touchTrueEvent, this));
	falseButton->addTouchEventListener(CC_CALLBACK_2(AgreeLayer::touchFalseEvent, this));


	auto messageText = dynamic_cast<Text*>(m_systemLayout->getChildByName("Text"));
	messageText->setString(sContent);

	listView = dynamic_cast<ListView*>(m_systemLayout->getChildByName("ListView_Agree"));
	m_tempLayout = dynamic_cast<Layout*>(m_systemLayout->getChildByName("Panel_Info"));
	m_tempLayout->setVisible(false);

	m_exitTime = 30.0f;

	char szCountNum[4] = { 0 };
	sprintf(szCountNum, "%02d", m_exitTime);
	auto timeText = dynamic_cast<Text*>(m_systemLayout->getChildByName("Text_Time"));
	timeText->setVisible(true);
	timeText->setString(szCountNum);
	unschedule(CC_SCHEDULE_SELECTOR(AgreeLayer::resetTime));
	this->schedule(CC_SCHEDULE_SELECTOR(AgreeLayer::resetTime), 1.0f);


	return true;
}

void AgreeLayer::UpdateAgreeState(QUserData* player[], int index)
{
	for (int i = 0; i < GAMECONFIG.m_max_num; i++)
	{
		if (i == index || player[i] == nullptr)
			continue;
		auto single = (Layout*)m_tempLayout->clone();
		single->setVisible(true);
		single->setTag(i);
		listView->pushBackCustomItem(single);
		auto m_Text = dynamic_cast<Text*>(single->getChildByName("Text_Info"));
		char dzAgree[100] = { 0 };
		sprintf(dzAgree, GetGameString("AgreeMent2"), player[i]->getNickName().c_str());
		m_Text->setString(dzAgree);
	}
	if (index==0)
	{
		auto trueButton = dynamic_cast<Button*>(m_systemLayout->getChildByName("Button_Ture"));
		trueButton->setVisible(false);
		auto falseButton = dynamic_cast<Button*>(m_systemLayout->getChildByName("Button_False"));
		falseButton->setVisible(false);
	}
}

void AgreeLayer::setAgreeState(QUserData* player, int index, int agree)
{
	auto items=listView->getItems();
	for (int i = 0; i < items.size(); i++)
	{
		auto single = (Layout*)items.at(i);
		if (index == single->getTag())
		{
			auto m_Text = dynamic_cast<Text*>(single->getChildByName("Text_Info"));
			char dzAgree[100] = { 0 };
			if (agree==0)
				sprintf(dzAgree, GetGameString("AgreeMent0"), player->getNickName().c_str());
			else
				sprintf(dzAgree, GetGameString("AgreeMent1"), player->getNickName().c_str());

			m_Text->setString(dzAgree);
		}
	}
}

//确认
void AgreeLayer::touchTrueEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	MsgGameRequest::sendVoteCancel(1);
	auto trueButton = dynamic_cast<Button*>(m_systemLayout->getChildByName("Button_Ture"));
	trueButton->setVisible(false);
	auto falseButton = dynamic_cast<Button*>(m_systemLayout->getChildByName("Button_False"));
	falseButton->setVisible(false);
	unschedule(CC_SCHEDULE_SELECTOR(AgreeLayer::resetTime));
}
//关闭
void AgreeLayer::touchFalseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	MsgGameRequest::sendVoteCancel(0);
	auto trueButton = dynamic_cast<Button*>(m_systemLayout->getChildByName("Button_Ture"));
	trueButton->setVisible(false);
	auto falseButton = dynamic_cast<Button*>(m_systemLayout->getChildByName("Button_False"));
	falseButton->setVisible(false);
	unschedule(CC_SCHEDULE_SELECTOR(AgreeLayer::resetTime));
}

void AgreeLayer::resetTime(float dt)
{
	if (m_exitTime == 0)
	{
		MsgGameRequest::sendVoteCancel(1);
		auto trueButton = dynamic_cast<Button*>(m_systemLayout->getChildByName("Button_Ture"));
		trueButton->setVisible(false);
		auto falseButton = dynamic_cast<Button*>(m_systemLayout->getChildByName("Button_False"));
		falseButton->setVisible(false);
		unschedule(CC_SCHEDULE_SELECTOR(AgreeLayer::resetTime));
		return;
	}
	m_exitTime--;
	char szCountNum[4] = { 0 };
	sprintf(szCountNum, "%02d", m_exitTime);
	auto timeText = dynamic_cast<Text*>(m_systemLayout->getChildByName("Text_Time"));
	timeText->setString(szCountNum);
}


