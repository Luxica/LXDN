#include "PlayIntroLayer.h"
#include "cocostudio/CocoStudio.h"
#include "../../../../GUISystem/WWSceneManager.h"
#include "../../../../StringTool.h"
#include "GameAudioEngine.h"
#include "WWMacros.h"

USING_NS_CC;
using namespace cocos2d::experimental;
// on "init" you need to initialize your instance
bool PlayIntroLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	auto systemExitLayer = CSLoader::createNode("PlayWayLayer.csb");
	if (systemExitLayer)
	{ 
		this->addChild(systemExitLayer);
	}

	auto systemLayout = dynamic_cast<Layout*>(systemExitLayer->getChildByName("Panel"));
	if (systemLayout)
	{
		auto summaryButton = dynamic_cast<Button*>(systemLayout->getChildByName("Button_Summary"));
		if (summaryButton)
		{
			summaryButton->setTag(6);
			summaryButton->addTouchEventListener(CC_CALLBACK_2(PlayIntroLayer::touchPlayEvent, this));
			m_playType = summaryButton;
		}
		auto playTypeButton = dynamic_cast<Button*>(systemLayout->getChildByName("Button_PlayType"));
		if (playTypeButton)
		{
			playTypeButton->setEnabled(false);
			playTypeButton->setTag(7);
			playTypeButton->addTouchEventListener(CC_CALLBACK_2(PlayIntroLayer::touchPlayEvent, this));
			m_playType = playTypeButton;
		}

		auto closeButton = dynamic_cast<Button*>(systemLayout->getChildByName("Button_Close"));
		if (closeButton)
		{
			closeButton->addTouchEventListener(CC_CALLBACK_2(PlayIntroLayer::touchCloseEvent, this));
		}

		listView_Intro = dynamic_cast<ListView*>(systemLayout->getChildByName("ListView_Introduct"));
		if (listView_Intro)
		{
			listView_Intro->setVisible(false);
		}
		listView_Type = dynamic_cast<ListView*>(systemLayout->getChildByName("ListView_playType"));
		if (listView_Type)
		{
			listView_Type->setVisible(true);
		}
	
	}
	//触碰事件  (单点触碰)
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(PlayIntroLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(PlayIntroLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(PlayIntroLayer::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}
//触碰事件  （单点触碰）
bool PlayIntroLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	return true;
}
void PlayIntroLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{

}
void PlayIntroLayer::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	
}

//关闭
void PlayIntroLayer::touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	this->removeFromParentAndCleanup(true);

}

void PlayIntroLayer::touchPlayEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	auto buttonCur = (Button*)sender;
	if (m_playType == buttonCur)
		return;
	buttonCur->setEnabled(false);
	m_playType->setEnabled(true);
	m_playType = buttonCur;
	if (m_playType->getTag() == 6)
	{
		listView_Intro->setVisible(true);
		listView_Type->setVisible(false);
	}
	if (m_playType->getTag() == 7)
	{
		listView_Intro->setVisible(false);
		listView_Type->setVisible(true);
	}
}

