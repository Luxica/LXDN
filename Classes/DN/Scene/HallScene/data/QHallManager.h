#ifndef __QHALL_MANAGER_H__
#define __QHALL_MANAGER_H__

#include "cocos2d.h"
#include "../../GameScene/data/QGameBaseData.h"
#include "QHallBaseData.h"

using namespace cocos2d;

class QHallManager
{
private:
	QHallManager();
	~QHallManager();

public:
	static QHallManager* getInstance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new QHallManager();
		}
		return m_pInstance;
	}

	static void dispose()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	void dealLogin(Ref *obj);
	void dealHallEnterRoom(Ref *obj);
	void dealSetInfo(Ref *obj);
	void dealGetInfo(Ref *obj);
	void dealAskRoom(Ref *obj);
	void dealGetRank(Ref *obj);
	void dealGetRecord(Ref *obj);
	void dealNotice(Ref *obj);
    void dealPayResult(Ref *obj);
    void dealBindCode(Ref *obj);
	void dealCanShare(Ref *obj);
	void dealDayShare(Ref *obj);
	void dealGetOnline(Ref *obj);
	void dealGetTake(Ref *obj);
	void updateUserCard();
	void updateUserVip();

	void dealFbkSubmit(Ref *obj);

public:
	enum E_USER_VIP_TYPE {
		K_NO_VIP			= 0,
		K_DIAMONDS_VIP		= 1,
		K_COLOR_DIAMOND_VIP
	};
	enum E_ROOM_TYPE {
		R_NOMAL = 0,
		R_DIAMOND = 1,
		R_COLORDIAMOND
	};
	CC_SYNTHESIZE(std::string, m_sGameIp_, GameIp);				//游戏ip
	CC_SYNTHESIZE(unsigned short, m_nGamePort_, GamePort);		//游戏port
	CC_SYNTHESIZE(std::string, m_sAccount_, Account);			//账号
	CC_SYNTHESIZE(std::string, m_sPassword_, Password);			//密码
    CC_SYNTHESIZE(std::string, m_sHeadUrl, HeadUrl);			//头像url
    CC_SYNTHESIZE(unsigned char, m_cSex, Sex);                  //性别
    CC_SYNTHESIZE(std::string, m_sNickName, NickName);          //昵称
	CC_SYNTHESIZE_READONLY(E_USER_VIP_TYPE, m_vipType, VIP);	//vip类型
	CC_SYNTHESIZE(E_ROOM_TYPE, m_roomType, RoomType);	//vip类型
	CC_SYNTHESIZE(unsigned int, m_nUserId_, UserId);			//userId
	CC_SYNTHESIZE(unsigned int, m_nRoomNum_, RoomNum);			//房间号
	CC_SYNTHESIZE(unsigned int, m_nDisRoomNum_, DisRoomNum);	//要解散的房间号
	CC_SYNTHESIZE(unsigned int, m_nKickRoomNum_, KickRoomNum);	//要踢人的房间号
	CC_SYNTHESIZE(unsigned int, m_nWarnRoomNum_, WarnRoomNum);	//要警告的房间号
	CC_SYNTHESIZE(unsigned int, m_nInsteadRoomNum_, InsteadRoomNum);	//代开成功的房间号
	CC_SYNTHESIZE(unsigned char, m_nKickSeatID_, KickSeatID);			//userId
	CC_SYNTHESIZE_READONLY(unsigned int, m_nRoomCard_, RoomCard);	//房卡剩余
	inline virtual void setRoomCard(unsigned int var) { 		
		m_nRoomCard_ = var;
		updateUserCard();
		updateUserVip();
	}
	CC_SYNTHESIZE(unsigned char, m_cGameType_, GameType);		//游戏类型(GAME_TYPE)[1牛牛2扎金花]
	CC_SYNTHESIZE(unsigned char, m_cTakeType_, TakeType);		//代开的类型(0未结束的代开,1已结束的代开)
	CC_SYNTHESIZE(unsigned int, m_nCostNum_, CostNum);			//要扣除的房卡数
	CC_SYNTHESIZE(unsigned int, m_nUserType_, UserType);		//用户类型
	CC_SYNTHESIZE(unsigned char, m_cPlayMode_, PlayMode);		//局数的模式
	CC_SYNTHESIZE(unsigned char, m_cZMode_, ZMode);				//坐庄的模式
	CC_SYNTHESIZE(unsigned char, m_cCostMode_, CostMode);		//付卡的模式
	CC_SYNTHESIZE(unsigned char, m_cGameMode_, GameMode);		//游戏的模式(0经典,1坎顺,2银牛,3顺金)
	CC_SYNTHESIZE(unsigned char, m_cMMode_, MMode);				// 倍数的模式(0普通倍数,1超级倍数)
	CC_SYNTHESIZE(unsigned char, m_cKcardWay_, KcardWay);		//扣牌的方式(0为默认扣一张，1为全扣)
	CC_SYNTHESIZE(unsigned char, m_cMaxBet_, MaxBet);			//下注
	CC_SYNTHESIZE(unsigned int, m_cMinCard_, MinCard);			//楚天最少卡数
	CC_SYNTHESIZE(unsigned char, m_cHalf_, Half);				//半副牌(0完整的,1半幅的)
	CC_SYNTHESIZE(unsigned char, m_cForbid_, Forbid);			//开局后禁止加入

	CC_SYNTHESIZE(unsigned char, m_cJoinType_, JoinType);		//创建 还是 加入 还是 代开
	CC_SYNTHESIZE(unsigned char, m_cBackType_, BackType);		//1是重连回来 0是正常
	CC_SYNTHESIZE(bool, m_bIsFirst, IsFirst);					//是否是首次登陆大厅
	CC_SYNTHESIZE(unsigned int, m_curNum, CurNum);			//当前在线人数

	CC_SYNTHESIZE(std::string, m_curAddress_, CurAddress);		//地址

	CC_SYNTHESIZE(QRankData*, m_pRankData_, RankData);	
	Vector<QRecordData*>m_vecRecord_;
	Vector<QTARoom*>	m_vecTARoom_;
	std::vector<std::string>m_VecNotice;
	std::vector<std::string>& getNoticeVec()		{ return m_VecNotice; }
private:
	static QHallManager* m_pInstance;
};
#define QHALLMGR QHallManager::getInstance()
#endif
