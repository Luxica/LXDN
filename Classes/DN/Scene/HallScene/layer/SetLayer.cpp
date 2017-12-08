#include "SetLayer.h"
#include "cocostudio/CocoStudio.h"
#include "../../../../GUISystem/WWSceneManager.h"
#include "WWMacros.h"
#include "GameAudioEngine.h"
#include "WWMacros.h"
#include "../QHallScene.h"
#include "../../GameScene/QGameScene.h"
#include "../../../Scene/LoginScene/QLoginScene.h"
#include "../../LoginScene/data/QLoginManager.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#include "LordNetManger.h"
#else
#include "net/LordNetManger.h"
#endif
USING_NS_CC;
using namespace cocos2d::experimental;
// on "init" you need to initialize your instance
bool SetLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	auto systemExitLayer = CSLoader::createNode("GameSet/GameSetLayer.csb");
    
	auto userDefault = UserDefault::getInstance();

	float music = userDefault->getFloatForKey(MUSIC_VALUE_TEXT, 0.5);
	float sound = userDefault->getFloatForKey(SOUND_VALUE_TEXT, 0.5);

	if (systemExitLayer)
	{
		this->addChild(systemExitLayer);
		auto systemLayout = dynamic_cast<Layout*>(systemExitLayer->getChildByName("Panel"));
		if (systemLayout)
		{
			auto closeButton = dynamic_cast<Button*>(systemLayout->getChildByName("Button_Close"));
			if (closeButton)
			{
				closeButton->addTouchEventListener(CC_CALLBACK_2(SetLayer::touchCloseEvent, this));
			}

			auto slider_Music = dynamic_cast<Slider*>(systemLayout->getChildByName("Slider_Music"));
			if (slider_Music)
			{
				slider_Music->setPercent(music * 100);
				slider_Music->addEventListener(CC_CALLBACK_2(SetLayer::musicSliderCallBack, this));
			}

			auto slider_Sound = dynamic_cast<Slider*>(systemLayout->getChildByName("Slider_Sound"));
			if (slider_Sound)
			{
				slider_Sound->setPercent(sound * 100);
				slider_Sound->addEventListener(CC_CALLBACK_2(SetLayer::soundSliderCallBack, this));
			}

			auto button_Exit = dynamic_cast<Button*>(systemLayout->getChildByName("Button_Exit"));
			if (button_Exit)
			{
				button_Exit->addTouchEventListener(CC_CALLBACK_2(SetLayer::touchExitEvent, this));
			}
		}
	}

	//触碰事件  (单点触碰)
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(SetLayer::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(SetLayer::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(SetLayer::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}
//触碰事件  （单点触碰）
bool SetLayer::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	return true;
}
void SetLayer::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{

}
void SetLayer::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	
}

void SetLayer::touchMusicEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_CHECKBOX_EFFECT);
	//音乐开关
	auto user = UserDefault::getInstance();
	auto button_Music = dynamic_cast<Button*>(sender);
	if (button_Music->isBright() == true)
	{
		button_Music->setBright(false);
		//音乐关
		user->setIntegerForKey(MUSIC_VALUE_TEXT, 0);
		GameAudioEngine::getInstance()->setSwitchOfMusic(false);
		GameAudioEngine::getInstance()->pauseBackgroundMusic();
		GameAudioEngine::getInstance()->setBackgroundMusicVolume(0.0f);
	}
	else
	{
		button_Music->setBright(true);
		//音乐开
		user->setIntegerForKey(MUSIC_VALUE_TEXT, 1);
		GameAudioEngine::getInstance()->setSwitchOfMusic(true);
		GameAudioEngine::getInstance()->resumeBackgroundMusic();

		QHallScene* hallScene = dynamic_cast<QHallScene*>(Director::getInstance()->getRunningScene());
		if (hallScene)
		{
			GameAudioEngine::getInstance()->playBackgroundMusic(GAME_HALL_BACKGROUND_MUSIC,true);
		}
		QGameScene* gameScene = dynamic_cast<QGameScene*>(Director::getInstance()->getRunningScene());
		if (gameScene)
		{
			GameAudioEngine::getInstance()->playBackgroundMusic(GAME_GAME_BACKGROUND_MUSIC,true);
		}
		
		GameAudioEngine::getInstance()->setBackgroundMusicVolume(1.0f);
	}
}

void SetLayer::touchSoundEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_CHECKBOX_EFFECT);
	//音乐开关
	auto user = UserDefault::getInstance();
	auto button_Sound = dynamic_cast<Button*>(sender);
	if (button_Sound->isBright() == true)
	{
		button_Sound->setBright(false);
		//音效关
		user->setIntegerForKey(SOUND_VALUE_TEXT, 0);
		GameAudioEngine::getInstance()->setSwitcjofEffect(false);
		GameAudioEngine::getInstance()->setEffectsVolume(0.0f);
	}
	else
	{
		button_Sound->setBright(true);
		//音效开
		user->setIntegerForKey(SOUND_VALUE_TEXT, 1);
		GameAudioEngine::getInstance()->setSwitcjofEffect(true);
		GameAudioEngine::getInstance()->setEffectsVolume(1.0f);
	}

}

void SetLayer::musicSliderCallBack(Ref *pSender, Slider::EventType type)
{
	auto item = (Slider*)(pSender);
	auto user = UserDefault::getInstance();
	float musicValue = item->getPercent() / 100.0f;
	user->setFloatForKey(MUSIC_VALUE_TEXT, musicValue);
	GameAudioEngine::getInstance()->setBackgroundMusicVolume(item->getPercent() / 100.0f);
}

void SetLayer::soundSliderCallBack(Ref *pSender, Slider::EventType type)
{
	auto item = (Slider*)(pSender);
	auto user = UserDefault::getInstance();
	float soundValue = item->getPercent() / 100.0f;
	user->setFloatForKey(SOUND_VALUE_TEXT, soundValue);
	GameAudioEngine::getInstance()->setEffectsVolume(item->getPercent() / 100.0f);
}

void SetLayer::touchCancelEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	WWSceneManager::getInstance()->closeAllUI();
	LordNetManger::getInstance()->onDisconnect(WWNetwork::eHallNet);
	WWSceneManager::getInstance()->openScene(QLoginScene::create());
	QLoginManager::getInstance()->setLoginType(1);
}

void SetLayer::showMyself()
{
	this->setVisible(true);
}

//关闭
void SetLayer::touchCloseEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	this->removeFromParentAndCleanup(true);
}

void SetLayer::touchExitEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	GameAudioEngine::getInstance()->playEffect(GAME_CLICK_BUTTON_EFFECT);
	WWSceneManager::getInstance()->exit();
}


