#include "LordNetManger.h"
#include "LordNetConstants.h"
#include "cocos2d.h"
#include "StringTool.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#include "ww-config.h"
#else
#include "ConfigSystem/ww-config.h"
#endif
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#include "ww-net.h"
#else
#include "NetSystem/ww-net.h"
#endif
#include "../DN/Scene/LoginScene/data/QLoginManager.h"
#include "../DN/Scene/HallScene/data/QHallManager.h"
#include "../DN/Scene/HallScene/QHallScene.h"
#include "../DN/Comm/request/MsgLogonRequest.h"
#include "../DN/Comm/request/MsgHallRequest.h"
#include "../DN/Comm/request/MsgGameRequest.h"
#include "../DN/Scene/GlobalLayer/DialogLayer.h"
#include "../DN/Scene/LoginScene/QLoginScene.h"
#include "ScriptDataManager.h"
#include "../GUISystem/WWSceneManager.h"
#include "LordNetConstants.h"
#include <SimpleAudioEngine.h>

#include "GameConfig.h"

using namespace cocos2d;

static const std::string DIS_CONNECT_LAYER_MASK = "ConnectLayer_DisType_disconnect";
const char *const kKeyForServerIP = "key_parallel_ip";
const char *const kkeyForServerPort = "key_paralle_port";
static const char *const keyNewSocketUrl = "newSocketUrl";

LordNetManger* LordNetManger::getInstance()
{
	static LordNetManger instance;
    return &instance;
}

LordNetManger::LordNetManger() : 
	m_hadShowDisConnetLayer(false)
{
	m_pLoginNetwork_ = new WWNetwork(WWNetwork::eLoginNet);
	m_pHallNetwork_ = new WWNetwork(WWNetwork::eHallNet);
	m_pGameNetwork_ = new WWNetwork(WWNetwork::eGameNet);
	
	//m_pGameNetwork_->setNewSocketUrl(WWConfigManager::getInstance()->getValue(keyNewSocketUrl)->getCString()); //ycc net
	m_pLoginNetwork_->setNewSocketUrl("");
	m_pHallNetwork_->setNewSocketUrl("");
	m_pGameNetwork_->setNewSocketUrl("");

	auto  eventDispatcher = Director::getInstance()->getEventDispatcher();

	auto pListener = EventListenerCustom::create(CONNECT_FAIL, CC_CALLBACK_1(LordNetManger::updateConnectEvent, this));
	eventDispatcher->addEventListenerWithFixedPriority(pListener, -1);

	pListener = EventListenerCustom::create("CONNECTRE_SUCCESS", CC_CALLBACK_1(LordNetManger::onConnectEvent, this));
	eventDispatcher->addEventListenerWithFixedPriority(pListener, -1);
}

LordNetManger::~LordNetManger()
{
	CC_SAFE_RELEASE_NULL(m_pLoginNetwork_);
	CC_SAFE_RELEASE_NULL(m_pHallNetwork_);
	CC_SAFE_RELEASE_NULL(m_pGameNetwork_);

	auto  eventDispatcher = Director::getInstance()->getEventDispatcher();
	eventDispatcher->removeCustomEventListeners(CONNECT_FAIL);
	eventDispatcher->removeCustomEventListeners("CONNECTRE_SUCCESS");
}

void LordNetManger::send(const int &iType,WWBuffer* buf)
{
	auto pNet = getNetWorkObj(iType);
	if (pNet)
		pNet->send(buf);
}

void LordNetManger::sendMessageNow(const int &iType,WWBuffer* buf)
{
	auto pNet = getNetWorkObj(iType);
	if (pNet)
		pNet->sendMessageNow(buf);
}

WWNetwork *LordNetManger::getNetWorkObj(const int &iType)
{
	if (iType == WWNetwork::eLoginNet)
		return m_pLoginNetwork_;
	else if (iType == WWNetwork::eHallNet)
		return m_pHallNetwork_;
	else if (iType == WWNetwork::eGameNet)
		return m_pGameNetwork_;

	return nullptr;
}

void LordNetManger::onConnectEvent(EventCustom* event) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    long iNetType = reinterpret_cast<long>(event->getUserData());
#else
    int iNetType = reinterpret_cast<int>(event->getUserData());
