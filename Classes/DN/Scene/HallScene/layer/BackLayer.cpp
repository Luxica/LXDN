#include "BackLayer.h"
#include "cocostudio/CocoStudio.h"
#include "../../../../GUISystem/WWSceneManager.h"
#include "../../../Scene/LoginScene/QLoginScene.h"
#include "GameAudioEngine.h"
#include "WWMacros.h"

USING_NS_CC;
using namespace cocos2d::experimental;
// on "init" you need to initialize your instance
bool BackLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto systemExitLayer = CSLoader::createNode("System_Exit/SystemExitLayer.csb");
	addChild(systemExitLayer);
	
	auto systemLayout = dynamic_cast<Layout*>(systemExitLayer->getChildByName("Panel"));
	auto trueButton = dynamic_cast<Button*>(systemLayout->getChildByName("Button_Ture"));
	trueButton->addTouchEventListener(CC_CALLBACK_2(BackLayer::touchTrueEvent, this));

	auto falseButton = dynamic_cast<Button*>(systemLayout->getChildByName("Button_False"));
	falseButton->addTouchEventListener(CC_CALLBACK_2(BackLayer::touchFalseEvent, this));

	//触碰事件  (单点触碰)
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(BackLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(BackLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(BackLayer::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}
//点击选择武将 触碰事件
//触碰事件  （单点触碰）
bool BackLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	return true;
}
void BackLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{

}
void BackLayer::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	
}

//雇佣事件
void BackLayer::touchTrueEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	WWSceneManager::getInstance()->closeAllUI();
	WWSceneManager::getInstance()->openScene(QLoginScene::create());
}
//关闭
void BackLayer::touchFalseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	WWSceneManager::getInstance()->closeGlobalUI(this);

}



