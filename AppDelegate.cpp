#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "ConfigSystem/ww-config.h"
#include "net/LordNetManger.h"
#include "SimpleAudioEngine.h"
#include "DN/Scene/LoginScene/QLoginScene.h"
#include "DN/Scene/StareScene/QStartScene.h"
#include "DN/Comm/filter/MsgGlobalFilter.h"  
#include "DN/Comm/filter/MsgHallFilter.h"
#include "DN/Comm/filter/MsgGameFilter.h"
#include "DN/Comm/request/MsgGameRequest.h"  
#include "GUISystem/WWSceneManager.h"
#include "DN/Controllers/ControllerMgr.h"
#include "ScriptDataManager.h"
#include "GameConfig.h"
#include "ProceConfig.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "iOSAdapterUtility.hpp"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "bugly/CrashReport.h"     
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "bugly/CrashReport.h"
#endif

USING_NS_CC;

#define DEBUG_CONSOLE

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
	ControllerMgr::destroyInstance();
	WWSceneManager::destoryInstance();
#ifdef DEBUG_CONSOLE
	Director::getInstance()->getConsole()->stop();
#endif
}


//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::createWithRect("HelloCpp", Rect(0, 0, 960, 540));
        director->setOpenGLView(glview);
    }

    director->getOpenGLView()->setDesignResolutionSize(1920, 1080, ResolutionPolicy::SHOW_ALL);

    // turn on display FPS
    //director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
	//------------------------------------------------------------------
	std::string buglyAppID = "";

	if (GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		buglyAppID = "23e9726b1f";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		buglyAppID = "846325483e";
#endif
	}



	if (buglyAppID.size() > 0) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		CrashReport::initCrashReport(buglyAppID.c_str(), true);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		CrashReport::initCrashReport(buglyAppID.c_str(), true);
#endif
	}

#ifdef DEBUG_CONSOLE
	// telnet 
	//director->getConsole()->setBindAddress("127.0.0.1");
	director->getConsole()->listenOnTCP(8890);
#endif // DEBUG_CONSOLE

	ControllerMgr::getInstance();
    ScriptDataManager::getInterface()->init();
    FileUtils::getInstance()->addSearchPath("res");
    
	//WWConfigManager::getInstance()->loadXML("XML/config.xml");
	//------------------------------------------------------------------
    // create a scene. it's an autorelease object
	//网络模块
	WWMsgManager::getInstance()->registerMsgFilter(MsgHallFilter::create());
	WWMsgManager::getInstance()->registerMsgFilter(MsgGlobalFilter::create());
	WWMsgManager::getInstance()->registerMsgFilter(MsgGameFilter::create());

	if (PROCECONFIG.m_bShowStartScene)
	{
		WWSceneManager::getInstance()->openScene(QStartScene::create());
	}
	else
	{
		WWSceneManager::getInstance()->openScene(QLoginScene::create());
	}

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	CrashReport::log(CrashReport::Info, "AppDelegate::applicationDidEnterBackground", "application did enter background............");
#endif
	Director::getInstance()->pause();
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    
    // if you use SimpleAudioEngine, it must be pause
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    //CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::getInstance()->stopAllEffects();
    
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    
#endif

	CCDirector::sharedDirector()->setProjection(CCDirector::Projection::_2D);
	CCDirector::sharedDirector()->stopAnimation();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	CrashReport::log(CrashReport::Info, "AppDelegate::applicationWillEnterForeground", "application will wenter foreground............");
#endif
	Director::getInstance()->resume();
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    // if you use SimpleAudioEngine, it must resume here
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    //CocosDenshion::SimpleAudioEngine::getInstance()->resumeAllEffects();
    
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif

    CCDirector::sharedDirector()->startAnimation();
}
