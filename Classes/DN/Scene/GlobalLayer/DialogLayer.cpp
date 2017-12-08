#include "DialogLayer.h"
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

DialogLayer* DialogLayer::create(int type, std::string sContent)
{
	DialogLayer *pRet = new DialogLayer();
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

bool DialogLayer::init(int type, std::string sContent)
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	auto systemExitLayer = CSLoader::createNode("System_Exit/SystemExitLayer.csb");
	addChild(systemExitLayer);
	m_systemLayout = dynamic_cast<Layout*>(systemExitLayer->getChildByName("Panel"));
	auto trueButton = dynamic_cast<Button*>(m_systemLayout->getChildByName("Button_Ture"));
	auto trueClone = dynamic_cast<Button*>(m_systemLayout->getChildByName("Button_Ture_0"));
	auto falseButton = dynamic_cast<Button*>(m_systemLayout->getChildByName("Button_False"));
	switch (type)	
	{
	case 1:
		if (trueClone)
		{
			trueClone->setVisible(true);
			trueClone->addTouchEventListener(CC_CALLBACK_2(DialogLayer::touchTrueEvent, this));
		}
		break;
	case 2:
		if (trueButton&&falseButton)
		{
			trueButton->setVisible(true);
			falseButton->setVisible(true);
			trueButton->addTouchEventListener(CC_CALLBACK_2(DialogLayer::touchTrueEvent, this));
			falseButton->addTouchEventListener(CC_CALLBACK_2(DialogLayer::touchFalseEvent, this));
		}
		break;
	default:
		break;
	}
	
	auto messageText = dynamic_cast<Text*>(m_systemLayout->getChildByName("Text"));
	if (sContent == GetGameString("GZ_YNF"))
	{
		messageText->setTextHorizontalAlignment(TextHAlignment::LEFT);
	}
	else
	{
		messageText->setTextHorizontalAlignment(TextHAlignment::CENTER);
	}
	messageText->setString(sContent);
	//messageText->setTextAreaSize(Size(100,0));

	//触碰事件  (单点触碰)
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(DialogLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(DialogLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(DialogLayer::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}
//触碰事件  （单点触碰）
bool DialogLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	return true;
}
void DialogLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{

}
void DialogLayer::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	
}

//确认
void DialogLayer::touchTrueEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	/*GameAudioEngine::getInstance()->stopBackgroundMusic();
	auto userDefault = UserDefault::getInstance();*/

	WWSceneManager::getInstance()->closeAllUI();
	LordNetManger::getInstance()->onDisconnect(WWNetwork::eHallNet);
	WWSceneManager::getInstance()->openScene(QLoginScene::create());
    QLoginManager::getInstance()->setLoginType(1);
}
//关闭
void DialogLayer::touchFalseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	WWSceneManager::getInstance()->closeGlobalUI(this);
}



