#include "ShareLayer.h"
#include "cocostudio/CocoStudio.h"
#include "../../../../GUISystem/WWSceneManager.h"
#include "GameAudioEngine.h"
#include "WWMacros.h"
#include "WxSocial.h"
#include "GameConfig.h"
#include "ScriptDataManager.h"
#include "../../../../DN/Comm/request/MsgHallRequest.h"
#include "../data/QHallManager.h"
#include "../MainMenu.h"
#include "StringTool.h"

USING_NS_CC;
using namespace cocos2d::experimental;
using namespace std;
// on "init" you need to initialize your instance
bool ShareLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	m_isShare = 0;
	auto systemExitLayer = CSLoader::createNode("ShareLayer.csb");
	if (systemExitLayer)
	{
		this->addChild(systemExitLayer);
		systemLayout = dynamic_cast<Layout*>(systemExitLayer->getChildByName("Panel"));
		if (systemLayout)
		{
			auto friendButton = dynamic_cast<Button*>(systemLayout->getChildByName("Button_1"));
			if (friendButton)
			{
				friendButton->addTouchEventListener(CC_CALLBACK_2(ShareLayer::touchToFriendEvent, this));
			}
			auto coterieButton = dynamic_cast<Button*>(systemLayout->getChildByName("Button_2"));
			if (coterieButton)
			{
				coterieButton->addTouchEventListener(CC_CALLBACK_2(ShareLayer::touchToCoterieEvent, this));
			}
			auto closeButton = dynamic_cast<Button*>(systemLayout->getChildByName("Button_Close"));
			if (closeButton)
			{
				closeButton->addTouchEventListener(CC_CALLBACK_2(ShareLayer::touchCloseEvent, this));
			}
		}
	}

	//触碰事件  (单点触碰)
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(ShareLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(ShareLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(ShareLayer::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}
//触碰事件  （单点触碰）
bool ShareLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	return true;
}
void ShareLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{

}
void ShareLayer::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	
}

//关闭
void ShareLayer::touchToFriendEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	m_isShare = 1;
	WxSocial::getInstance()->shareRoom(GAMECONFIG.m_sHttpDownUrl.c_str(), GAMECONFIG.m_sAppName.c_str(), GetGameString("WXInvite3"), CC_CALLBACK_1(ShareLayer::onInviteAB, this));
}
//分享到朋友圈
void ShareLayer::touchToCoterieEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	m_isShare = 2;
	char wxTitle[255] = { 0 };
	sprintf(wxTitle, GetGameString("WXInvite4"), QHallManager::getInstance()->getUserId());
	cocos2d::log("ShareLayer::touchToCoterieEvent %s", wxTitle);

	WxSocial::getInstance()->shareRoomTo(GAMECONFIG.m_sHttpDownUrl.c_str(), wxTitle, GetGameString("WXInvite4"), CC_CALLBACK_1(ShareLayer::onInviteCB, this));
}


void ShareLayer::touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	this->removeFromParentAndCleanup(true);
}

void ShareLayer::onInviteCB(int errorCode)
{
	cocos2d::log("ShareLayer::onInviteCB %d", errorCode);
	switch (errorCode)
	{
	case 0:
		MsgHallRequest::sendDayShare();
		MsgHallRequest::sendCanShare();
		break;
	case -2:
		break;
	case -4:
		break;
	default:
		break;
	}
    this->removeFromParentAndCleanup(true);
	return;
}

void ShareLayer::onInviteAB(int errorCode)
{
	cocos2d::log("ShareLayer::onInviteAB %d", errorCode);
    switch (errorCode)
    {
    case 0:
        this->removeFromParentAndCleanup(true);
        break;
    default:
        return;
        break;
    }
    return;
}

void ShareLayer::setUID(int uid)
{
	auto textUID = dynamic_cast<Text*>(systemLayout->getChildByName("Text_ID"));
	if (textUID)
	{
		char dzUID[8] = {0};
		sprintf(dzUID,"%d",uid);
		textUID->setString(dzUID);
	}
}
