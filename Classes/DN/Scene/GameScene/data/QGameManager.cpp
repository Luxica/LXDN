#include "QGameManager.h"
#include "../../../../net/LordNetManger.h"
#include "../../../../NetSystem/ww-net.h"
#include "../../../../GUISystem/WWSceneManager.h"
#include "../../../Comm/request/MsgGameRequest.h"
#include "../../HallScene/data/QHallManager.h"
#include "../../HallScene/QHallScene.h"
#include "../../GlobalLayer/PopLayer.h"
#include "../QGameScene.h"
#include "../../QDNEvent.h"
#include "ScriptDataManager.h"
#include "../../../../DN/GlobalDefines.h"
#include "GameConfig.h"

QGameManager::QGameManager()
{
	m_nUserId_ = 0;
	m_nRoomNum_ = 0;
	m_cSeatId_ = 0;
	m_cGameStatues_ = 0;
	m_cReady_ = 0;
	m_strTime = "";
	m_pCardData_ = new QCardData();
	m_gotoRoomId = "";
}

QGameManager::~QGameManager()
{
    clearData();
    
    if (m_pCardData_) {
        delete m_pCardData_;
    }
    
}
void QGameManager::clearData(){
    m_nUserId_ = 0;
    m_nRoomNum_ = 0;
    m_cSeatId_ = 0;
    m_cGameStatues_ = 0;
	m_cReady_ = 0;
  
    m_pCardData_->clearData();
    
    m_vecOver_.clear();
    m_vecResult_.clear();
    m_vecUser_.clear();
    if (m_qGameBackData)
    {
        CC_SAFE_DELETE(m_qGameBackData);
    }
}

