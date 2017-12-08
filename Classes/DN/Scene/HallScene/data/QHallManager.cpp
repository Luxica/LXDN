#include "QHallManager.h"
#include "../../../../net/LordNetManger.h"
#include "../../../../NetSystem/ww-net.h"
#include "../../../../GUISystem/WWSceneManager.h"
#include "../../../Scene/GlobalLayer/Load.h"
#include "../QHallScene.h"
#include "../../QDNEvent.h"
#include "../../../Scene/GlobalLayer/PopLayer.h"
#include "../../../Scene/GlobalLayer/Prompt.h"
#include "../../../Scene/LoginScene/QLoginLayer.h"
#include "GameConfig.h"
#include "ScriptDataManager.h"
#include "../../../Comm/request/MsgHallRequest.h"
#include "GameConfig.h"
#include "../../../GlobalDefines.h"
#include "../../LoginScene/layer/LoginLoad.h"
#include "../../GlobalLayer/DialogLayer.h"
#include "QHallBaseData.h"

QHallManager* QHallManager::m_pInstance = nullptr;
QHallManager::QHallManager()
	:m_pRankData_(nullptr)
	, m_vipType(K_NO_VIP)
{
	m_sGameIp_ = "";
	m_nGamePort_ = 0;
	m_sAccount_ = "";
	m_sPassword_ = "";
    m_sNickName = "";
    m_sHeadUrl = "";
    m_cSex = 0;
	m_nUserId_ = 0;
	m_nRoomNum_ = 0;
	m_nRoomCard_ = 0;
	m_nUserType_ = 0;
	m_cPlayMode_ = 5;
	m_cZMode_ = 4;
	m_cCostMode_ = 0;
	m_cGameMode_ = 0;
	m_cMMode_ = 0;
	m_cKcardWay_ = 1;
	m_cMaxBet_ = 10;
	m_cMinCard_ = 0;
	m_cHalf_ = 0;
	m_cForbid_ = 0;
	m_cJoinType_ = 0;

	m_cGameType_ = 1;
	m_cTakeType_ = 0;

	m_nKickRoomNum_ = 0;
	m_nWarnRoomNum_ = 0;
	m_nInsteadRoomNum_ = 0;

	m_curAddress_ = "";

	m_nCostNum_ = 0;

	m_bIsFirst = true;

	m_curNum = cocos2d::random(1500, 2000);
	if (((int)cocos2d::random(0, 100)) % 2 == 0) {
		m_curNum += cocos2d::random(1, cocos2d::random(100, 800));
	} else {
		m_curNum -= cocos2d::random(1, cocos2d::random(100, 800));
	}

	m_pRankData_ = QRankData::create();
	m_pRankData_->retain();
}

QHallManager::~QHallManager()
{
	CC_SAFE_RELEASE_NULL(m_pRankData_);
	m_vecRecord_.clear();
	m_VecNotice.clear();
}

void QHallManager::dealLogin(Ref *obj)
{
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)
		return;
	m_nUserId_ = pBuff->readUInt();
	auto roomCard = pBuff->readUInt();
	m_nUserType_ = pBuff->readUInt();
	QHallManager::getInstance()->setRoomCard(roomCard);
	log("m_nUserId_===%d,m_nRoomCard_===%d", m_nUserId_, m_nRoomCard_);
	if (m_nUserId_ != 0) 
	{
		MsgHallRequest::sendAskRoom();
        if (GAMECONFIG.m_bNetTest == true)
        {
            //WWSceneManager::getInstance()->openScene(QHallScene::create());
            //WWSceneManager::getInstance()->closeGlobalUIByName("loginload");
        }else
        {
            //上传微信信息
			MsgHallRequest::sendSetInfo(QHallManager::getInstance()->getNickName().c_str(), QHallManager::getInstance()->getHeadUrl().c_str(), QHallManager::getInstance()->getSex(), QHallManager::getInstance()->getCurAddress().c_str());
        }
	}
	else
	{
		//提示登录失败 等待界面消失   (弹窗)
		//弹窗
		PopLayer* popup = PopLayer::create();
		popup->setContentText(GetGameString("loginFail"), 30, 800, 500);
		Director::getInstance()->getRunningScene()->addChild(popup);
        WWSceneManager::getInstance()->closeGlobalUIByName("loginload");
	}
	
}

