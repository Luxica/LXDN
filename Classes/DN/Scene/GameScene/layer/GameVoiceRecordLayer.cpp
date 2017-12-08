#include "AppDelegate.h"
#include "GameAudioEngine.h"
#include "AdmobJni.h"
#include "../../../Comm/request/MsgGameRequest.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "iOSAdapterUtility.hpp"
#endif

#include "GameVoiceRecordLayer.h"
#include "ScriptDataManager.h"
#include "../../GlobalLayer/PopLayer.h"
#include "../../GameScene/QGameLayer.h"
#include "StringTool.h"

GameVoiceRecordLayer::GameVoiceRecordLayer()
{

}

GameVoiceRecordLayer::~GameVoiceRecordLayer()
{

}

void GameVoiceRecordLayer::showDialog(Node *parent)
{
    m_pParent = parent;
    auto voiceLayer = m_pParent->getChildByName("voice");
    if (voiceLayer == nullptr)
    {
        m_pParent->addChild(this);
    }
}

bool GameVoiceRecordLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	m_pRootNode = CSLoader::createNode("GameVoiceRecordLayer.csb");
	addChild(m_pRootNode);

	m_pLdpVoice = static_cast<LoadingBar*>(m_pRootNode->getChildByName("ldp_voice"));

	gettimeofday(&m_tvBegin, NULL);

	GameAudioEngine::getInstance()->pauseBackgroundMusic();

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	AdmobJni::startRecord();
#endif
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    iOSAdapterUtility::getInstance()->startRecord();
#endif

	schedule(CC_SCHEDULE_SELECTOR(GameVoiceRecordLayer::updateVoiceDb), 0.1f);

	return true;
}


void GameVoiceRecordLayer::onExit()
{
	Layer::onExit();
}

void GameVoiceRecordLayer::stopRecord()
{
	GameAudioEngine::getInstance()->resumeBackgroundMusic();

	struct timeval	tvEnd;
	gettimeofday(&tvEnd, NULL);
	m_recordTime = tvEnd.tv_sec * 1000L + tvEnd.tv_usec / 1000L - m_tvBegin.tv_sec * 1000L - m_tvBegin.tv_usec / 1000L;

	if (m_recordTime < 1000 || m_recordTime > 1000 * 30)
	{
        
        auto loginFail = ScriptDataManager::getInterface()->getGameString("notEnoughTime");
        //弹窗
        PopLayer* popup = PopLayer::create();
		if (popup)
		{
			popup->setContentText(loginFail.c_str(), 30, 800, 500);
			Director::getInstance()->getRunningScene()->addChild(popup);
		}
        
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
		AdmobJni::cancelRecord();
#endif
        
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
        iOSAdapterUtility::getInstance()->cancelRecord();
#endif
		removeFromParent();

		return;
	}

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	QGameLayer * qgameLayer = dynamic_cast<QGameLayer*>(m_pParent);
	qgameLayer->setSpeekTime();
	removeFromParent();
	return;
#endif

	std::string strVoiceData;

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	strVoiceData = AdmobJni::stopRecord();
#endif
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    strVoiceData = iOSAdapterUtility::getInstance()->stopRecord();
#endif
    if (strVoiceData == "" || strVoiceData.length() == 0)
        return;
       //iOSAdapterUtility::getInstance()->playRecord(strVoiceData.c_str(), 20);
    
    MsgGameRequest::sendChatVoice(strVoiceData.c_str(),m_recordTime);
    QGameLayer * gameLayer = dynamic_cast<QGameLayer*>(m_pParent);
	if (gameLayer)
	{
		gameLayer->setSpeekTime();
	}
	removeFromParent();
}


void GameVoiceRecordLayer::cancelRecord()
{
	GameAudioEngine::getInstance()->resumeBackgroundMusic();

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	AdmobJni::cancelRecord();
#endif
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    iOSAdapterUtility::getInstance()->cancelRecord();
#endif

	removeFromParent();
}

void GameVoiceRecordLayer::updateVoiceDb(float dt)
{
	int db = 0;

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	db = AdmobJni::getVoiceDb();
#endif
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    db = iOSAdapterUtility::getInstance()->getVoiceDb();
#endif

	if (db > 100)
	{
		db = 100;
	}
	if (db < 0)
	{
		db = 0;
	}

	m_pLdpVoice->setPercent(db);
}
