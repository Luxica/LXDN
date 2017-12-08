#include "Prompt.h"
#include "cocostudio/CocoStudio.h"
#include "../../../GUISystem/WWSceneManager.h"
#include "../LoginScene/QLoginScene.h"
#include "GameAudioEngine.h"
#include "WWMacros.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#include "LordNetManger.h"
#else
#include "net/LordNetManger.h"
#endif
USING_NS_CC;
using namespace cocos2d::experimental;

Prompt* Prompt::create(int type, std::string sContent)
{
	Prompt *pRet = new Prompt();
	if (pRet && pRet->init(type, sContent))
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

bool Prompt::init(int type, std::string sContent)
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	auto systemExitLayer = CSLoader::createNode("PromptLayer.csb");
	addChild(systemExitLayer);
	m_systemLayout = dynamic_cast<Layout*>(systemExitLayer->getChildByName("Panel"));
	auto trueButton = dynamic_cast<Button*>(m_systemLayout->getChildByName("Button_True"));
	auto falseButton = dynamic_cast<Button*>(m_systemLayout->getChildByName("Button_False"));
	auto viewButton = dynamic_cast<Button*>(m_systemLayout->getChildByName("Button_View"));
	auto copyButton = dynamic_cast<Button*>(m_systemLayout->getChildByName("Button_Copy"));
	auto inviteButton = dynamic_cast<Button*>(m_systemLayout->getChildByName("Button_Invite"));
	auto closeButton = dynamic_cast<Button*>(m_systemLayout->getChildByName("Button_Close"));
	if (closeButton)
	{
		closeButton->addTouchEventListener(CC_CALLBACK_2(Prompt::touchCloseEvent, this));
	}
	switch (type)	
	{
	case 1:
		if (viewButton)
		{
			viewButton->setVisible(true);
			viewButton->addTouchEventListener(CC_CALLBACK_2(Prompt::touchTrueEvent, this));
		}
		if (copyButton)
		{
			copyButton->setVisible(true);
		}
		if (inviteButton)
		{
			inviteButton->setVisible(true);
			inviteButton->addTouchEventListener(CC_CALLBACK_2(Prompt::touchTrueEvent, this));
		}
		break;
	case 2:
		if (trueButton&&falseButton)
		{
			trueButton->setVisible(true);
			//trueButton->setEnabled(true);
			falseButton->setVisible(true);
			trueButton->addTouchEventListener(CC_CALLBACK_2(Prompt::touchTrueEvent, this));
			falseButton->addTouchEventListener(CC_CALLBACK_2(Prompt::touchCloseEvent, this));
		}
		break;
	default:
		break;
	}
	
	auto messageText = dynamic_cast<Text*>(m_systemLayout->getChildByName("Text_Prompt"));
	messageText->setString(sContent);
	//messageText->setTextAreaSize(Size(100,0));

	this->setName("prompt");

	//触碰事件  (单点触碰)
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(Prompt::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Prompt::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Prompt::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);	

    return true;
}
//触碰事件  （单点触碰）
bool Prompt::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	return true;
}
void Prompt::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{

}
void Prompt::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	
}

//确认
void Prompt::touchTrueEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	this->removeFromParentAndCleanup(true);
	/*GameAudioEngine::getInstance()->stopBackgroundMusic();
	auto userDefault = UserDefault::getInstance();*/

}

//关闭
void Prompt::touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	this->removeFromParentAndCleanup(true);
}