void QHallManager::dealHallEnterRoom(Ref *obj)
{
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)
		return;
	unsigned char cType = pBuff->readUChar();
	//if (cType != 1) return;
	m_sGameIp_    = pBuff->readLengthAndString();
	m_nGamePort_  = pBuff->readUShort();
	log("m_sGameIp_===%s,m_nGamePort_===%d", m_sGameIp_.c_str(), m_nGamePort_);
	if (m_sGameIp_.length() != 0)
	{ 
		LordNetManger::getInstance()->connectNet(WWNetwork::eGameNet);
	}
	else
	{
		//加入或创建失败 提示    （弹窗）
		switch (getJoinType())
		{
		case 0:
		{			
			//弹窗
			PopLayer* popup = PopLayer::create();
			popup->setContentText(GetGameString("creatFail"), 30, 800, 500);
			Director::getInstance()->getRunningScene()->addChild(popup,100);

			LordNetManger::getInstance()->onDisconnect(WWNetwork::eGameNet);
			Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_ENTEROOM_FILURE);
		}
			break;
		case 1:
		{
				
			//弹窗
			PopLayer* popup = PopLayer::create();
			popup->setContentText(GetGameString("enterFail"), 30, 800, 500);
			Director::getInstance()->getRunningScene()->addChild(popup,100);

			LordNetManger::getInstance()->onDisconnect(WWNetwork::eGameNet);
			Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_ENTEROOM_FILURE);
		}
			break;
		case 2:
		{			
			Prompt* prompt = Prompt::create(2, GetGameString("insteadFail"));
			Director::getInstance()->getRunningScene()->addChild(prompt, 100);
			LordNetManger::getInstance()->onDisconnect(WWNetwork::eGameNet);
			Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_ENTEROOM_FILURE);
			
		}
			break;
		default:
			break;
		}
		
		
	}
	WWSceneManager::getInstance()->closeGlobalUIByName("load");
}

void QHallManager::dealSetInfo(Ref *obj)
{
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)
		return;
	unsigned char cResult = pBuff->readUChar();	// 0为成功，非0失败
    if (cResult == 0)
    {
        /*if (Director::getInstance()->getRunningScene()->getName() != "QHallScene") {
            WWSceneManager::getInstance()->openScene(QHallScene::create());
            WWSceneManager::getInstance()->closeGlobalUIByName("loginload");
        }*/
    }
}

void QHallManager::dealGetInfo(Ref *obj)
{
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)
		return;

	QVXUser *qUser = QVXUser::create();
	qUser->setUserId(pBuff->readUInt());
	qUser->setNickName(pBuff->readLengthAndString());
	qUser->setImageName(pBuff->readLengthAndString());
	qUser->setSex(pBuff->readUChar());
	qUser->setStrIP(pBuff->readLengthAndString());
}


void QHallManager::dealAskRoom(Ref *obj)
{
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)
		return;
	unsigned char iGameType = pBuff->readUChar();
	m_sGameIp_ = pBuff->readLengthAndString();
	m_nGamePort_ = pBuff->readUShort();
	if (iGameType != 1 && m_sGameIp_.length() != 0)
	{
		WWSceneManager::getInstance()->openGlobalUI(DialogLayer::create(2, GetGameString("warring")));
		return;
	}
	if (m_sGameIp_.length() != 0)
	{
		m_cBackType_ = 1;
		//m_bIsFirst = false;
		//表示是重连的
		log("dealAskRoom.....");
		QHallManager::getInstance()->setIsFirst(false);
		if (!LordNetManger::getInstance()->isConnect(WWNetwork::eGameNet)) {
			LordNetManger::getInstance()->connectNet(WWNetwork::eGameNet);
		}
	}
	else
	{
		m_cBackType_ = 0;
		if (Director::getInstance()->getRunningScene()->getName() != "QHallScene") {
			WWSceneManager::getInstance()->openScene(QHallScene::create());
			WWSceneManager::getInstance()->closeGlobalUIByName("loginload");
		}
	}
	//取消转圈圈的
}

void QHallManager::dealGetRank(Ref *obj)
{
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)
		return;
	m_pRankData_->getRankUserVec().clear();

	m_pRankData_->setRankType(pBuff->readUChar());
	unsigned int iNum = pBuff->readUInt();
	QRankUser *qUser = nullptr;
	for (int i = 0; i < iNum; i++)
	{
		qUser = QRankUser::create();
		qUser->setUserId(pBuff->readUInt());
		qUser->setNickName(pBuff->readLengthAndString());
		qUser->setImageName(pBuff->readLengthAndString());
		qUser->setScore(pBuff->readInt());
		qUser->setWinCount(pBuff->readUInt());
		qUser->setLoseCount(pBuff->readUInt());
		m_pRankData_->getRankUserVec().pushBack(qUser);
	}

	//监听排行榜数据
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_RANK);
}

