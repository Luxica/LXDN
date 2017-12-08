#include "QHallBaseData.h"

//------------------------------------------QScoreData--------------------------------------------
QScoreData::QScoreData()
{
	m_nUserId_ = 0;
	m_strNick = "";
	m_strImage = "";
	m_nScore_ = 0;
}

QScoreData::~QScoreData()
{

}

//------------------------------------------QRankUser---------------------------------------------
QRankUser::QRankUser()
{
	m_nUserId_ = 0;
	m_strNick = "";
	m_strImage = "";
	m_nScore_ = 0;
	m_nWinCount_ = 0;
	m_nLoseCount_ = 0;
}

QRankUser::~QRankUser()
{

}

//------------------------------------------QRankData----------------------------------------------
QRankData::QRankData()
{
	m_nRankType_ = 0;
}

QRankData::~QRankData()
{
	m_vecRankUser_.clear();
}

//---------------------------------------QRecordData------------------------------------------------
QRecordData::QRecordData()
{
	m_nRecdID_ = 0;
	m_nRoomNum_ = 0;
	m_nHostID_ = 0;
	m_cGameType_ = 1;
	m_cRType = 0;
	m_sTime_ = "";

	m_sVXRoom_ = new QVXRoom();
}

QRecordData::~QRecordData()
{
	CC_SAFE_DELETE(m_sVXRoom_);
	m_vecScore_.clear();
}

QVXUser::QVXUser()
{
	m_nUserId_ = 0;
	m_strNick_ = "";
	m_strImage_ = "";
	m_cSex_ = 1;
	m_strIP_ = "";
}

QVXUser::~QVXUser()
{

}

QVXRoom::QVXRoom()
{
	m_cPlayMode_ = 1;
	m_cZMode_ = 1;
	m_cPayMode_ = 1;
	m_cGameMode_ = 0;
	m_cCover_ = 4;
	m_cMaxBet_ = 10;
	m_nMinCard_ = 1;
	m_cHalf_ = 0;
	m_cForbid_ = 0;
}

QVXRoom::~QVXRoom()
{

}

QTARoom::QTARoom()
{
	m_nRoomNum_ = 666666;
	m_nTakeId_ = 888888;
	m_cRoomState_ = 0;
	m_sStrTime_ = "";
	//m_sStrRoom = "";
	m_sVXRoom_ = new QVXRoom();
}

QTARoom::~QTARoom()
{
	CC_SAFE_DELETE(m_sVXRoom_);
	m_vecUser_.clear();
}
