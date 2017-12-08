#include "NewsLayer.h"
#include "cocostudio/CocoStudio.h"
#include "../../../../GUISystem/WWSceneManager.h"
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
bool NewsLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	m_CurSelect = nullptr;
	auto systemExitLayer = CSLoader::createNode("News/NewsLayer.csb");
	if (systemExitLayer)
	{
		this->addChild(systemExitLayer);

		auto systemLayout = dynamic_cast<Layout*>(systemExitLayer->getChildByName("Panel"));
		if (systemLayout)
		{
			auto closeButton = dynamic_cast<Button*>(systemLayout->getChildByName("Button_Close"));
			if (closeButton)
			{
				closeButton->addTouchEventListener(CC_CALLBACK_2(NewsLayer::touchCloseEvent, this));
			}
			auto checkBox_GZH = dynamic_cast<CheckBox*>(systemLayout->getChildByName("CheckBox_GZH"));
			if (checkBox_GZH)
			{
				checkBox_GZH->setSelected(true);
				checkBox_GZH->setTouchEnabled(false);
				checkBox_GZH->setTag(1);
				checkBox_GZH->addEventListener(CC_CALLBACK_2(NewsLayer::touchNewsEvent, this));
				m_CurSelect = checkBox_GZH;
			}
			auto checkBox_GFSM = dynamic_cast<CheckBox*>(systemLayout->getChildByName("CheckBox_GFSM"));
			if (checkBox_GFSM)
			{
				checkBox_GFSM->setSelected(false);
				checkBox_GFSM->setTag(2);
				checkBox_GFSM->addEventListener(CC_CALLBACK_2(NewsLayer::touchNewsEvent, this));
			}
			m_listGZH = dynamic_cast<ListView*>(systemLayout->getChildByName("ListView_GZH"));
			if (m_listGZH)
			{
				m_listGZH->setVisible(true);
			}
			m_listGFSM = dynamic_cast<ListView*>(systemLayout->getChildByName("ListView_GFSM"));
			if (m_listGFSM)
			{
				m_listGFSM->setVisible(false);
				if (GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL)
				{
					auto panel_news = dynamic_cast<Layout*>(m_listGFSM->getChildByName("Panel_News"));
					if (panel_news)
					{
						panel_news->removeAllChildren();
						std::string path = "News/new.png";
						auto pPortrait = WWNetSprite::create();
						if (GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL)
						{
							pPortrait = WWNetSprite::createWithUrl(GAMECONFIG.m_sNewsUrl.c_str(), path.c_str(), Size(615.0f, 365.0f));
						}
						pPortrait->setLocalZOrder(-1);
						pPortrait->setAnchorPoint(Vec2(0.5f, 0.5f));
						panel_news->addChild(pPortrait);
						pPortrait->setPosition(panel_news->getContentSize().width / 2, panel_news->getContentSize().height / 2);
					}
				}
			}
        }
    }

	//触碰事件  (单点触碰)
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(NewsLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(NewsLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(NewsLayer::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}
//触碰事件  （单点触碰）
bool NewsLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	return true;
}
void NewsLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{

}
void NewsLayer::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	
}

//关闭
void NewsLayer::touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	this->removeFromParentAndCleanup(true);
}

void NewsLayer::touchNewsEvent(Ref* pSender, CheckBox::EventType type)
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
	case 1:
		m_listGZH->setVisible(true);
		m_listGFSM->setVisible(false);
		break;
	case 2:
		m_listGZH->setVisible(false);
		m_listGFSM->setVisible(true);
		break;
	default:
		break;
	}
}
