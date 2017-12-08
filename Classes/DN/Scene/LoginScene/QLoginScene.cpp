#include "QLoginScene.h"
#include "data/QLoginManager.h"
#include "cocostudio/CocoStudio.h"
#include "QLoginLayer.h"
#include "../../../GUISystem/WWSceneManager.h"
#include "../../../net/LordNetManger.h"
#include "../../Comm/request/MsgLogonRequest.h"
#include "../../../ConfigSystem/ww-config.h"
#include "../GlobalLayer/Load.h"
#include "../GlobalLayer/DialogLayer.h"
#include "GameConfig.h"
#include "ScriptDataManager.h"
#include "AdmobJni.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "iOSAdapterUtility.hpp"
#endif

USING_NS_CC;
using namespace cocos2d::network;
using namespace cocostudio::timeline;

QLoginScene::QLoginScene()
{
	m_nHttpCounts_ = 0;
	setName("QLoginScene");
	log("Enter...QLoginScene");
}

QLoginScene::~QLoginScene()
{
	log("exit....QLoginScene");
}

void QLoginScene::onEnter()
{
	Scene::onEnter();
	WWMsgManager::getInstance()->registerMsgFilter(this);
	LordNetManger::getInstance()->connectNet(WWNetwork::eLoginNet);
	
	
	//显示转圈圈界面
	m_loading = Load::create();
	WWSceneManager::getInstance()->openGlobalUI(m_loading);
	
	sendVersionHttp();
}

void QLoginScene::onExit()
{
	WWMsgManager::getInstance()->unregisterMsgFilter(this);
	Scene::onExit();
}

bool QLoginScene::init()
{
	auto layer=QLoginLayer::create();
	this->addChild(layer);

	//Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("SHOW_WAIT");
	return true;
}

void QLoginScene::sendVersionHttp()
{
	HttpRequest* request = new (std::nothrow) HttpRequest();
	request->setUrl(GAMECONFIG.m_sHttpVer.c_str());
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback(CC_CALLBACK_2(QLoginScene::onHttpRequestCompleted, this));
	request->setTag("GET_VERSION");
	HttpClient::getInstance()->sendImmediate(request);
	request->release();
	m_nHttpCounts_++;
}

void QLoginScene::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
{
	if (!response)
	{
		return;
	}
	long statusCode = response->getResponseCode();
	log("statusCode: %d", statusCode);
	if (statusCode < 200 || statusCode > 299)
	{
		//转圈圈界面消失
		if (m_loading)
		{
			WWSceneManager::getInstance()->closeGlobalUI(m_loading);
		}
		//地址错误或者地址访问不了  弹框显示错误 确定按钮（退出游戏）
		m_dialogLayer = DialogLayer::create(1, GetGameString("lose_updateLose"));
		this->addChild(m_dialogLayer);
		auto button = dynamic_cast<Button*>(m_dialogLayer->m_systemLayout->getChildByName("Button_Ture_0"));
		button->addTouchEventListener(CC_CALLBACK_2(QLoginScene::touchExitEvent, this));

		return;
	}
	log("isScucceed: %d", response->isSucceed());
	if (!response->isSucceed())
	{
		log("response failed");
		log("error buffer: %s", response->getErrorBuffer());
		if (m_nHttpCounts_<5)
			sendVersionHttp();
		else
		{
			//转圈圈界面消失
			WWSceneManager::getInstance()->closeGlobalUI(m_loading);
			//提示 您的网络不稳定	弹框显示错误 确定按钮（退出游戏）
			m_dialogLayer = DialogLayer::create(1, GetGameString("lose_webLose"));
			this->addChild(m_dialogLayer);
			auto button = dynamic_cast<Button*>(m_dialogLayer->m_systemLayout->getChildByName("Button_Ture_0"));
			button->addTouchEventListener(CC_CALLBACK_2(QLoginScene::touchExitEvent, this));

		}
		return;
	}

	// dump data
	std::vector<char> *buffer = response->getResponseData();
	std::string buff_contain_origin(buffer->begin(), buffer->end());
	log("buff_contain_origin====%s", buff_contain_origin.c_str());
	//std::string sVersion = WWConfigManager::getInstance()->getValue(GAME_VERSION)->getCString();
	std::string sVersion = GAMECONFIG.m_game_version;

	if (buff_contain_origin.length()!=0 && strcmp(buff_contain_origin.c_str(), sVersion.c_str()) != 0)
	{
        if(m_loading)
        {
            WWSceneManager::getInstance()->closeGlobalUI(m_loading);
        }
		//提示更新版本  确定按钮调用 GameActivity里面的openUrl 传一个url 进去
		//c++ 掉 java
		m_dialogLayer = DialogLayer::create(1, GetGameString("update_game"));
		this->addChild(m_dialogLayer);
		auto button = dynamic_cast<Button*>(m_dialogLayer->m_systemLayout->getChildByName("Button_Ture_0"));
		button->addTouchEventListener(CC_CALLBACK_2(QLoginScene::touchUpdateEvent, this));
		auto text = dynamic_cast<Text*>(m_dialogLayer->m_systemLayout->getChildByName("Text_Zhu"));
		if (text) {
			text->setVisible(true);
		}
		log("version update!!!");
	}
	else
	{
		if (m_loading)
		{
			//转圈圈界面消失
			WWSceneManager::getInstance()->closeGlobalUI(m_loading);
		}
	}
}

void QLoginScene::onDataReceiver(int msgType, Ref* objectData)
{
	switch (msgType)
	{
		case MSG_LOGON_TYPE:
		{
			QLoginManager::getInstance()->dealGetType(objectData);
			break;
		}
		case MSG_LOGON_GETWAY:
		{
			QLoginManager::getInstance()->dealGetWay(objectData);
			break;
		}
	}
}

void QLoginScene::touchExitEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	WWSceneManager::getInstance()->closeAllUI();
	LordNetManger::getInstance()->onDisconnect(WWNetwork::eLoginNet);
	exit(0);
}

void QLoginScene::touchUpdateEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	//m_dialogLayer->removeFromParentAndCleanup(true);
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	Application::getInstance()->openURL(GAMECONFIG.m_sHttpApk.c_str());
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	Application::getInstance()->openURL(GAMECONFIG.m_sHttpIpa.c_str());
#endif
}

