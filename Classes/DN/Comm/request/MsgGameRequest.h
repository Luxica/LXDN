#ifndef __MSGGAMEEQUEST_H__
#define __MSGGAMEREQUEST_H__
#include "cocos2d.h"

class WWBuffer;

class MsgGameRequest
{
public:
	MsgGameRequest();
	virtual ~MsgGameRequest();

	static void sendJoinRoom(const unsigned int &num);
	static void sendCreateRoom(unsigned char cPlayNum, unsigned char cZMode, unsigned char cCostMode, unsigned char cGameMode,
		unsigned char cMMode, unsigned char cCoverNum, unsigned char cMaxBet, unsigned int minRoomCard, unsigned char cHalf,
		unsigned char cForbid);
	static void sendBackRoom();
	static void sendReady();
	static void sendRoomCancel();
	static void sendVoteCancel(unsigned char cAgree); // 0表示不同意，非0表示同意
	static void sendRobZhuang(unsigned char cRob);    // 0表示不抢庄，非0表示抢庄
	static void sendBet(unsigned char cBet);		  //闲家下注(庄家就不要去下注了)
	static void sendOpenPoker();				      //用户发送开牌

	static void sendChatFast(unsigned char cChatId);
	static void sendChatVoice(const char *sChatData,unsigned int cPlayTime );

	static void sendCtrlWin(unsigned char cTimes, unsigned char cWinSeat);

	static void sendTakeOn(unsigned char cPlayNum, unsigned char cZMode, unsigned char cCostMode, unsigned char cGameMode,
		unsigned char cMMode, unsigned char cCoverNum, unsigned char cMaxBet, unsigned int minRoomCard, unsigned char cHalf,
		unsigned char cForbid);
	static void sendTakeOff(unsigned int cRoomNum);

	static void sendRoomKick(unsigned int cRoomNum, unsigned char cSeatID);
	static void sendRoomWarn(unsigned int cRoomNum, std::string cStrWarn);

	static void sendStartGame(unsigned int cUserID);
};

#endif /* MsgGameRequest */
