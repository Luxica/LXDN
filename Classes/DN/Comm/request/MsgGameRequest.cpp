#include "MsgGameRequest.h"
#include "../../../NetSystem/ww-net.h"
#include "../../../EventSystem/ww-event.h"

USING_NS_CC;

enum CR_CMD_MSG                                         // 客户端->房间服务器
{
	CR_CMD_ROOM_MAKE = 11000,                           // 用户请求开房
	CR_CMD_ROOM_JOIN,                                   // 用户请求进房
	CR_CMD_ROOM_BACK,                                   // 用户请求回房(断线后重进,请求恢复到掉线前的房间)
	CR_CMD_USER_HAND,                                   // 用户准备就绪  
	CR_CMD_ROOM_EXIT,                                   // 用户发起退房
	CR_CMD_VOTE_EXIT,                                   // 用户退房表决
	CR_CMD_VOTE_BANKER,                                 // 用户抢庄表决
	CR_CMD_USER_BET,                                    // 闲家发送下注
	CR_CMD_OPEN_POKER,                                  // 用户发送开牌
	CR_CMD_CHAT_FAST,                                   // 用户快捷聊天(不要频繁发,客户端做个判断,至少间隔3秒)
	CR_CMD_CHAT_VOICE,                                  // 用户语音聊天
	CR_CMD_CTRL_WIN,									// 控制游戏输赢(非GM无效)
	CR_CMD_CTRL_POKER,									// 控制游戏的牌(非GM无效)
	CR_CMD_TAKE_ON,                                     // 请求代开房间(代开者)
	CR_CMD_TAKE_OFF,                                    // 请求解散代开(代开者)
	CR_CMD_ROOM_KICK,                                   // 房主执行踢人(代开者or房主)
	CR_CMD_ROOM_WARN,                                   // 房主执行警告(代开者or房主)
	CR_CMD_START_GAME,                                  // 房主执行开始游戏
};

MsgGameRequest::MsgGameRequest()
{

}

MsgGameRequest::~MsgGameRequest()
{

}

void MsgGameRequest::sendJoinRoom(const unsigned int &num)
{
	WWBuffer wBuffer;
	wBuffer.writeInt(CR_CMD_ROOM_JOIN);
	wBuffer.writeUInt(num);
	WWMsgManager::getInstance()->sendMessage(WWNetwork::eGameNet, &wBuffer);
}

void MsgGameRequest::sendCreateRoom(unsigned char cPlayNum, unsigned char cZMode, unsigned char cCostMode, 
	unsigned char cGameMode, unsigned char cMMode, unsigned char cCoverNum, unsigned char cMaxBet, unsigned int minRoomCard,
	unsigned char cHalf, unsigned char cForbid)
{
 	WWBuffer wBuffer;
	wBuffer.writeInt(CR_CMD_ROOM_MAKE);
	wBuffer.writeUChar(cPlayNum);
	wBuffer.writeUChar(cZMode);
	wBuffer.writeUChar(cCostMode);
	wBuffer.writeUChar(cGameMode);
	wBuffer.writeUChar(cMMode);
	wBuffer.writeUChar(cCoverNum);
	wBuffer.writeUChar(cMaxBet);
	wBuffer.writeUInt(minRoomCard);
	wBuffer.writeUChar(cHalf);
	wBuffer.writeUChar(cForbid);
	WWMsgManager::getInstance()->sendMessage(WWNetwork::eGameNet, &wBuffer);
}

void MsgGameRequest::sendReady()
{
	WWBuffer wBuffer;
	wBuffer.writeInt(CR_CMD_USER_HAND);
	WWMsgManager::getInstance()->sendMessage(WWNetwork::eGameNet, &wBuffer);
}

void MsgGameRequest::sendRoomCancel()  //用户发起退房
{
	WWBuffer wBuffer;
	wBuffer.writeInt(CR_CMD_ROOM_EXIT);
	WWMsgManager::getInstance()->sendMessage(WWNetwork::eGameNet, &wBuffer);
}

void MsgGameRequest::sendBackRoom()
{
	WWBuffer wBuffer;
	wBuffer.writeInt(CR_CMD_ROOM_BACK);
	WWMsgManager::getInstance()->sendMessage(WWNetwork::eGameNet, &wBuffer);
}

void MsgGameRequest::sendVoteCancel(unsigned char cAgree)	//1表示同意退出，0表示不同意退出
{
	WWBuffer wBuffer;
	wBuffer.writeInt(CR_CMD_VOTE_EXIT);
	wBuffer.writeUChar(cAgree);
	WWMsgManager::getInstance()->sendMessage(WWNetwork::eGameNet, &wBuffer);
}

