#include "Circular.h"
#include "cocostudio/CocoStudio.h"
#include "../../../../GUISystem/WWSceneManager.h"
#include "GameAudioEngine.h"
#include "WWMacros.h"
#include "../../../Scene/HallScene/data/QHallManager.h"
#include "GameConfig.h"
#include "../../../../ScriptDataManager.h"
#include "../../GlobalLayer/PopLayer.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#include "WWNetSprite.h"
#include "iOSAdapterUtility.hpp"
#else
#include "GameNode/WWNetSprite.h"
#include "AdmobJni.h"
#endif
USING_NS_CC;
using namespace cocos2d::experimental;
// on "init" you need to initialize your instance
bool Circular::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	auto systemExitLayer = CSLoader::createNode("circularLayer.csb");
	if (systemExitLayer)
	{
		this->addChild(systemExitLayer);

		auto systemLayout = dynamic_cast<Layout*>(systemExitLayer->getChildByName("Panel"));
		if (systemLayout)
		{
			auto closeButton = dynamic_cast<Button*>(systemLayout->getChildByName("Button_Close"));
			if (closeButton)
			{
				closeButton->addTouchEventListener(CC_CALLBACK_2(Circular::touchCloseEvent, this));
			}

			auto m_listCircular = dynamic_cast<ListView*>(systemLayout->getChildByName("ListView_Circular"));
			if (m_listCircular)
			{
				auto panel_circular = dynamic_cast<Layout*>(m_listCircular->getChildByName("Panel_Circular"));
				if (panel_circular)
				{
					panel_circular->removeAllChildren();
					std::string path = "Circular/tz.png";
					WWNetSprite* pPortrait = nullptr;
					pPortrait->setLocalZOrder(-1);
					pPortrait->setAnchorPoint(Vec2(0.5f, 0.5f));
					panel_circular->addChild(pPortrait);
					pPortrait->setPosition(panel_circular->getContentSize().width / 2, panel_circular->getContentSize().height / 2);
				}
			}
			auto text_wx = dynamic_cast<Text*>(systemLayout->getChildByName("Text_WX"));
			if (text_wx)
			{
				text_wx->setString(GetGameString("Text_WX"));
				text_wx->setTag(1);
				text_wx->addTouchEventListener(CC_CALLBACK_2(Circular::touchCopyEvent, this));
			}
			auto text_gz = dynamic_cast<Text*>(systemLayout->getChildByName("Text_GZ"));
			if (text_gz)
			{
				text_gz->setString(GetGameString("Text_GZ"));
				text_gz->setTag(2);
				text_gz->addTouchEventListener(CC_CALLBACK_2(Circular::touchCopyEvent, this));
			}
        }
    }

	//触碰事件  (单点触碰)
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(Circular::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Circular::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Circular::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}
//触碰事件  （单点触碰）
bool Circular::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	return true;
}
void Circular::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{

}
void Circular::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	
}
void Circular::touchCopyEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	auto text = (Text*)sender;
	if (type == Widget::TouchEventType::BEGAN)
	{
		text->setColor(Color3B::RED);
	}
	if (type == Widget::TouchEventType::ENDED)
	{
		text->setColor(Color3B::YELLOW);
		char str[32] = { 0 };
		switch (text->getTag())
		{
		case 1:
			sprintf(str, "%s", GetGameString("Text_WX"));
			break;
		case 2:
			sprintf(str, "%s", GetGameString("Text_GZ"));
			break;
		default:
			break;
		}
		int code = 2;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		code=AdmobJni::copyToClipboard(str);
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
		code= iOSAdapterUtility::getInstance()->copyToClipboard(str);
#endif
		if (code == -1)//失败	
		{
			//弹窗
			PopLayer* popup = PopLayer::create();
			popup->setContentText(GetGameString("Copy_Fail"), 30, 800, 500);
			Director::getInstance()->getRunningScene()->addChild(popup);
		}
		if (code == 0)//成功
		{
			//弹窗
			PopLayer* popup = PopLayer::create();
			popup->setContentText(GetGameString("Copy_Success"), 30, 800, 500);
			Director::getInstance()->getRunningScene()->addChild(popup);
		}
	}
	
}
//关闭
void Circular::touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	this->removeFromParentAndCleanup(true);
}

