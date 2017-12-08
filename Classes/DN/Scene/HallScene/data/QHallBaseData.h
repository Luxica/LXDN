#ifndef __QHALL_BASE_DATA_H__
#define __QHALL_BASE_DATA_H__

#include "cocos2d.h"
using namespace cocos2d;

class QScoreData :public Ref
{
public:
	QScoreData();
	~QScoreData();
	bool init()	{ return true; }
	CREATE_FUNC(QScoreData);

	CC_SYNTHESIZE(unsigned int, m_nUserId_, UserId);					//userId
	CC_SYNTHESIZE(std::string, m_strNick, NickName);					//昵称
	CC_SYNTHESIZE(std::string, m_strImage, HeadImage);                  // 头像
	CC_SYNTHESIZE(int, m_nScore_, Score);								//积分
};

class QRankUser : public Ref
{
public:
	QRankUser();
	~QRankUser();
	bool init()	{ return true; }
	CREATE_FUNC(QRankUser);

	CC_SYNTHESIZE(unsigned int, m_nUserId_, UserId);					//userId
	CC_SYNTHESIZE(std::string, m_strNick, NickName);					//昵称
	CC_SYNTHESIZE(std::string, m_strImage, ImageName);					//图像
	CC_SYNTHESIZE(int, m_nScore_, Score);								//积分
	CC_SYNTHESIZE(unsigned int, m_nWinCount_, WinCount);				//赢局
	CC_SYNTHESIZE(unsigned int, m_nLoseCount_, LoseCount);				//输局
};

class QRankData : public Ref
{
public:
	QRankData();
	~QRankData();

	bool init()	{ return true; }
	CREATE_FUNC(QRankData);

	CC_SYNTHESIZE(unsigned char, m_nRankType_, RankType);					//排行榜的类型(0今日排行1昨日排行2本周排行3上周排行)
	Vector<QRankUser*>& getRankUserVec()		{ return m_vecRankUser_; }
public:
	Vector<QRankUser*>m_vecRankUser_;
};

class QVXRoom : public Ref
{
public:
	QVXRoom();
	~QVXRoom();

	bool init()	{ return true; }
	CREATE_FUNC(QVXRoom);

public:
	CC_SYNTHESIZE(unsigned char, m_cPlayMode_, PlayMode);		//局数的模式
	CC_SYNTHESIZE(unsigned char, m_cZMode_, ZMode);				//坐庄的模式
	CC_SYNTHESIZE(unsigned char, m_cPayMode_, PayMode);			//付卡的模式
	CC_SYNTHESIZE(unsigned char, m_cGameMode_, GameMode);		//玩法模式
	CC_SYNTHESIZE(unsigned char, m_cMMode_, MMode);				//倍数的模式(0普通倍数,1超级倍数)
	CC_SYNTHESIZE(unsigned char, m_cCover_, Cover);				//盖住几张牌
	CC_SYNTHESIZE(unsigned char, m_cMaxBet_, MaxBet);			//最大下注/下注模式
	CC_SYNTHESIZE(unsigned int, m_nMinCard_, MinCard);			//最少房卡(大于等于此值才可以开房/加房)
	CC_SYNTHESIZE(unsigned char, m_cHalf_, Half);				//半副牌(0完整的,1半幅的)
	CC_SYNTHESIZE(unsigned char, m_cForbid_, Forbid);			//开局后禁止加入
};

class QRecordData : public Ref
{
public:
	QRecordData();
	~QRecordData();

	bool init()	{ return true; }
	CREATE_FUNC(QRecordData);

public:
	CC_SYNTHESIZE(unsigned int, m_nRecdID_, RecdID);			//记录ID(每条记录都有唯一ID)
	CC_SYNTHESIZE(unsigned int, m_nRoomNum_, RoomNum);			//房间号
	CC_SYNTHESIZE(unsigned int, m_nHostID_, HostID);			// 房主(房间创建者)
	CC_SYNTHESIZE(unsigned char, m_cGameType_, GameType);		//游戏类型(牛牛1，扎金花2)
	CC_SYNTHESIZE(unsigned char, m_cRType, RType);				// 房间类型(ROOM_TYPE)
	CC_SYNTHESIZE(QVXRoom*, m_sVXRoom_, VXRoom);				//房间信息
	CC_SYNTHESIZE(std::string, m_sTime_, RecordTime);			//时间
	Vector<QScoreData*>& getScoreVec()		{ return m_vecScore_; }
public:
	Vector<QScoreData*>m_vecScore_;
};

class QVXUser : public Ref
{
public:
	QVXUser();
	~QVXUser();

	bool init()	{ return true; }
	CREATE_FUNC(QVXUser);

public:
	CC_SYNTHESIZE(unsigned int, m_nUserId_, UserId);			//用户ID(0为无效ID)
	CC_SYNTHESIZE(std::string, m_strNick_, NickName);			//昵称
	CC_SYNTHESIZE(std::string, m_strImage_, ImageName);			//图像
	CC_SYNTHESIZE(unsigned char, m_cSex_, Sex);					//性别(0未知1男2女)
	CC_SYNTHESIZE(std::string, m_strIP_, StrIP);				//IP
};

struct QVxRoom
{
	unsigned char m_cPlayMode_;			//局数的模式
	unsigned char m_cZMode_;			//坐庄的模式
	unsigned char m_cPayMode_;			//付卡的模式
	unsigned char m_cCover_;			//盖住几张牌
	unsigned char m_cMaxBet_;			//最大下注/下注模式
	unsigned int m_nMinCard_;			//最少房卡(大于等于此值才可以开房/加房)
	unsigned int m_nExtend_;			//扩展数据(缺省为0,1表示加入坎顺玩法)  
};

class QTARoom : public Ref
{
public:
	QTARoom();
	~QTARoom();

	bool init()	{ return true; }
	CREATE_FUNC(QTARoom);

public:
	CC_SYNTHESIZE(unsigned int, m_nRoomNum_, RoomNum);			//房间号
	CC_SYNTHESIZE(unsigned int, m_nTakeId_, TakeId);			//代开人
	CC_SYNTHESIZE(unsigned char, m_nGameType_, GameType);		//游戏类型(GAME_TYPE)[1牛牛,2炸金花]
	CC_SYNTHESIZE(char, m_cRoomState_, RoomState);				//房间状态(0游戏未开始 1游戏进行中 -1已结束)
	CC_SYNTHESIZE(std::string, m_sStrTime_, StrTime);			//代开时间
	CC_SYNTHESIZE(QVXRoom*, m_sVXRoom_, VXRoom);				//房间信息
	Vector<QVXUser*>& getUserVec()		{ return m_vecUser_; }
public:
	Vector<QVXUser*>m_vecUser_;
};
#endif