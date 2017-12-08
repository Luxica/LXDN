#ifndef __QGAME_BASE_DATA_H__
#define __QGAME_BASE_DATA_H__

#include "cocos2d.h"
using namespace cocos2d;

class QCardData : public Ref
{
public:
	QCardData();
	~QCardData();

	bool init()	{ return true; }
    void clearData();
	CREATE_FUNC(QCardData);
public:
	CC_SYNTHESIZE(unsigned char, m_cCow_, Cow);
	CC_SYNTHESIZE(unsigned int, m_cValue_, Value);
	CC_SYNTHESIZE(unsigned char, m_cSeatId_, SeatId);
	std::vector<unsigned char>& getPokerVec()		{ return m_vecPoker_; }
public:
	std::vector<unsigned char>m_vecPoker_;
};

class QUserData : public Ref
{
public:
	QUserData();
	~QUserData();

	bool init()	{ return true; }
	CREATE_FUNC(QUserData);
	void setUserData(QUserData *pData);
public:
	CC_SYNTHESIZE(unsigned int, m_nUserId, UserId);
	CC_SYNTHESIZE(unsigned int, m_nSeatId, SeatId);
	CC_SYNTHESIZE(std::string, m_sNickName_, NickName);
	CC_SYNTHESIZE(std::string, m_strImage_, StrImage);
	CC_SYNTHESIZE(unsigned char, m_cSex, Sex);
	CC_SYNTHESIZE(std::string, m_strIP_, StrIP);
	CC_SYNTHESIZE(std::string, m_strLocation_, StrLocation);
	CC_SYNTHESIZE(unsigned int, m_nWinCount, WinCount);
	CC_SYNTHESIZE(unsigned int, m_nLoseCount, LoseCount);

	CC_SYNTHESIZE(unsigned int, m_userBet, UserBet);
	CC_SYNTHESIZE(unsigned int, m_userCow, UserCow);
};

class QUserBackData :public Ref
{
public:
	QUserBackData();
	~QUserBackData();
	bool init() { return true; }
	CREATE_FUNC(QUserBackData);
public:
	CC_SYNTHESIZE(unsigned char, m_cSeatId_, SeatId);
	CC_SYNTHESIZE(unsigned char, m_cReady_, Ready);
	CC_SYNTHESIZE(unsigned char, m_cRobState_, RobState);
	CC_SYNTHESIZE(unsigned char, m_cBet_, BetCount);
	CC_SYNTHESIZE(unsigned char, m_cOpenState_, OpenState);
	CC_SYNTHESIZE(unsigned char, m_cConnect,Connect);
	CC_SYNTHESIZE(int, m_cScore_, Score);
	CC_SYNTHESIZE(QCardData*, m_pCardData_, pCardData);
};

class QGameBackData : public Ref
{
public:
	QGameBackData();
	~QGameBackData();
	bool init() { return true; }
	CREATE_FUNC(QGameBackData);
public:
	CC_SYNTHESIZE(unsigned char, m_cGameStatues_, GameStatues);
	CC_SYNTHESIZE(unsigned char, m_cBanker_, Banker);
	CC_SYNTHESIZE(unsigned char, m_cMulti_, Multi);
	CC_SYNTHESIZE(unsigned char, m_cPlayNum_, PlayNum);

	Vector<QUserBackData*>& getUserBackVec()		{ return m_vecUserBack_; }
private:
	Vector<QUserBackData*>m_vecUserBack_;
};

class QRetData : public Ref
{
public:
	QRetData();
	~QRetData();
	bool init() { return true; }
	CREATE_FUNC(QRetData);

public:
	CC_SYNTHESIZE(unsigned char, m_cSeatId, SeatId);
	CC_SYNTHESIZE(int, m_nScore_, Score);

};

class QEndData : public Ref
{
public:
	QEndData();
	~QEndData();
	bool init() { return true; }
	CREATE_FUNC(QEndData);

public:
	std::vector<unsigned char>& getXiVec()		{ return m_vecXi; }

public:
	CC_SYNTHESIZE(unsigned char, m_cSeatId, SeatId);
	CC_SYNTHESIZE(unsigned char, m_cKillCount_, KillCount);
	CC_SYNTHESIZE(unsigned char, m_cCowCount_, CowCount);
	CC_SYNTHESIZE(unsigned char, m_cWinCount_, WinCount);
	CC_SYNTHESIZE(unsigned char, m_cLoseCount_, LoseCount);
	CC_SYNTHESIZE(int, m_nScore_, Score);

	std::vector<unsigned char>	m_vecXi;				//牛型出现的次数
};

/**
 * 聊天语音
 */
class QGameVoiceData : public Ref
{
public:
    QGameVoiceData();
    ~QGameVoiceData();
    bool init() { return true; }
public:
    CC_SYNTHESIZE(unsigned int, m_nPlayTime_, PlayTime);
    CC_SYNTHESIZE(unsigned char, m_cSeatId_, SeatId);
    CC_SYNTHESIZE(std::string, m_sVoice_, Voice);
};

#endif