void QHallManager::dealGetRecord(Ref *obj)
{
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)
		return;

	m_vecRecord_.clear();
	QRecordData *pRecord = nullptr;
	QScoreData *pData = nullptr;
	unsigned int iCount = pBuff->readUInt();
	for (int i = 0; i < iCount; i++)
	{
		pRecord = QRecordData::create();
		pRecord->setRecdID(pBuff->readUInt());
		pRecord->setRoomNum(pBuff->readUInt());
		pRecord->setHostID(pBuff->readUInt());
		pRecord->setGameType(pBuff->readUChar());
		pRecord->setRType(pBuff->readUChar());
		//std::string m_strRoom = pBuff->readLengthAndString();
		/*WWBuffer buffRoom;
		buffRoom.writeData(m_strRoom.data(), m_strRoom.size());*/
		WWBuffer *buffRoom = pBuff->readLengthAndData();
		pRecord->getVXRoom()->setPlayMode(buffRoom->readUChar());
		pRecord->getVXRoom()->setZMode(buffRoom->readUChar());
		pRecord->getVXRoom()->setPayMode(buffRoom->readUChar());
		pRecord->getVXRoom()->setGameMode(buffRoom->readUChar());
		pRecord->getVXRoom()->setMMode(buffRoom->readUChar());
		pRecord->getVXRoom()->setCover(buffRoom->readUChar());
		pRecord->getVXRoom()->setMaxBet(buffRoom->readUChar());
		pRecord->getVXRoom()->setMinCard(buffRoom->readUInt());
		pRecord->getVXRoom()->setHalf(buffRoom->readUChar());
		pRecord->getVXRoom()->setForbid(buffRoom->readUChar());
		unsigned int iNum = pBuff->readUInt();
		for (int i = 0; i < iNum; i++)
		{
			pData = QScoreData::create();
			pData->setUserId(pBuff->readUInt());
			pData->setNickName(pBuff->readLengthAndString());
			pData->setHeadImage(pBuff->readLengthAndString());
			pData->setScore(pBuff->readInt());
			pRecord->getScoreVec().pushBack(pData);
		}
		pRecord->setRecordTime(pBuff->readLengthAndString());
		m_vecRecord_.pushBack(pRecord);
	}
	
	//记录战绩数据
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_RECORD);
}

void QHallManager::dealNotice(Ref *obj)
{
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)
		return;
	m_VecNotice.clear();
	unsigned int iNum = pBuff->readUInt();
	for (int i = 0; i < iNum; i++)
	{
		m_VecNotice.push_back(pBuff->readLengthAndString());
	}
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_NOTICE);
}

void QHallManager::dealPayResult(Ref *obj)
{
    WWBuffer *pBuff = (WWBuffer*)obj;
    if (pBuff == nullptr)
        return;
    
    unsigned char goodId = pBuff->readUChar();
    unsigned int addCard = pBuff->readInt();
    int currentCard = QHallManager::getInstance()->getRoomCard();

    QHallManager::getInstance()->setRoomCard(currentCard + addCard);
    QHallScene* hallScene = dynamic_cast<QHallScene*>(Director::getInstance()->getRunningScene()) ;
    if (hallScene != nullptr)
    {
        hallScene->setHallRoomCard();
    }
    
}
void QHallManager::dealBindCode(Ref *obj)
{
    WWBuffer *pBuff = (WWBuffer*)obj;
    if (pBuff == nullptr)
        return;
	PopLayer* popup = PopLayer::create();
    int iCard = pBuff->readInt();
	if (iCard >= 0)
    {
        QHallManager::getInstance()->setUserType(2);
		popup->setContentText(GetGameString("bindSuccess"), 40, 800, 500);

		int currentCard = QHallManager::getInstance()->getRoomCard();
		QHallManager::getInstance()->setRoomCard(currentCard + iCard);
		QHallScene* hallScene = dynamic_cast<QHallScene*>(Director::getInstance()->getRunningScene());
		if (hallScene != nullptr)
		{
			hallScene->setHallRoomCard();
			hallScene->setInviteShow();
			hallScene->showShop();
		}
    }
	else
	{
		popup->setContentText(GetGameString("bindFail"), 20, 800, 500);
	}
	Director::getInstance()->getRunningScene()->addChild(popup);
    WWSceneManager::getInstance()->closeGlobalUIByName("loginload");

}
void QHallManager::dealCanShare(Ref *obj)
{
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)
		return;
	int isCanShare = pBuff->readInt();
	cocos2d::log("QHallManager::dealCanShare : %d", isCanShare);
	QHallScene* hallScene = dynamic_cast<QHallScene*>(Director::getInstance()->getRunningScene());
	if (hallScene != nullptr)
	{
		cocos2d::log("QHallManager::dealCanShare ------ A");
		if (isCanShare == 0)
		{
			hallScene->setHint(false);
		}
		else
		{
			hallScene->setHint(true);
		}
	}
}

