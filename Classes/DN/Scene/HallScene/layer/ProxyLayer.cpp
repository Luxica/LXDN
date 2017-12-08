#include "ProxyLayer.h"
#include "cocostudio/CocoStudio.h"
#include "../../../../GUISystem/WWSceneManager.h"
#include "AgreeMent.h"
#include "GameAudioEngine.h"
#include "WWMacros.h"
#include "../../../Scene/HallScene/data/QHallManager.h"
#include "GameConfig.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#include "WWNetSprite.h"
#else
#include "GameNode/WWNetSprite.h"
#endif
USING_NS_CC;

using namespace cocos2d::experimental;
// on "init" you need to initialize your instance
bool ProxyLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    
	auto systemExitLayer = CSLoader::createNode("ProxyScene.csb");
	if (systemExitLayer)
	{
		this->addChild(systemExitLayer);

		auto systemLayout = dynamic_cast<Layout*>(systemExitLayer->getChildByName("Panel"));
		if (systemLayout)
		{
			auto closeButton = dynamic_cast<Button*>(systemLayout->getChildByName("Button_Close"));
			if (closeButton)
			{
				closeButton->addTouchEventListener(CC_CALLBACK_2(ProxyLayer::touchCloseEvent, this));
			}
			auto button_proxy_1 = dynamic_cast<Button*>(systemLayout->getChildByName("Button_proxy_1"));
			if (button_proxy_1)
			{
				button_proxy_1->setEnabled(false);
				button_proxy_1->setTag(1);
				button_proxy_1->addTouchEventListener(CC_CALLBACK_2(ProxyLayer::touchProxyEvent, this));
				m_buttonCur = button_proxy_1;
			}
			auto button_proxy_2 = dynamic_cast<Button*>(systemLayout->getChildByName("Button_proxy_2"));
			if (button_proxy_2)
			{
				button_proxy_2->setTag(2);
				button_proxy_2->addTouchEventListener(CC_CALLBACK_2(ProxyLayer::touchProxyEvent, this));
			}
			auto button_proxy_3 = dynamic_cast<Button*>(systemLayout->getChildByName("Button_proxy_3"));
			if (button_proxy_3)
			{
				button_proxy_3->setTag(3);
				button_proxy_3->addTouchEventListener(CC_CALLBACK_2(ProxyLayer::touchProxyEvent, this));
			}
			m_listProxy_1 = dynamic_cast<ListView*>(systemLayout->getChildByName("ListView_proxy_1"));
			if (m_listProxy_1)
			{
				m_listProxy_1->setVisible(true);
                m_listProxy_1->removeAllChildren();
                std::string path="Proxy/proxy1.png";
                auto pPortrait_1 = WWNetSprite::createWithUrl(GAMECONFIG.m_sProxy1Url.c_str(), path.c_str(), Size(500.0f, 390.0f));
                pPortrait_1->setLocalZOrder(-1);
                pPortrait_1->setAnchorPoint(Vec2(0.5f,0.5f));
                m_listProxy_1->addChild(pPortrait_1);
                pPortrait_1->setPosition(m_listProxy_1->getContentSize().width/2,m_listProxy_1->getContentSize().height/2);
			}
			m_listProxy_2 = dynamic_cast<ListView*>(systemLayout->getChildByName("ListView_proxy_2"));
			if (m_listProxy_2)
			{
				m_listProxy_2->setVisible(false);
			}
			m_listProxy_3 = dynamic_cast<ListView*>(systemLayout->getChildByName("ListView_proxy_3"));
			if (m_listProxy_3)
			{
				m_listProxy_3->setVisible(false);
                auto panel_proxy = dynamic_cast<Layout*>(m_listProxy_3->getChildByName("Panel_1"));
                if(panel_proxy)
                {
                    panel_proxy->removeAllChildren();
                    std::string path="Proxy/proxy2.png";
					WWNetSprite* pPortrait_2 = nullptr;
					pPortrait_2 = WWNetSprite::createWithUrl(GAMECONFIG.m_sProxy2Url.c_str(), path.c_str(), Size(500.0f, 390.0f));
                    pPortrait_2->setLocalZOrder(-1);
                    pPortrait_2->setAnchorPoint(Vec2(0.5f,0.5f));
                    panel_proxy->addChild(pPortrait_2);
                    pPortrait_2->setPosition(panel_proxy->getContentSize().width/2,panel_proxy->getContentSize().height/2);

                }
                
                
			}
			checkBox_Agreement = dynamic_cast<CheckBox*>(systemLayout->getChildByName("CheckBox_Agreement"));
			if (checkBox_Agreement)
			{
				checkBox_Agreement->setVisible(false);
			}
			button_Agreement = dynamic_cast<Button*>(systemLayout->getChildByName("Button_Agreement"));
			if (button_Agreement)
			{
				button_Agreement->setVisible(false);
				button_Agreement->addTouchEventListener(CC_CALLBACK_2(ProxyLayer::touchAgreementEvent, this));
			}
		}
	}

	//触碰事件  (单点触碰)
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(ProxyLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(ProxyLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(ProxyLayer::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

//触碰事件  （单点触碰）
bool ProxyLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	return true;
}
void ProxyLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{

}
void ProxyLayer::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	
}

//关闭
void ProxyLayer::touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	this->removeFromParentAndCleanup(true);
}

void ProxyLayer::touchAgreementEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	this->addChild(Agreement::create());
}

void ProxyLayer::touchProxyEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	auto buttonCur = (Button*)sender;
	if (m_buttonCur == buttonCur)
		return;
	buttonCur->setEnabled(false);
	m_buttonCur->setEnabled(true);
	m_buttonCur = buttonCur;
	switch (m_buttonCur->getTag())
	{
	case 1:
		m_listProxy_1->setVisible(true);
		m_listProxy_2->setVisible(false);
		m_listProxy_3->setVisible(false);
		break;
	case 2:
		m_listProxy_1->setVisible(false);
		m_listProxy_2->setVisible(true);
		m_listProxy_3->setVisible(false);
		break;
	case 3:
		m_listProxy_1->setVisible(false);
		m_listProxy_2->setVisible(false);
		m_listProxy_3->setVisible(true);
		break;
	default:
		break;
	}
	if (m_buttonCur->getTag() == 3)
	{
		checkBox_Agreement->setVisible(true);
		button_Agreement->setVisible(true);
	}
	else
	{
		checkBox_Agreement->setVisible(false);
		button_Agreement->setVisible(false);
	}
}