#endif
	if (iNetType == WWNetwork::eLoginNet)
		MsgLogonRequest::sendGetWay();
	else if (iNetType == WWNetwork::eHallNet) {
		std::string sAccount = QHallManager::getInstance()->getAccount();
		std::string sPassword = QHallManager::getInstance()->getPassword();
		MsgHallRequest::sendLogin(sAccount.c_str(), sPassword.c_str());
	}
	else if (iNetType == WWNetwork::eGameNet) {
		std::string sPassword = QHallManager::getInstance()->getPassword();
		MsgHallRequest::sendLoginGame(QHallManager::getInstance()->getUserId(), sPassword.c_str());
	}
}

void LordNetManger::updateConnectEvent(EventCustom* event)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    long iType = reinterpret_cast<long>(event->getUserData());
#else
    int iType = reinterpret_cast<int>(event->getUserData());
#endif
	if (!m_hadShowDisConnetLayer) {
		if (GAMECONFIG.m_nAppType == GAME_TYPE_KXNNL)
		{
			if (iType != WWNetwork::eLoginNet)
			{
				auto curScene = Director::getInstance()->getRunningScene();
				auto m_exitRoom = DialogLayer::create(1, GetGameString("lose_webFail"));
				curScene->addChild(m_exitRoom, 1);
				WWSceneManager::getInstance()->closeGlobalUIByName("load");
				auto agreeExit = dynamic_cast<Button*>(m_exitRoom->m_systemLayout->getChildByName("Button_Ture"));
				if (agreeExit)
				{
					agreeExit->addTouchEventListener(CC_CALLBACK_2(LordNetManger::touchAgreeEvent, this));
				}
			}
		}

		m_hadShowDisConnetLayer = true;
	}
}

void LordNetManger::touchAgreeEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
		return;
	WWSceneManager::getInstance()->closeGlobalUIByName("load");
	WWSceneManager::getInstance()->closeAllUI();
	WWSceneManager::getInstance()->openScene(QLoginScene::create());
	
}

void LordNetManger::connectNet(const int &iNetType)
{
 	connect(iNetType);		
}

bool LordNetManger::isConnect(const int &iNetType) {
	auto pNet = getNetWorkObj(iNetType);
	if (pNet && pNet->isConnected()) {
		return true;
	}
	return false;
}

void LordNetManger::connect(const int &iNetType)
{
	auto pNet = getNetWorkObj(iNetType);
	if (pNet && pNet->isConnected())
	{
		CCLOG("LordNetManger::connect had already connected! don't need do it again!!==%d", iNetType);
		return;
	}
	
	std::vector<std::string> ips;
	std::vector<int> ports;


	if (iNetType == WWNetwork::eLoginNet)
	{
		//ips.push_back(std::string(WWConfigManager::getInstance()->getValue(g_domain_key)->getCString()));
		ips.push_back(GAMECONFIG.m_domain_key.c_str());
		//ports.push_back(WWConfigManager::getInstance()->getValue(g_ip_ports)->intValue());
		ports.push_back(GAMECONFIG.m_socket_ip_ports);
	}
	else if (iNetType == WWNetwork::eHallNet)
	{
		ips.push_back(QLoginManager::getInstance()->getHallIp());
		ports.push_back(QLoginManager::getInstance()->getHallPort());
	}
	else if (iNetType == WWNetwork::eGameNet)
	{
		ips.push_back(QHallManager::getInstance()->getGameIp());
		ports.push_back(QHallManager::getInstance()->getGamePort());
	}
		
	std::vector< WWInetAddress > inetAddrs = genInetAddrs(ips, ports);
	pNet->setDelegate(this);
	pNet->parallelConnect(inetAddrs);

	m_hadShowDisConnetLayer = false;
}

void LordNetManger::onDisconnect(const int &iNetType)
{
	auto pNet = getNetWorkObj(iNetType);
	if (pNet)
	{
		pNet->setReconnect(false);
		pNet->disconnect();
	}
}

std::vector< WWInetAddress > LordNetManger::genInetAddrs(const std::vector< std::string > &ips, const std::vector< int > &ports)
{
	std::vector< WWInetAddress > ret;
    
	for (auto &ip : ips)
    {
		WWInetAddress inetAddr;
		auto hostInfo = gethostbyname(ip.c_str());
		if (hostInfo)
        {
			inetAddr.setIp(inet_ntoa(*((struct in_addr *)hostInfo->h_addr)));
		}
        else
        {
			inetAddr.setIp(ip.c_str());
		}

		for (auto &port : ports)
        {
			WWInetAddress inetItem;
			inetItem.setIp(inetAddr.getIp());
			inetItem.setPort(port);
			ret.push_back(inetItem);
		}
	}

	return ret;
}

