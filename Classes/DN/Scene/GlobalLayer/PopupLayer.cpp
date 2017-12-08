#include "PopupLayer.h"
#include "cocostudio/CocoStudio.h"
#include "../../../GUISystem/WWSceneManager.h"
#include "../LoginScene/QLoginScene.h"
#include "../LoginScene/data/QLoginManager.h"
#include "ScriptDataManager.h"
#include "GameAudioEngine.h"
#include "WWMacros.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#include "LordNetManger.h"
#else
#include "net/LordNetManger.h"
#endif
USING_NS_CC;
using namespace cocos2d::experimental;

PopupLayer* PopupLayer::create(std::string path)
{
	PopupLayer *pRet = new PopupLayer();
	if (pRet && pRet->init(path))
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

bool PopupLayer::init(std::string path)
{
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	m_bgLayout = nullptr;
	auto systemExitLayer = CSLoader::createNode("PopupLayer.csb");
	addChild(systemExitLayer);
	m_bgLayout = dynamic_cast<Layout*>(systemExitLayer->getChildByName("Panel_Bg"));
	if (m_bgLayout)
	{
		m_bgLayout->addTouchEventListener(CC_CALLBACK_2(PopupLayer::touchCloseEvent, this));
	}
	m_systemLayout = dynamic_cast<Layout*>(systemExitLayer->getChildByName("Panel"));
	auto closeBtn = dynamic_cast<Button*>(m_systemLayout->getChildByName("Button_Close"));
	if (closeBtn)
	{
		closeBtn->addTouchEventListener(CC_CALLBACK_2(PopupLayer::touchCloseEvent, this));
	}

	auto listview = dynamic_cast<ListView*>(m_systemLayout->getChildByName("ListView"));
	if (listview)
	{
		auto image_Show = dynamic_cast<ImageView*>(listview->getChildByName("Image_Show"));
		if (image_Show)
		{
			image_Show->loadTexture(path);
		}
	}


	//触碰事件  (单点触碰)
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(PopupLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(PopupLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(PopupLayer::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}
//触碰事件  （单点触碰）
bool PopupLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	return true;
}
void PopupLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{

}
void PopupLayer::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	
}

//确认
void PopupLayer::touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);

	WWSceneManager::getInstance()->closeGlobalUI(this);
}