void QGameManager::dealLoginGame(Ref *obj)
{
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)
		return;
    QUserData *qUser = QUserData::create();
	m_nUserId_ = pBuff->readUInt();

	qUser->setUserId(m_nUserId_);
	qUser->setNickName(pBuff->readLengthAndString());
	qUser->setStrImage(pBuff->readLengthAndString());
	qUser->setSex(pBuff->readUChar());
	qUser->setStrIP(pBuff->readLengthAndString());
	qUser->setStrLocation(pBuff->readLengthAndString());
	qUser->setWinCount(pBuff->readUInt());
	qUser->setLoseCount(pBuff->readUInt());
	m_vecUser_.pushBack(qUser);

	int iCard = pBuff->readInt();
	unsigned int userType = pBuff->readUInt();
	//QHallManager::getInstance()->setRoomCard(iCard);
	log("--11111111--%d----", iCard);
	log("m_nUserId_===%d,iCard===%d", m_nUserId_, iCard);

	if (m_nUserId_ == 0) //登录失败
	{
		//登录服务器失败
		LordNetManger::getInstance()->onDisconnect(WWNetwork::eGameNet);
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_ENTEROOM_FILURE);
	}
	else
	{
		unsigned char cBackType = QHallManager::getInstance()->getBackType();
		if (cBackType == 1)
		{
			MsgGameRequest::sendBackRoom();
		}
		else
		{
			unsigned char cType = QHALLMGR->getJoinType();
			unsigned char cPlayMode = QHALLMGR->getPlayMode();
			unsigned char cZMode = QHALLMGR->getZMode();
			unsigned char cCostMode = QHALLMGR->getCostMode();
			unsigned char cGameMode = QHALLMGR->getGameMode();
			unsigned char cMMode = QHALLMGR->getMMode();
			unsigned char cCoverNum = QHALLMGR->getKcardWay();
			unsigned char cMaxBet = QHALLMGR->getMaxBet();
			unsigned int cMinCard = QHALLMGR->getMinCard();
			unsigned char cHalf = QHALLMGR->getHalf();
			unsigned char cForbid = QHALLMGR->getForbid();
			if (cType == 0)
			{
				MsgGameRequest::sendCreateRoom(cPlayMode, cZMode, cCostMode, cGameMode, cMMode, cCoverNum, cMaxBet, cMinCard, cHalf, cForbid);
			}
			else if (cType == 2)
			{
				MsgGameRequest::sendTakeOn(cPlayMode, cZMode, 1, cGameMode, cMMode, cCoverNum, cMaxBet, cMinCard, cHalf, cForbid);
			}
			else if (cType == 3)
			{
				unsigned int disRoomNum = QHallManager::getInstance()->getDisRoomNum();
				MsgGameRequest::sendTakeOff(disRoomNum);
			}
			else if (cType == 4)
			{
				unsigned int kickRoomNum = QHallManager::getInstance()->getKickRoomNum();
				unsigned char kickSeatID = QHALLMGR->getKickSeatID();
				MsgGameRequest::sendRoomKick(kickRoomNum, kickSeatID);
			}
			else if (cType == 5)
			{
				unsigned int warnRoomNum = QHallManager::getInstance()->getWarnRoomNum();
				MsgGameRequest::sendRoomWarn(warnRoomNum, "warn");
			}
			else
			{
				unsigned int iRoomId = QHallManager::getInstance()->getRoomNum();
				MsgGameRequest::sendJoinRoom(iRoomId);
			}
		}
	}
}
//自己进房
void QGameManager::dealEnterRoom(Ref *obj)
{
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)
		return;
	m_cSeatId_ = pBuff->readUChar();
	m_nRoomNum_ = pBuff->readUInt();
	
	unsigned char cPlayMode = pBuff->readUChar();
	unsigned char cZMode = pBuff->readUChar();
	unsigned char cCostMode = pBuff->readUChar();
	unsigned char cGameMode = pBuff->readUChar();
	unsigned char cMMode = pBuff->readUChar();
	unsigned char cCoverNum = pBuff->readUChar();
	unsigned char cMaxBet = pBuff->readUChar();
	unsigned int cMinCard = pBuff->readUInt();
	unsigned char cHalf = pBuff->readUChar();
	unsigned char cForbid = pBuff->readUChar();
	QHallManager::getInstance()->setPlayMode(cPlayMode);
	QHallManager::getInstance()->setZMode(cZMode);
	QHallManager::getInstance()->setCostMode(cCostMode);
	QHallManager::getInstance()->setGameMode(cGameMode);
	QHallManager::getInstance()->setMMode(cMMode);
	QHallManager::getInstance()->setKcardWay(cCoverNum);
	QHallManager::getInstance()->setMaxBet(cMaxBet);
	QHallManager::getInstance()->setMinCard(cMinCard);
	QHallManager::getInstance()->setHalf(cHalf);
	QHallManager::getInstance()->setForbid(cForbid);
	if (m_cSeatId_ >= 0 && m_cSeatId_ <= GAMECONFIG.m_max_num - 1)
	{
		int iCount = m_vecUser_.size();
		QUserData *qUser = nullptr;
		for (int i = 0; i < iCount; i++)
		{
			qUser = m_vecUser_.at(i);
			if (qUser->getUserId() == m_nUserId_)
				qUser->setSeatId(m_cSeatId_);
		}

		if (Director::getInstance()->getRunningScene()->getName() != "QGameScene") {
			WWSceneManager::getInstance()->openScene(QGameScene::create());
			MsgGameRequest::sendReady();
			WWSceneManager::getInstance()->closeGlobalUIByName("loginload");
		}
	}
	else
	{
		char joinFail[100] = { 0 };
		int index = 0;
		if (m_cSeatId_ == (unsigned char)-1)
			index = 1;
		else if (m_cSeatId_ == (unsigned char)-2)
			index = 2;
		else if (m_cSeatId_ == (unsigned char)-3)
			index = 3;
		else if (m_cSeatId_ == (unsigned char)-4)
			index = 4;
		else
			index = 0;
		sprintf(joinFail, "join_login_%d",index);
		log("join room failure!");
		//加入或创建失败 提示
		LordNetManger::getInstance()->onDisconnect(WWNetwork::eGameNet);
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_ENTEROOM_FILURE);

		//弹窗
		PopLayer* popup = PopLayer::create();
		popup->setContentText(GetGameString(joinFail), 30, 800, 500);
		Director::getInstance()->getRunningScene()->addChild(popup);
	}
}

