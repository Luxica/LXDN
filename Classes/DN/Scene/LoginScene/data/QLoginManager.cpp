#include "QLoginManager.h"
#include "../../../../net/LordNetManger.h"
#include "../../../../NetSystem/ww-net.h"
#include "../../../Scene/GlobalLayer/PopLayer.h"
#include "../../../../GUISystem/WWSceneManager.h"
#include "ScriptDataManager.h"

QLoginManager::QLoginManager()
{
	m_isTourist_ = true;
	m_sHallIp_ = "";
	m_nHallPort_ = 0;
	m_sAccount_ = "";
	m_sPassWord_ = "";
    m_loginType = 0;
}

QLoginManager::~QLoginManager()
{

}

void QLoginManager::dealGetType(Ref *obj) {
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)
		return;
	m_isTourist_ = pBuff->readUChar() == 0;
}

void QLoginManager::dealGetWay(Ref *obj)
{
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)	
		return;
	m_sHallIp_ = pBuff->readLengthAndString();
	m_nHallPort_ = pBuff->readUShort();
	log("m_sHallIp_===%s,m_nHallPort_===%d", m_sHallIp_.c_str(), m_nHallPort_);
	if (m_sHallIp_.length() == 0)
	{
		//无法连接到服务器	 （弹窗）
		/*__Dictionary* m_Dictionary = __Dictionary::createWithContentsOfFile("XML/chinese.xml");
		auto disConnectSVKey = m_Dictionary->valueForKey("disConnectSV");
		auto disConnectSV = disConnectSVKey->getCString();
		*/
		//弹窗
		PopLayer* popup = PopLayer::create();
		popup->setContentText(GetGameString("disConnectSV"), 40, 800, 500);
		Director::getInstance()->getRunningScene()->addChild(popup);
	}
	else
	{
		LordNetManger::getInstance()->onDisconnect(WWNetwork::eLoginNet);
	}
}