void QHallManager::dealDayShare(Ref *obj)
{
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)
		return;
	int iCard = pBuff->readInt();
	cocos2d::log("QHallManager::dealDayShare : %d", iCard);
	if (iCard > 0)
	{
		int currentCard = QHallManager::getInstance()->getRoomCard();
		QHallManager::getInstance()->setRoomCard(currentCard + iCard);
		QHallScene* hallScene = dynamic_cast<QHallScene*>(Director::getInstance()->getRunningScene());
		if (hallScene != nullptr)
		{
			hallScene->setHallRoomCard();
		}
	}
}

void QHallManager::dealGetOnline(Ref *obj)
{
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)
		return;
	unsigned int onlineNum = pBuff->readInt();
	unsigned int tempCurNum = cocos2d::random(1500, 2000);
	onlineNum = onlineNum > tempCurNum ? onlineNum : tempCurNum;
	if (onlineNum == m_curNum) {
		if (((int)cocos2d::random(0, 100)) % 2 == 0) {
			onlineNum += cocos2d::random(1, cocos2d::random(100, 800));
		} else {
			onlineNum -= cocos2d::random(1, cocos2d::random(100, 800));
		}
	}
	setCurNum(onlineNum);
	QHallScene* hallScene = dynamic_cast<QHallScene*>(Director::getInstance()->getRunningScene());
	if (hallScene)
	{
		hallScene->updateOnlineNum(getCurNum());
	}
}

void QHallManager::dealGetTake(Ref *obj)
{
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)
		return;

	m_vecTARoom_.clear();
	QTARoom *pTARoom = nullptr;
	QVXUser *pVXUser = nullptr;
	m_cGameType_ = pBuff->readUChar();
	m_cTakeType_ = pBuff->readUChar();
	unsigned int iCount = pBuff->readUInt();
	for (int i = 0; i < iCount; i++)
	{
		pTARoom = QTARoom::create();
		pTARoom->setRoomNum(pBuff->readUInt());
		pTARoom->setTakeId(pBuff->readUInt());
		pTARoom->setGameType(pBuff->readUChar());
		pTARoom->setRoomState(pBuff->readChar());
		pTARoom->setStrTime(pBuff->readLengthAndString());
		WWBuffer *buffRoom = pBuff->readLengthAndData();
		pTARoom->getVXRoom()->setPlayMode(buffRoom->readUChar());
		pTARoom->getVXRoom()->setZMode(buffRoom->readUChar());
		pTARoom->getVXRoom()->setPayMode(buffRoom->readUChar());
		pTARoom->getVXRoom()->setGameMode(buffRoom->readUChar());
		pTARoom->getVXRoom()->setMMode(buffRoom->readUChar());
		pTARoom->getVXRoom()->setCover(buffRoom->readUChar());
		pTARoom->getVXRoom()->setMaxBet(buffRoom->readUChar());
		pTARoom->getVXRoom()->setMinCard(buffRoom->readUInt());
		pTARoom->getVXRoom()->setHalf(buffRoom->readUChar());
		pTARoom->getVXRoom()->setForbid(buffRoom->readUChar());
		unsigned int iNum = pBuff->readUInt();
		for (int i = 0; i < iNum; i++)
		{
			pVXUser = QVXUser::create();
			pVXUser->setUserId(pBuff->readUInt());
			pVXUser->setNickName(pBuff->readLengthAndString());
			pVXUser->setImageName(pBuff->readLengthAndString());
			pVXUser->setSex(pBuff->readUChar());
			pVXUser->setStrIP(pBuff->readLengthAndString());
			pTARoom->getUserVec().pushBack(pVXUser);
		}
		m_vecTARoom_.pushBack(pTARoom);
	}

	//记录代开房数据
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_INSTEAD);
}

void QHallManager::updateUserCard()
{
	QHallScene* hallScene = dynamic_cast<QHallScene*>(Director::getInstance()->getRunningScene());
	if (hallScene != nullptr)
	{
		hallScene->setHallRoomCard();
	}
}

void QHallManager::updateUserVip() {
	
}

void QHallManager::dealFbkSubmit(Ref *obj)
{
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)
		return;
	unsigned char cResult = pBuff->readUChar();
	EventCustom event(EVENT_FEEDBACK);
	event.setUserData((void*)cResult);
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}