void QGameManager::dealJoinRoom(Ref *obj)
{
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)
		return;

	QUserData *qUser = new QUserData();
	qUser->setSeatId(pBuff->readUChar());
	qUser->setUserId(pBuff->readUInt());
	qUser->setNickName(pBuff->readLengthAndString());
	qUser->setStrImage(pBuff->readLengthAndString());
	qUser->setSex(pBuff->readUChar());
	qUser->setStrIP(pBuff->readLengthAndString());
	qUser->setStrLocation(pBuff->readLengthAndString());
	qUser->setWinCount(pBuff->readUInt());
	qUser->setLoseCount(pBuff->readUInt());

	m_vecUser_.pushBack(qUser);
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_JOINROOM);
}

//别的玩家
void QGameManager::dealUserLost(Ref *obj)
{
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)
		return;
	unsigned char cSeatId_ = pBuff->readUChar();
	//显示玩家掉线
	EventCustom event(EVENT_LOSEROOM);
	event.setUserData((void*)cSeatId_);
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

//别的玩家
void QGameManager::dealUserBack(Ref *obj)
{
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)
		return;
	unsigned char cSeatId_ = pBuff->readUChar();

	EventCustom event(EVENT_BACKROOM);
	event.setUserData((void*)cSeatId_);
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void QGameManager::dealUserExit(Ref *obj)
{
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)
		return;
	unsigned char cSeatId_ = pBuff->readUChar();
	if (cSeatId_ == m_cSeatId_)
	{
		WWSceneManager::getInstance()->openScene(QHallScene::create());
	}
	else
	{
		//发送事件 通知 用户已经离开 把头像删了
		EventCustom event(EVENT_EXITROOM);
		event.setUserData((void*)cSeatId_);
		Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
	}
}

void QGameManager::dealGameBack(Ref *obj)
{
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)
		return;
	m_vecSellteData.clear();
	QGameBackData *qGameBackData = QGameBackData::create();
	qGameBackData->setGameStatues(pBuff->readUChar());	// 当前游戏状态
	qGameBackData->setBanker(pBuff->readUChar());		// 当前庄家
	qGameBackData->setMulti(pBuff->readUChar());		// 庄家的倍数
	qGameBackData->setPlayNum(pBuff->readUChar());		// 当前局数(现在是第几局)
	m_cGameStatues_ = qGameBackData->getGameStatues();
	unsigned int iNum = pBuff->readUInt();
	QUserBackData *qUserData = nullptr;
	QCardData *pCard = QCardData::create();
	for (int i = 0; i < iNum; i++)
	{
		qUserData = QUserBackData::create();
		qUserData->setSeatId(pBuff->readUChar());
		pCard->setSeatId(qUserData->getSeatId());
		qUserData->setReady(pBuff->readUChar());
		qUserData->setRobState(pBuff->readUChar());
		qUserData->setBetCount(pBuff->readUChar());
		qUserData->setOpenState(pBuff->readUChar());
		qUserData->setConnect(pBuff->readUChar());
		qUserData->setScore(pBuff->readInt());

		unsigned int n = pBuff->readUInt();
		qUserData->getpCardData()->getPokerVec().clear();
		for (int j = 0; j < n; j++)
		{
			auto temp = pBuff->readUChar();
			qUserData->getpCardData()->getPokerVec().push_back(temp);
			pCard->getPokerVec().push_back(temp);
		}
		qUserData->getpCardData()->setCow(pBuff->readUChar());
		pCard->setCow(qUserData->getpCardData()->getCow());
		qUserData->getpCardData()->setValue(pBuff->readUShort());
		pCard->setValue(qUserData->getpCardData()->getValue());
		qUserData->getpCardData()->setSeatId(qUserData->getSeatId());

		if (qUserData->getSeatId() == m_cSeatId_)
		{
			m_cReady_ = qUserData->getReady();
			m_pCardData_->setCow(qUserData->getpCardData()->getCow());
			m_pCardData_->setValue(qUserData->getpCardData()->getValue());
			m_pCardData_->setSeatId(qUserData->getpCardData()->getSeatId());
			m_pCardData_->getPokerVec() = qUserData->getpCardData()->getPokerVec();
		}
		if (pCard->getPokerVec().size() != 0)
		{
			m_vecSellteData.pushBack(pCard);
		}
		qGameBackData->getUserBackVec().pushBack(qUserData);
	}

	EventCustom event(EVENT_GAMEBACK);
	event.setUserData((void*)qGameBackData);
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void QGameManager::dealReady(Ref *obj)
{
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)
		return;
	unsigned char cSeatId_ = pBuff->readUChar();
	EventCustom event(EVENT_GAMEREADY);
	event.setUserData((void*)cSeatId_);
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void QGameManager::dealRoomCancel(Ref *obj)
{
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)
		return;
	unsigned char cSeatId_ = pBuff->readUChar();	

	EventCustom event(EVENT_ROOMCANCEL);
	event.setUserData((void*)cSeatId_);
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void QGameManager::dealVoteCancel(Ref *obj)
{
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)
		return;
	unsigned char cSeatId = pBuff->readUChar();
	unsigned char cAgree = pBuff->readUChar();
	int seatId = cSeatId;
	int agree = cAgree;
	std::stringstream is;
	is << seatId << "\n" << agree;
	std::string sInfo = is.str();

	EventCustom event(EVENT_VOTECANCEL);
	event.setUserData((char*)sInfo.c_str());
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void QGameManager::dealRoomFate(Ref *obj)
{
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)
		return;

	unsigned char cPass = pBuff->readUChar();	// 0表示不通过表决,非0表示通过表决了(意味着要解散房间了)
	
	EventCustom event(EVENT_GAMEOVER);
	event.setUserData((void*)cPass);
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void QGameManager::dealRoomOver(Ref *obj)
{
	m_vecOver_.clear();
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)
		return;
	m_strTime = pBuff->readLengthAndString();
	unsigned int iPlayerNum = pBuff->readUInt();
	QEndData *qData = nullptr;
	for (int i = 0; i < iPlayerNum; i++)
	{
		qData = QEndData::create();
		qData->setSeatId(pBuff->readUChar());
		qData->setKillCount(pBuff->readUChar());
		qData->setCowCount(pBuff->readUChar());
		qData->setWinCount(pBuff->readUChar());
		qData->setLoseCount(pBuff->readUChar());
		qData->setScore(pBuff->readInt());
		unsigned int num = pBuff->readUInt();
		for (int j = 0; j < num; j++)
		{
			qData->getXiVec().push_back(pBuff->readUChar());
		}
		m_vecOver_.pushBack(qData);
	}
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_ROOMOVER);
}

