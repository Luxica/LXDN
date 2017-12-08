#include "ShopLayer.h"
#include "cocostudio/CocoStudio.h"
#include "../../../../GUISystem/WWSceneManager.h"
#include "GameAudioEngine.h"
#include "WWMacros.h"
#include "../../GlobalLayer/DialogLayer.h"
#include "ScriptDataManager.h"
#include "../../../Scene/HallScene/data/QHallManager.h"
#include "../../GlobalLayer/PopLayer.h"
#include "WxSocial.h"
#include "GameConfig.h"
#include "GameAudioEngine.h"
#include "StringTool.h"
#include "WWMacros.h"
#include "../../../Scene/HallScene/MainMenu.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "iOSAdapterUtility.hpp"
#endif
#include "AdmobJni.h"

USING_NS_CC;
using namespace cocos2d::experimental;
// on "init" you need to initialize your instance
bool ShopLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto systemExitLayer = CSLoader::createNode("shopLayer.csb");
	if (systemExitLayer)
	{
		addChild(systemExitLayer);
		auto systemLayout = dynamic_cast<Layout*>(systemExitLayer->getChildByName("Panel_Layout"));
		if (systemLayout)
		{
			m_cardNum = dynamic_cast<Text*>(systemLayout->getChildByName("Text_Card"));
			if (m_cardNum)
				m_cardNum->setString(StringTool::intToString(QHallManager::getInstance()->getRoomCard()));
			auto trueButton = dynamic_cast<Button*>(systemLayout->getChildByName("closeButton"));
			if (trueButton)
			{
				trueButton->addTouchEventListener(CC_CALLBACK_2(ShopLayer::touchCloseEvent, this));
			}

			auto falseButton = dynamic_cast<Button*>(systemLayout->getChildByName("Button_close"));
			if (falseButton)
			{
				falseButton->addTouchEventListener(CC_CALLBACK_2(ShopLayer::touchCloseEvent, this));
			}

			auto card_Num = dynamic_cast<Text*>(systemLayout->getChildByName("Text_Card"));
			if (card_Num)
			{
				card_Num->setString(StringTool::intToString(QHallManager::getInstance()->getRoomCard()));
			}

			char szName[100] = { 0 };
			auto listView = dynamic_cast<ListView*>(systemLayout->getChildByName("ListView"));
			for (int index = 1; index < 10; index++)
			{
				sprintf(szName, "Panel_%d", index);
				auto layout_card = dynamic_cast<Layout*>(listView->getChildByName(szName));
				if (layout_card)
				{
					auto buy_Btn = dynamic_cast<Button*>(layout_card->getChildByName("Button_Shop"));
					if (buy_Btn)
					{
						buy_Btn->setTag(index);
						buy_Btn->addTouchEventListener(CC_CALLBACK_2(ShopLayer::touchCardEvent, this));
					}
				}
			}
		}
	}
	
	//触碰事件  (单点触碰)
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(ShopLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(ShopLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(ShopLayer::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}
//点击选择武将 触碰事件
//触碰事件  （单点触碰）
bool ShopLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	return true;
}
void ShopLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{

}
void ShopLayer::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	
}


//关闭
void ShopLayer::touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::ENDED://按钮按下在按钮上抬起

		GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
		this->removeFromParentAndCleanup(true);
	
		break;
	}
}

void ShopLayer::touchCardEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
        return;
    int userType = QHALLMGR->getUserType();
    
    if (GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL)
    {
		if (userType == 0)
        {
            MainMenu * menu = dynamic_cast<MainMenu*>(m_parentNode);
            menu->onShowInviteDlg();
            this->removeFromParentAndCleanup(true);
            return;
        }
        int userId = QHALLMGR->getUserId();
        Button* node = (Button*)sender;
        int tag = node->getTag();
		if (userType == 2)
		{
		}
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    iOSAdapterUtility::getInstance()->thirdPay(1,tag,userId,CC_CALLBACK_1(ShopLayer::thirdPayCallBack, this));
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    AdmobJni::thirdPay(1, tag,userId,CC_CALLBACK_1(ShopLayer::thirdPayCallBack, this));
#endif
    }
    else
    {
        m_shopingLayer = DialogLayer::create(1, GetGameString("buy_card"));
        this->addChild(m_shopingLayer);
        auto text = dynamic_cast<Text*>(m_shopingLayer->m_systemLayout->getChildByName("Text"));
        text->ignoreContentAdaptWithSize(false);
        text->setSize(Size(350, 70));
        text->setFontSize(20.0f);
        auto button = dynamic_cast<Button*>(m_shopingLayer->m_systemLayout->getChildByName("Button_Ture_0"));
        button->addTouchEventListener(CC_CALLBACK_2(ShopLayer::touchLoginShopEvent, this));
    }
}
void ShopLayer::touchLoginShopEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	m_shopingLayer->removeFromParentAndCleanup(true);
}
void ShopLayer::thirdPayCallBack(int code)
{
    std::string strResult = "";
    switch(code)
    {
        case 0: //成功
            strResult = GetGameString("PayResult1");
            break;
        case 1: //取消
            strResult = GetGameString("PayResult2");
            break;
        case -1: //失败
            strResult = GetGameString("PayResult3");
            break;
            
        default:
            strResult = GetGameString("nullError");
            log("error pay %d",code);
            break;
    }
    PopLayer* popup = PopLayer::create();
    popup->setContentText(strResult.c_str(), 30, 800, 500);
    Director::getInstance()->getRunningScene()->addChild(popup,100);
}

void ShopLayer::setParentScene(Node* node)
{
    m_parentNode = node;
}

void ShopLayer::setRoomCard()
{
	if (m_cardNum)
		m_cardNum->setString(StringTool::intToString(QHallManager::getInstance()->getRoomCard()));
}
