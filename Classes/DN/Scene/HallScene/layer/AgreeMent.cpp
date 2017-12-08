#include "Agreement.h"
#include "cocostudio/CocoStudio.h"
#include "../../../../GUISystem/WWSceneManager.h"
#include "GameAudioEngine.h"
#include "WWMacros.h"

USING_NS_CC;
using namespace cocos2d::experimental;
// on "init" you need to initialize your instance
bool Agreement::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	auto systemExitLayer = CSLoader::createNode("ProxyAgreementLayer.csb");
	if (systemExitLayer)
	{
		this->addChild(systemExitLayer);

		auto systemLayout = dynamic_cast<Layout*>(systemExitLayer->getChildByName("Panel"));
		if (systemLayout)
		{
			auto agreeButton = dynamic_cast<Button*>(systemLayout->getChildByName("Button_True"));
			if (agreeButton)
			{
				agreeButton->addTouchEventListener(CC_CALLBACK_2(Agreement::touchCloseEvent, this));
			}
			auto closeButton = dynamic_cast<Button*>(systemLayout->getChildByName("Button_False"));
			if (closeButton)
			{
				closeButton->addTouchEventListener(CC_CALLBACK_2(Agreement::touchCloseEvent, this));
			}

		}
	}

	//触碰事件  (单点触碰)
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(Agreement::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Agreement::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Agreement::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}
//触碰事件  （单点触碰）
bool Agreement::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	return true;
}
void Agreement::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{

}
void Agreement::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	
}

//关闭
void Agreement::touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	this->removeFromParentAndCleanup(true);
}