void QGameManager::dealGameStatues(Ref *obj)
{
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)
		return;

	m_cGameStatues_ = pBuff->readUChar();

	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_GAMESTATUES);
}

void QGameManager::dealPoker(Ref *obj)
{
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)
		return;
	QCardData *pCard = QCardData::create();

	unsigned int iNum = pBuff->readUInt();
	m_pCardData_->getPokerVec().clear();
	for (int i = 0; i < iNum; i++)
	{
		auto temp = pBuff->readUChar();
		m_pCardData_->getPokerVec().push_back(temp);
		pCard->getPokerVec().push_back(temp);
	}
	auto cow = pBuff->readUChar();
	m_pCardData_->setCow(cow);
	pCard->setCow(cow);
	auto value = pBuff->readUShort();
	m_pCardData_->setValue(value);
	pCard->setValue(value);
	m_pCardData_->setSeatId(m_cSeatId_);
	pCard->setSeatId(m_cSeatId_);

	//m_vecSellteData.pushBack(pCard);

	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_GETCARD);
}

void QGameManager::dealQZhuang(Ref *obj)
{
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)
		return;

	unsigned char cSeatId = pBuff->readUChar();
	unsigned char cRob = pBuff->readUChar();
	int seatId = cSeatId;
	int rob = cRob;
	std::stringstream is;
	is << seatId << "\n" << rob;
	std::string sInfo = is.str();

	EventCustom event(EVENT_ROB);
	event.setUserData((char*)sInfo.c_str());
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);

}

void QGameManager::dealZhuangEnd(Ref *obj)
{
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)
		return;

	unsigned char cSeatId = pBuff->readUChar(); //谁是庄家
	unsigned char cMulti = pBuff->readUChar(); //抢庄的倍数
	int seatId = cSeatId;
	int multi = cMulti;
	std::stringstream is;
	is << seatId << "\n" << multi;
	std::string sInfo = is.str();

	EventCustom event(EVENT_GETZHUANG);
	event.setUserData((char*)sInfo.c_str());
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);

}

