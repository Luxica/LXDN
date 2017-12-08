#include "QGameBaseData.h"

//----------------------------------------------QCardData--------------------------------------------------------
QCardData::QCardData()
{
	m_cCow_ = 0;
	m_cSeatId_ = 0;
	m_cValue_ = 0;
}

QCardData::~QCardData()
{
	m_vecPoker_.clear();
}

void QCardData::clearData() {
    m_vecPoker_.clear();
}
//----------------------------------------------QUserData--------------------------------------------------------
QUserData::QUserData()
{
	m_nUserId = 0;
	m_sNickName_ = "";
	m_strImage_ = "";
	m_nSeatId = 0;
	m_strIP_ = "";
	m_strLocation_ = "";
	m_nWinCount = 0;
	m_nLoseCount = 0;

	m_userBet = 0;
	m_userCow = -1;
}

QUserData::~QUserData()
{
	
}

void QUserData::setUserData(QUserData *pData)
{
	m_nUserId = pData->m_nUserId;
	m_nSeatId = pData->m_nSeatId;
	m_sNickName_ = pData->m_sNickName_;
	m_strImage_ = pData->m_strImage_;
	m_cSex = pData->m_cSex;
	m_strIP_ = pData->m_strIP_;
	m_strLocation_ = pData->m_strLocation_;
	m_nWinCount = pData->m_nWinCount;
	m_nLoseCount = pData->m_nLoseCount;
}

//----------------------------------------------QUserBackData-------------------------------------------------
QUserBackData::QUserBackData()
{
	m_cSeatId_ = 0;
	m_cReady_ = 0;
	m_cRobState_ = 0;
	m_cBet_ = 0;
	m_cOpenState_ = 0;
	m_cConnect = 0;
	m_cScore_ = 0;
	m_pCardData_ = new QCardData();
}

QUserBackData::~QUserBackData()
{
	CC_SAFE_DELETE(m_pCardData_);
}
//----------------------------------------------QGameBackData-------------------------------------------------
QGameBackData::QGameBackData()
{
	m_cGameStatues_ = 0;
	m_cBanker_ = -1;
	m_cPlayNum_ = 0;
	m_cMulti_ = 1;
}

QGameBackData::~QGameBackData()
{
	m_vecUserBack_.clear();
}
//-----------------------------------------------QRetData----------------------------------
QRetData::QRetData()
{
	m_cSeatId = 0;
	m_nScore_ = 0;
}

QRetData::~QRetData()
{

}
//-----------------------------------------------QEndData----------------------------------------------------
QEndData::QEndData()
{
	m_cSeatId = 0;
	m_cKillCount_ = 0;
	m_cCowCount_ = 0;
	m_cWinCount_ = 0;
	m_cLoseCount_ = 0;
	m_nScore_ = 0;
}

QEndData::~QEndData()
{
	m_vecXi.clear();
}

QGameVoiceData::QGameVoiceData()
{
    m_nPlayTime_ = 0;
    m_cSeatId_ = 0;
    m_sVoice_ = "";
}

QGameVoiceData::~QGameVoiceData()
{
    
}
