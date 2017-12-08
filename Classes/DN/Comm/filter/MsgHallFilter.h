
#ifndef MSGHALLDATAFILTER_H_
#define MSGHALLDATAFILTER_H_
#include "cocos2d.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#include "ww-event.h"
#include "ww-net.h"
#else
#include "EventSystem/ww-event.h"
#include "NetSystem/ww-net.h"
#endif

enum LC_CMD_MSG                                         // 服务器->大厅客户端
{
	LC_CMD_USR_LOGIN = 20000,                            // 响应帐号登录
	LC_CMD_LOG_UID,                                      // 响应用户登录(用UID登录)
	LC_CMD_SET_INFO,                                     // 响应上传信息
	LC_CMD_GET_INFO,                                     // 响应获取信息
	LC_CMD_ASK_ROOM,                                     // 返回房间地址
	LC_CMD_GET_ROOM,                                     // 响应房间地址
	LC_CMD_GET_RANK,                                     // 响应获取排行
	LC_CMD_GET_RECD,                                     // 响应获取记录
	LC_CMD_GET_TIMES,                                    // 响应获取战局
	LC_CMD_GET_PLAY,                                     // 响应获取回放
	LC_CMD_GET_NOTICE,                                   // 返回公告信息(跑马灯)
	LC_CMD_MSG_POPUP,                                    // 通知弹窗消息
	LC_CMD_MSG_PAYS,                                     // 通知支付成功
	LC_CMD_BND_CODE,                                     // 响应绑定结果
	LC_CMD_CAN_SHARE,                                    // 响应能否分享
	LC_CMD_DAY_SHARE,                                    // 响应每日分享
	LC_CMD_GET_ONLINE,                                   // 在线用户数量
	LC_CMD_GET_TAKE,                                     // 响应获取代开
	LC_CMD_FND_ROOM,                                     // 响应查看房间(GM有效)
	LC_CMD_FBK_SUBMIT,                                   // 提交反馈信息结果
};

class MsgHallFilter :public WWIMsgFilter, public cocos2d::Ref
{
public:
	MsgHallFilter();
	virtual ~MsgHallFilter();
	virtual bool init() { return true; }
	CREATE_FUNC(MsgHallFilter);

	virtual void onDataReceiver(int msgType,cocos2d::Ref* objectData ){};
	virtual std::vector<int> onFilters();
	virtual std::vector<int> onRootFilters();
	virtual bool onMessageReceived(int msgType, WWBuffer *oBuffer);
	virtual bool onRootMessageReceived(int msgType, int userId, int result,std::string reason, int reasonType);

public:
	void parseLogin(WWBuffer *oBuffer);
	void parseLoginGame(WWBuffer *oBuffer);
	Ref* parseHallEnterRoom(WWBuffer *oBuffer);
	void parseSetInfo(WWBuffer *oBuffer);
	void parseGetInfo(WWBuffer *oBuffer);
	void parseAskRoom(WWBuffer *oBuffer);
	void parseGetRank(WWBuffer *oBuffer);
	void parseGetRecord(WWBuffer *oBuffer);
	void parseNotice(WWBuffer *oBuffer);
    void parsePayResult(WWBuffer *oBuffer);
    void parseBindCode(WWBuffer *oBuffer);
	void parseCanShare(WWBuffer *oBuffer);
	void parseDayShare(WWBuffer *oBuffer);
	void parseGetOnline(WWBuffer *oBuffer);
	void parseGetTake(WWBuffer *oBuffer);

	void parseFbkSubmit(WWBuffer *oBuffer);
};

#endif /* MSGHALLDATAFILTER_H_ */