void QGameManager::dealBet(Ref *obj)
{
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)
		return;

	unsigned char cSeatId = pBuff->readUChar();
	unsigned char cBet = pBuff->readUChar();

	int seatId = cSeatId;
	int bet = cBet;

	std::stringstream is;
	is << seatId << "\n" << bet;
	std::string sInfo = is.str();

	EventCustom event(EVENT_GETBET);
	event.setUserData((char*)sInfo.c_str());
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void QGameManager::dealOpenCard(Ref *obj)
{
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)
		return;
	QCardData *pCard = QCardData::create();
	pCard->setSeatId(pBuff->readUChar());
	unsigned int iNum = pBuff->readUInt();
	for (int i = 0; i < iNum; i++)
	{
		pCard->getPokerVec().push_back(pBuff->readUChar());
	}
	pCard->setCow(pBuff->readUChar());
	pCard->setValue(pBuff->readUShort());

	m_vecSellteData.pushBack(pCard);

	EventCustom event(EVENT_OPENCARD);
	event.setUserData((void*)pCard);
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void QGameManager::dealGameResult(Ref *obj)
{
	m_vecResult_.clear();
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)
		return;

	unsigned int iNum = pBuff->readUInt();
	QRetData *qData = nullptr;
	for (int i = 0; i < iNum; i++)
	{
		qData = QRetData::create();
		qData->setSeatId(pBuff->readUChar());
		qData->setScore(pBuff->readInt());
		m_vecResult_.pushBack(qData);
	}
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_RESULT);
}

void QGameManager::dealCostRoomCardNum(Ref *obj)
{
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)
		return;

	int cNum = pBuff->readInt();
	unsigned int cTotal = QHallManager::getInstance()->getRoomCard();
	cTotal = cTotal + cNum;
	QHallManager::getInstance()->setRoomCard(cTotal);
}

void QGameManager::dealChatFast(Ref *obj)
{
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)
		return;

	unsigned char cSeatId = pBuff->readUChar();
	unsigned char cChatId = pBuff->readUChar(); 	

	int seatId = cSeatId;
	int chatId = cChatId;
	std::stringstream is;
	is << seatId << "\n" << chatId;
	std::string sInfo = is.str();

	EventCustom event(EVENT_CHAR_FAST);
	event.setUserData((char*)sInfo.c_str());
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);

}

void QGameManager::dealChatVoice(Ref *obj)
{
    WWBuffer *pBuff = (WWBuffer*)obj;
    if (pBuff == nullptr)
        return;
    
    unsigned char cSeatId = pBuff->readUChar();
    unsigned int cPlayTime = pBuff->readUInt();
    std::string cVoice = pBuff->readLengthAndString();
    
	QGameVoiceData *qVoiceData = new QGameVoiceData(); //::create();
    int seatId = cSeatId;
    int nPlayTime = cPlayTime;			//
    qVoiceData->setVoice(cVoice);
    qVoiceData->setSeatId(seatId);
    qVoiceData->setPlayTime(nPlayTime);
    
    EventCustom event(EVENT_CHAR_VOICE);
    event.setUserData((void*)qVoiceData);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    
}

void QGameManager::dealTakeOn(Ref *obj)
{
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)
		return;

	unsigned int cRoomNum = pBuff->readUInt();
	EventCustom event(EVENT_TAKE_ON);
	event.setUserData((void*)cRoomNum);
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void QGameManager::dealTakeOff(Ref *obj)
{
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)
		return;

	unsigned char errorCode = pBuff->readUChar();
	EventCustom event(EVENT_TAKE_OFF);
	event.setUserData((void*)errorCode);
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void QGameManager::dealRoomKick(Ref *obj)
{
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)
		return;

	unsigned int cError = pBuff->readUChar();
	EventCustom event(EVENT_ROOM_KICK);
	event.setUserData((void*)cError);
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void QGameManager::dealRoomWarn(Ref *obj)
{
	WWBuffer *pBuff = (WWBuffer*)obj;
	if (pBuff == nullptr)
		return;

	std::string cWarn = pBuff->readLengthAndString();
	EventCustom event(EVENT_ROOM_WARN);
	event.setUserData((char*)cWarn.c_str());
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}