void MsgGameRequest::sendRobZhuang(unsigned char cRob)	 // 0表示不抢庄，非0表示抢庄
{
	WWBuffer wBuffer;
	wBuffer.writeInt(CR_CMD_VOTE_BANKER);
	wBuffer.writeUChar(cRob);
	WWMsgManager::getInstance()->sendMessage(WWNetwork::eGameNet, &wBuffer);
}

void MsgGameRequest::sendBet(unsigned char cBet)	//闲家下注(庄家就不要去下注了)
{
	WWBuffer wBuffer;
	wBuffer.writeInt(CR_CMD_USER_BET);
	wBuffer.writeUChar(cBet);
	WWMsgManager::getInstance()->sendMessage(WWNetwork::eGameNet, &wBuffer);
}

void MsgGameRequest::sendOpenPoker()
{
	WWBuffer wBuffer;
	wBuffer.writeInt(CR_CMD_OPEN_POKER);
	WWMsgManager::getInstance()->sendMessage(WWNetwork::eGameNet, &wBuffer);
}

//语音
void MsgGameRequest::sendChatFast(unsigned char cChatId)
{
	WWBuffer wBuffer;
	wBuffer.writeInt(CR_CMD_CHAT_FAST);
	wBuffer.writeUChar(cChatId);
	WWMsgManager::getInstance()->sendMessage(WWNetwork::eGameNet, &wBuffer);
}


void MsgGameRequest::sendChatVoice(const char *sChatData,unsigned int cPlayTime)
{
	WWBuffer wBuffer;
	wBuffer.writeInt(CR_CMD_CHAT_VOICE);
    wBuffer.writeUInt(cPlayTime);
	wBuffer.writeLengthAndString(sChatData);
	WWMsgManager::getInstance()->sendMessage(WWNetwork::eGameNet, &wBuffer);
}

void MsgGameRequest::sendCtrlWin(unsigned char cTimes, unsigned char cWinSeat)
{
	WWBuffer wBuffer;
	wBuffer.writeInt(CR_CMD_CTRL_WIN);
	wBuffer.writeUChar(cTimes);
	wBuffer.writeUChar(cWinSeat);
	WWMsgManager::getInstance()->sendMessage(WWNetwork::eGameNet, &wBuffer);
}

void MsgGameRequest::sendTakeOn(unsigned char cPlayNum, unsigned char cZMode, unsigned char cCostMode, unsigned char cGameMode,
	unsigned char cMMode, unsigned char cCoverNum, unsigned char cMaxBet, unsigned int minRoomCard, unsigned char cHalf,
	unsigned char cForbid)
{
	WWBuffer wBuffer;
	wBuffer.writeInt(CR_CMD_TAKE_ON);
	wBuffer.writeUChar(cPlayNum);
	wBuffer.writeUChar(cZMode);
	wBuffer.writeUChar(cCostMode);
	wBuffer.writeUChar(cGameMode);
	wBuffer.writeUChar(cMMode);
	wBuffer.writeUChar(cCoverNum);
	wBuffer.writeUChar(cMaxBet);
	wBuffer.writeUInt(minRoomCard);
	wBuffer.writeUChar(cHalf);
	wBuffer.writeUChar(cForbid);
	WWMsgManager::getInstance()->sendMessage(WWNetwork::eGameNet, &wBuffer);
}

void MsgGameRequest::sendTakeOff(unsigned int cRoomNum)
{
	WWBuffer wBuffer;
	wBuffer.writeInt(CR_CMD_TAKE_OFF);
	wBuffer.writeUInt(cRoomNum);
	WWMsgManager::getInstance()->sendMessage(WWNetwork::eGameNet, &wBuffer);
}

void MsgGameRequest::sendRoomKick(unsigned int cRoomNum, unsigned char cSeatID)
{
	WWBuffer wBuffer;
	wBuffer.writeInt(CR_CMD_ROOM_KICK);
	wBuffer.writeUInt(cRoomNum);
	wBuffer.writeUChar(cSeatID);
	WWMsgManager::getInstance()->sendMessage(WWNetwork::eGameNet, &wBuffer);
}

void MsgGameRequest::sendRoomWarn(unsigned int cRoomNum, std::string cStrWarn)
{
	WWBuffer wBuffer;
	wBuffer.writeInt(CR_CMD_ROOM_WARN);
	wBuffer.writeUInt(cRoomNum);
	wBuffer.writeLengthAndString(cStrWarn.c_str());
	WWMsgManager::getInstance()->sendMessage(WWNetwork::eGameNet, &wBuffer);
}

void MsgGameRequest::sendStartGame(unsigned int cUserID)
{
	WWBuffer wBuffer;
	wBuffer.writeInt(CR_CMD_START_GAME);
	wBuffer.writeUInt(cUserID);
	WWMsgManager::getInstance()->sendMessage(WWNetwork::eGameNet, &wBuffer);
}

