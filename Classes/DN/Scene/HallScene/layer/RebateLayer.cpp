#include "RebateLayer.h"
#include "cocostudio/CocoStudio.h"
#include "../../../../GUISystem/WWSceneManager.h"
#include "GameAudioEngine.h"
#include "WWMacros.h"
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
bool RebateLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	auto systemExitLayer = CSLoader::createNode("RebateLayer.csb");
	if (systemExitLayer)
	{
		this->addChild(systemExitLayer);

		auto systemLayout = dynamic_cast<Layout*>(systemExitLayer->getChildByName("Panel"));
		if (systemLayout)
		{
			auto closeButton = dynamic_cast<Button*>(systemLayout->getChildByName("Button_Close"));
			if (closeButton)
			{
				closeButton->addTouchEventListener(CC_CALLBACK_2(RebateLayer::touchCloseEvent, this));
			}
			m_listView = dynamic_cast<ListView*>(systemLayout->getChildByName("ListView_rebate"));
			if (m_listView)
			{
				m_rebateLayout = dynamic_cast<Layout*>(m_listView->getChildByName("Panel_rebate"));
				if (m_rebateLayout)
				{
					m_rebateLayout->setVisible(false);
				}
				auto panel_1 = dynamic_cast<Layout*>(m_listView->getChildByName("Panel_1"));
				if (panel_1)
				{
					auto button_Copy = dynamic_cast<Button*>(panel_1->getChildByName("Button_Copy"));
					if (button_Copy)
					{
						button_Copy->setTag(1);
						button_Copy->addTouchEventListener(CC_CALLBACK_2(RebateLayer::touchCopyEvent, this));
					}
					auto text_WxNum = dynamic_cast<Text*>(panel_1->getChildByName("Text_WxNum"));
					if (text_WxNum)
					{
						m_selfWxNum = text_WxNum->getString();
					}
				}
			}
			char name[255] = {};
			for (int i = 0; i < 6; i++)
			{
				sprintf(name, "Button_Copy_%d", i);
				auto button_copy = dynamic_cast<Button*>(systemLayout->getChildByName(name));
				if (button_copy)
				{
					button_copy->addTouchEventListener(CC_CALLBACK_2(RebateLayer::touchCopyEvent, this));
				}
			}
		}
	}

	//触碰事件  (单点触碰)
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(RebateLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(RebateLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(RebateLayer::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}
//触碰事件  （单点触碰）
bool RebateLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	return true;
}
void RebateLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{

}
void RebateLayer::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	
}

//关闭
void RebateLayer::touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	this->removeFromParentAndCleanup(true);
}

void RebateLayer::touchCopyEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	auto button_copy = (Button*)sender;

	char str[100] = { 0 };
	switch (button_copy->getTag())
	{
	case 1:
	{
		const char * wxNum = m_selfWxNum.c_str();
		strcpy(str, wxNum);
		break;
	}
	case 2:
		sprintf(str, "%s", GetGameString("Text_WX_2"));
		break;
	case 3:
		sprintf(str, "%s", GetGameString("Text_WX_3"));
		break;
	case 4:
		sprintf(str, "%s", GetGameString("Text_WX_4"));
		break;
	default:
		break;
	}
	int code = 2;
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	code = AdmobJni::copyToClipboard(str);
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	code = iOSAdapterUtility::getInstance()->copyToClipboard(str);
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



