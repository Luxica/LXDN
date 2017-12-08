#ifndef MSGHALLEQUEST_H_
#define MSGHALLEQUEST_H_
#include "cocos2d.h"

class WWBuffer;

class MsgHallRequest
{
public:
	MsgHallRequest();
	virtual ~MsgHallRequest();
	static void sendLogin(const char *sAccount, const char *sPassword);//銮峰彇ip port
	static void sendLoginGame(unsigned int iUserid, const char *sPassword);
	static void sendEnterRoom(const unsigned int &num);
	static void sendAskRoom();
	static void sendSetInfo(const char *sNickName, const char *sImage,const char sSex, const char* sLocation);
	static void sendGetInfo();
	static void sendGetRank(unsigned char cType);	//鎺掕姒灭殑绫诲瀷(0浠婃棩鎺掕1鏄ㄦ棩鎺掕2链懆鎺掕3涓婂懆鎺掕)
	static void sendGetRecord(unsigned char cGameType);
	static void sendGetNotice();
    static void sendBindCode(const char *sCode);
	static void sendCanShare();
	static void sendDayShare();
	static void sendGetOnline();
	static void sendGetTake(unsigned char cGameType, unsigned char cTakeType);
	static void sendFbkSubmit(const char * sPhone, const char * sFeedBack);
};

#endif /* MsgHallRequest */
