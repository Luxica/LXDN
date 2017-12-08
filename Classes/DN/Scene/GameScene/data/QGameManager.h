#ifndef __QGAME_MANAGER_H__
#define __QGAME_MANAGER_H__

#include "cocos2d.h"
#include "QGameBaseData.h"
using namespace cocos2d;

class QGameManager
{
private:
	QGameManager();
	~QGameManager();

public:
	static QGameManager* getInstance()
	{
		static QGameManager pInstance;
		return &pInstance;
	}
    
    void clearData();

public:
	void dealLoginGame(Ref *obj);
	void dealEnterRoom(Ref *obj);
	void dealJoinRoom(Ref *obj);
	void dealUserLost(Ref *obj);
	void dealUserBack(Ref *obj);
	void dealGameBack(Ref *obj);
	void dealUserExit(Ref *obj);
	void dealReady(Ref *obj);
	void dealRoomCancel(Ref *obj);
	void dealVoteCancel(Ref *obj);
	void dealRoomFate(Ref *obj);
	void dealRoomOver(Ref *obj);
	void dealGameStatues(Ref *obj);
	void dealPoker(Ref *obj);
	void dealQZhuang(Ref *obj);
	void dealZhuangEnd(Ref *obj);
	void dealBet(Ref *obj);
	void dealOpenCard(Ref *obj);
	void dealGameResult(Ref *obj);
	void dealCostRoomCardNum(Ref *obj);

	void dealChatFast(Ref *obj);
    void dealChatVoice(Ref *obj);

	void dealTakeOn(Ref *obj);
	void dealTakeOff(Ref *obj);

	void dealRoomKick(Ref *obj);
	void dealRoomWarn(Ref *obj);


public:
	CC_SYNTHESIZE(unsigned int, m_nUserId_, UserId);					//userId
	CC_SYNTHESIZE(unsigned int, m_nRoomNum_, RoomNum);					//房间号
	CC_SYNTHESIZE(unsigned char, m_cSeatId_, SeatId);					//自己的座位
	CC_SYNTHESIZE(unsigned char, m_cGameStatues_, GameStatues);			//游戏状态

	CC_SYNTHESIZE(unsigned char, m_cReady_, Ready);						//是否准备

	CC_SYNTHESIZE(std::string, m_strTime, StrTime);						//游戏开始时间
	
	CC_SYNTHESIZE(QGameBackData*, m_qGameBackData, qGameBackData);
	CC_SYNTHESIZE(QCardData*, m_pCardData_, pCardData);

	CC_SYNTHESIZE(std::string, m_gotoRoomId, GotoRoomId);

public:

	Vector<QEndData*> m_vecOver_;
	Vector<QRetData*> m_vecResult_;
	Vector<QUserData*> m_vecUser_;
	Vector<QCardData*> m_vecSellteData;
};

#endif
