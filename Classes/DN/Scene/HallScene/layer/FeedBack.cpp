#include "FeedBack.h"
#include "cocostudio/CocoStudio.h"
#include "../../../../GUISystem/WWSceneManager.h"
#include "GameAudioEngine.h"
#include "WWMacros.h"
#include "../../../Scene/HallScene/data/QHallManager.h"
#include "../../../Comm/request/MsgHallRequest.h"
#include "../../GlobalLayer/PopLayer.h"
#include "ScriptDataManager.h"
#include "GameConfig.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#include "WWNetSprite.h"
#else
#include "GameNode/WWNetSprite.h"
#endif
USING_NS_CC;
using namespace cocos2d::experimental;
// on "init" you need to initialize your instance
bool FeedBack::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	m_CurSelect = nullptr;
	auto systemExitLayer = CSLoader::createNode("FeedbackLayer.csb");
	if (systemExitLayer)
	{
		this->addChild(systemExitLayer);
		m_fkLayer = dynamic_cast<Layout*>(systemExitLayer->getChildByName("Panel_FK"));
		if (m_fkLayer)
		{
			m_fkLayer->setVisible(false);
			m_textPhone = dynamic_cast<TextField*>(m_fkLayer->getChildByName("TextField_HM"));
			if (m_textPhone)
			{

			}
			m_textOpinion = dynamic_cast<TextField*>(m_fkLayer->getChildByName("TextField_JY"));
			if (m_textOpinion)
			{

			}
			auto submitBtn = dynamic_cast<Button*>(m_fkLayer->getChildByName("Button_Submit"));
			if (submitBtn)
			{
				submitBtn->addTouchEventListener(CC_CALLBACK_2(FeedBack::touchSubmitEvent, this));
			}
			auto closeBtn = dynamic_cast<Button*>(m_fkLayer->getChildByName("Button_Close"));
			if (closeBtn)
			{
				closeBtn->addTouchEventListener(CC_CALLBACK_2(FeedBack::touchCloseEvent, this));
			}
		}

		m_kfLayer = dynamic_cast<Layout*>(systemExitLayer->getChildByName("Panel_KF"));
		if (m_kfLayer)
		{
			m_kfLayer->setVisible(true);
			auto closeBtn = dynamic_cast<Button*>(m_kfLayer->getChildByName("Button_Close"));
			if (closeBtn)
			{
				closeBtn->addTouchEventListener(CC_CALLBACK_2(FeedBack::touchCloseEvent, this));
			}
		}
		auto checkBox_KF = dynamic_cast<CheckBox*>(systemExitLayer->getChildByName("CheckBox_KF"));
		if (checkBox_KF)
		{
			checkBox_KF->setSelected(true);
			checkBox_KF->setTouchEnabled(false);
			checkBox_KF->setTag(0);
			m_CurSelect = checkBox_KF;
			checkBox_KF->addEventListener(CC_CALLBACK_2(FeedBack::touchBtnEvent, this));
		}
		auto checkBox_FK = dynamic_cast<CheckBox*>(systemExitLayer->getChildByName("CheckBox_FK"));
		if (checkBox_FK)
		{
			checkBox_FK->setTag(1);
			checkBox_FK->addEventListener(CC_CALLBACK_2(FeedBack::touchBtnEvent, this));
		}
	}

	//触碰事件  (单点触碰)
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(FeedBack::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(FeedBack::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(FeedBack::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}
//触碰事件  （单点触碰）
bool FeedBack::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	return true;
}
void FeedBack::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{

}
void FeedBack::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	
}

void FeedBack::touchBtnEvent(Ref* pSender, CheckBox::EventType type)
{
	bool isSelect = false;
	if (type != CheckBox::EventType::SELECTED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	auto checkBox = (CheckBox*)pSender;
	if (m_CurSelect == checkBox)
		return;
	checkBox->setSelected(true);
	checkBox->setTouchEnabled(false);
	m_CurSelect->setSelected(false);
	m_CurSelect->setTouchEnabled(true);
	m_CurSelect = checkBox;
	switch (m_CurSelect->getTag())
	{
	case 0:
		m_kfLayer->setVisible(true);
		m_fkLayer->setVisible(false);
		break;
	case 1:
		m_kfLayer->setVisible(false);
		m_fkLayer->setVisible(true);
		break;
	default:
		break;
	}
}

void FeedBack::touchSubmitEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);

	std::string phone = m_textPhone->getString();
	std::string option = m_textOpinion->getString();
	if (option.length() > 0)
	{
		MsgHallRequest::sendFbkSubmit(phone.c_str(), option.c_str());
	}
	else
	{
		//弹窗
		PopLayer* popup = PopLayer::create();
		popup->setContentText(GetGameString("FeedBackFail"), 40, 800, 500);
		Director::getInstance()->getRunningScene()->addChild(popup);
	}
}

//关闭
void FeedBack::touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	this->removeFromParentAndCleanup(true);
}

void FeedBack::updateFeedBack(int errorCode)
{
	switch (errorCode)
	{
	case 0:
	{
		//弹窗
		PopLayer* popup = PopLayer::create();
		popup->setContentText(GetGameString("FeedBackFail"), 40, 800, 500);
		Director::getInstance()->getRunningScene()->addChild(popup);
	}
		break;
	case 1:
	{
		m_textPhone->setString("");
		m_textOpinion->setString("");
		m_textPhone->setPlaceHolder(GetGameString("textPhone"));
		m_textOpinion->setPlaceHolder(GetGameString("textOption"));
		//弹窗
		PopLayer* popup = PopLayer::create();
		popup->setContentText(GetGameString("FeedBackSuccess"), 40, 800, 500);
		Director::getInstance()->getRunningScene()->addChild(popup);
	}
		break;
	default:
		break;
	}
}