void LordNetManger::setReconnect(const int &iNetType,bool isReconnect)
{
	auto pNet = getNetWorkObj(iNetType);
	if (pNet)
	{
		pNet->setReconnect(false);
	}
}

void LordNetManger::reConnect(const int &iNetType, float delayTime)
{
	int iType = iNetType;
    const std::string reConnectMask = "LordNetManager::reconnect";
    Scheduler *scheduler = Director::getInstance()->getScheduler();
    
    if (scheduler->isScheduled(reConnectMask ,this)) {
        CCLOGINFO("roconect scheduler.");
        return ;
    }
    
    if (delayTime < 0.000001f) {
        delayTime = 0.f;
    }
    scheduler->schedule([=](float dt) {
		connect(iType);
    }, this, 0.f, 0, delayTime, false, reConnectMask);
}

WWBuffer *LordNetManger::onRequestLogonInfo()
{
	//auto userId = UserData::getInstance()->getUserID();
	return nullptr;
	CCLOG("onRequestLogonInfo");
}

void LordNetManger::onExceptionCaught(WWNetStatus statue)
{
	switch (statue) {
		case NetStatueClose:
		{
			const char *hostaddr = m_pGameNetwork_->getInetAddress().getHostAddress();
			CCLOG("##############net disconnect onExceptionCaught addr=%s ######################", hostaddr);
			//糟糕，好像网络断掉了！
			//if (!m_hadShowDisConnetLayer && m_pGameNetwork_->getExitStatus() != eSocketDisconnected)
   //         {
			//	//ycc net
   //         	/*LoadingManager::getInstance()->endLoading();
   //         	if(!(DataCenter::getInstance()->nowWhichScene == e_scene_game && GameUtil::isAloneGame()))
   //         	{
   //                 auto layer = NetDisConnectLayer::create(color, NetDisConnectLayer::eNetDisType_disconnet);
   //                 layer->setLocalZOrder(NET_DISCONNECT_LAYER_Z_ORDER);
   //                 layer->setName(DIS_CONNECT_LAYER_MASK);
   //                 WWSceneManager::getInstance()->openGlobalUI(layer);
			//		cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_NET_DISCONNECTED);
   //         	}
   //         	*/
   //             m_hadShowDisConnetLayer = true;
   //         }
		}
			break;
		case NetStatueLag:	//网络状态差
		{
			const char *hostaddr = m_pGameNetwork_->getInetAddress().getHostAddress();
			CCLOG("##############net bad onExceptionCaught addr=%s ######################", hostaddr);
			//WWToast::makeText(GetWWString(g_NetStatu_Lag_Str,g_NetStatu_SECTION).c_str(),2.0f); //ycc net
		}
			break;
		case NetStatueNoRsp:  //服务器无响应
		{
			//const char *hostaddr = m_pGameNetwork_->getInetAddress().getHostAddress();
			CCLOG("##############server not back onExceptionCaught ######################");
			//ycc net
           /* if (!m_hadShowDisConnetLayer && m_pGameNetwork_->getExitStatus() != eSocketDisconnected)
            {
            	LoadingManager::getInstance()->endLoading();
                auto layer = NetDisConnectLayer::create(color, NetDisConnectLayer::eNetDisType_no_respond);
                WWSceneManager::getInstance()->openGlobalUI(layer);
                m_hadShowDisConnetLayer = true;
            }
        	*/
		}
		break;
		case NetStatueHandSharingFialed:
		{
            //LoadingManager::getInstance()->endLoading();  //ycc net

			CCLOG("<LordNetManager | net_statue_hand_sharing_failded.");
			//m_pGameNetwork_->pushMsgFront(onRequestLogonInfo());
		}
		break;
		case NetGameCut:
		{
			//游戏服务器 断开之后返回大厅
			//WWSceneManager::getInstance()->openScene(QHallScene::create());
		}
		break;
		case NetHallCut:
		{
			connectNet(WWNetwork::eHallNet);
		}
		break;
		default:
			break;
	}
}

bool LordNetManger::gethadShowDisConnetLayer()
{
	return m_hadShowDisConnetLayer;
}

