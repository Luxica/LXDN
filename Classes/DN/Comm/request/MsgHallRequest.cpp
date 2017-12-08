#include "MsgHallRequest.h"
#include "../../../NetSystem/ww-net.h"
#include "../../../EventSystem/ww-event.h"
#include "../../Scene/HallScene/data/QHallManager.h"

USING_NS_CC;

enum CL_CMD_MSG                                         // 客户端->大厅服务器
{
	CL_CMD_USR_LOGIN = 10000,                            // 请求帐号登录
	CL_CMD_LOG_UID,                                      // 用户请求登录(用UID登录)
	CL_CMD_SET_INFO,                                     // 上传用户信息
	CL_CMD_GET_INFO,                                     // 获取用户信息(这个消息一般用不到)
	CL_CMD_ASK_ROOM,                                     // 询问房间地址(断线重连后用，查询自己所在的房间服务器)
	CL_CMD_GET_ROOM,                                     // 请求房间地址(开房，加房用这个消息)
	CL_CMD_GET_RANK,                                     // 请求获取排行
	CL_CMD_GET_RECD,                                     // 请求获取记录
	CL_CMD_GET_TIMES,                                    // 请求获取战局(小结记录)
	CL_CMD_GET_PLAY,                                     // 请求获取回放(小结回放)
	CL_CMD_GET_NOTICE,                                   // 获取公告信息(跑马灯,客户端登录成功后主动发送获取一下)
	CL_CMD_BND_CODE,                                     // 用户绑邀请码
	CL_CMD_MSG_POPUP,                                    // 全服弹窗消息(非GM无效)
	CL_CMD_CAN_SHARE,                                    // 今日能否分享
	CL_CMD_DAY_SHARE,                                    // 用户每日分享
	CL_CMD_GET_ONLINE,                                   // 在线用户数量
	CL_CMD_GET_TAKE,                                     // 获取代开房间
	CL_CMD_FND_ROOM,                                     // 查找指定房间(GM有效)
	CL_CMD_OFF_ROOM,                                     // 解散指定房间(GM有效)
	CL_CMD_FBK_SUBMIT,                                   // 提交反馈信息
};

MsgHallRequest::MsgHallRequest()
{

}

MsgHallRequest::~MsgHallRequest()
{

}

void MsgHallRequest::sendLogin(const char *sAccount, const char *sPassword)
{
    log("MsgHallRequest::sendLogin");
	WWBuffer wBuffer;
	wBuffer.writeInt(CL_CMD_USR_LOGIN);
	wBuffer.writeLengthAndString(sAccount);
	wBuffer.writeLengthAndString(sPassword);
	WWMsgManager::getInstance()->sendMessage(WWNetwork::eHallNet, &wBuffer);
}

void MsgHallRequest::sendLoginGame(unsigned int iUserid, const char *sPassword)
{
	log("MsgGameRequest::sendLogin");
	WWBuffer wBuffer;
	wBuffer.writeInt(CL_CMD_LOG_UID);
	wBuffer.writeUInt(iUserid);
	wBuffer.writeLengthAndString(sPassword);
	WWMsgManager::getInstance()->sendMessage(WWNetwork::eGameNet, &wBuffer);
}

void MsgHallRequest::sendEnterRoom(const unsigned int &num)
{
	WWBuffer wBuffer;
	wBuffer.writeInt(CL_CMD_GET_ROOM);
	wBuffer.writeUInt(num);
	WWMsgManager::getInstance()->sendMessage(WWNetwork::eHallNet, &wBuffer);
}

void MsgHallRequest::sendAskRoom()
{
	WWBuffer wBuffer;
	wBuffer.writeInt(CL_CMD_ASK_ROOM);
	WWMsgManager::getInstance()->sendMessage(WWNetwork::eHallNet, &wBuffer);
}

void MsgHallRequest::sendSetInfo(const char *sNickName, const char *sImage, const char sex, const char* sLocation)
{
	WWBuffer wBuffer;
	wBuffer.writeInt(CL_CMD_SET_INFO);
	wBuffer.writeLengthAndString(sNickName);
	wBuffer.writeLengthAndString(sImage);
    wBuffer.writeUChar(sex);
	wBuffer.writeLengthAndString(sLocation);

	WWMsgManager::getInstance()->sendMessage(WWNetwork::eHallNet, &wBuffer);
}

void MsgHallRequest::sendGetInfo()
{
	WWBuffer wBuffer;
	wBuffer.writeInt(CL_CMD_GET_INFO);
	wBuffer.writeUInt(QHallManager::getInstance()->getUserId());
	WWMsgManager::getInstance()->sendMessage(WWNetwork::eHallNet, &wBuffer);
}

void MsgHallRequest::sendGetRank(unsigned char cType)
{
	WWBuffer wBuffer;
	wBuffer.writeInt(CL_CMD_GET_RANK);
	wBuffer.writeUChar(cType);
	WWMsgManager::getInstance()->sendMessage(WWNetwork::eHallNet, &wBuffer);
}

void MsgHallRequest::sendGetRecord(unsigned char cGameType)
{
	WWBuffer wBuffer;
	wBuffer.writeInt(CL_CMD_GET_RECD);
	wBuffer.writeUChar(cGameType);
	WWMsgManager::getInstance()->sendMessage(WWNetwork::eHallNet, &wBuffer);
}

void MsgHallRequest::sendGetNotice()
{
	WWBuffer wBuffer;
	wBuffer.writeInt(CL_CMD_GET_NOTICE);
	WWMsgManager::getInstance()->sendMessage(WWNetwork::eHallNet, &wBuffer);
}

void MsgHallRequest::sendBindCode(const char *sCode)
{
    WWBuffer wBuffer;
    wBuffer.writeInt(CL_CMD_BND_CODE);
    wBuffer.writeLengthAndString(sCode);
    WWMsgManager::getInstance()->sendMessage(WWNetwork::eHallNet, &wBuffer);
}
void MsgHallRequest::sendCanShare()
{
	WWBuffer wBuffer;
	wBuffer.writeInt(CL_CMD_CAN_SHARE);
	WWMsgManager::getInstance()->sendMessage(WWNetwork::eHallNet, &wBuffer);
}
void MsgHallRequest::sendDayShare()
{
	WWBuffer wBuffer;
	wBuffer.writeInt(CL_CMD_DAY_SHARE);
	WWMsgManager::getInstance()->sendMessage(WWNetwork::eHallNet, &wBuffer);
}

void MsgHallRequest::sendGetOnline() {
	WWBuffer wBuffer;
	wBuffer.writeInt(CL_CMD_GET_ONLINE);
	WWMsgManager::getInstance()->sendMessage(WWNetwork::eHallNet, &wBuffer);
}

void MsgHallRequest::sendGetTake(unsigned char cGameType, unsigned char cTakeType)
{
	WWBuffer wBuffer;
	wBuffer.writeInt(CL_CMD_GET_TAKE);
	wBuffer.writeUChar(cGameType);
	wBuffer.writeUChar(cTakeType);
	WWMsgManager::getInstance()->sendMessage(WWNetwork::eHallNet, &wBuffer);
}

void MsgHallRequest::sendFbkSubmit(const char * sPhone, const char * sFeedBack)
{
	WWBuffer wBuffer;
	wBuffer.writeInt(CL_CMD_FBK_SUBMIT);
	wBuffer.writeLengthAndString(sPhone);
	wBuffer.writeLengthAndString(sFeedBack);
	WWMsgManager::getInstance()->sendMessage(WWNetwork::eHallNet, &wBuffer);
}

