#include "Sociaty.h"
#include "cocostudio/CocoStudio.h"
#include "../../../../GUISystem/WWSceneManager.h"
#include "GameAudioEngine.h"
#include "WWMacros.h"

USING_NS_CC;
using namespace cocos2d::experimental;
// on "init" you need to initialize your instance
bool Sociaty::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	auto systemSociatyLayer = CSLoader::createNode("SociatyLayer.csb");
	if (systemSociatyLayer)
	{
		this->addChild(systemSociatyLayer);
		auto layer = dynamic_cast<Layout*>(systemSociatyLayer->getChildByName("Panel"));
		if (layer)
		{
			auto closeButton = dynamic_cast<Button*>(layer->getChildByName("Button_Close"));
			if (closeButton)
			{
				closeButton->addTouchEventListener(CC_CALLBACK_2(Sociaty::touchCloseEvent, this));
			}
		}
	}

	//触碰事件  (单点触碰)
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(Sociaty::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Sociaty::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Sociaty::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void Sociaty::touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	this->removeFromParentAndCleanup(true);
}

//触碰事件  （单点触碰）
bool Sociaty::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	return true;
}
void Sociaty::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{

}
void Sociaty::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	
}
