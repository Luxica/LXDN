
#ifndef _MSG_GAMEFILTER_H_
#define _MSG_GAMEFILTER_H_
#include "cocos2d.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#include "ww-event.h"
#include "ww-net.h"
#else
#include "EventSystem/ww-event.h"
#include "NetSystem/ww-net.h"
#endif

enum RC_CMD_MSG                                         // 房间服务器->客户端
{
	RC_CMD_ROOM_JOIN = 21000,                           // 通知进入房间(表示自己进来了)(开房,加房,复房都回应这消息)
	RC_CMD_USER_JOIN,                                   // 通知用户进房(表示别人进来了)
	RC_CMD_USER_LOST,                                   // 通知用户掉房(表示有人掉线了)(offline)(标记掉线了)
	RC_CMD_USER_BACK,                                   // 通知用户回房(表示有人回来了)(reline)(标记复线了)
	RC_CMD_GAME_BACK,                                   // 恢复游戏现场(这个是发给掉线后回来的那个用户，让他恢复游戏)
	RC_CMD_USER_EXIT,                                   // 通知用户退出(表示有用户退出去了)(在第一局游戏还没开始前非房主是可以退出自己的)
	RC_CMD_USER_HAND,                                   // 用户准备就绪
	RC_CMD_SURE_BANKER,                                 // 广播本局庄家(轮庄,固庄模式下游戏开始后就发送，抢庄则在随机庄家状态时发送)
	RC_CMD_ROOM_EXIT,                                   // 用户发起退房
	RC_CMD_VOTE_EXIT,                                   // 用户退房表决
	RC_CMD_ROOM_OVER,                                   // 房间是否解散([收到解散]半数人投票同意或者游戏局数到了,[收到不解散]半数人投票不同意解散)
	RC_CMD_GAME_STATE,                                  // 通知游戏状态(GAME_STATE状态机切换)
	RC_CMD_SEND_POKER,                                  // 发五张牌过去
	RC_CMD_VOTE_BANKER,                                 // 用户抢庄表决
	RC_CMD_USER_BET,                                    // 用户下注结果
	RC_CMD_OPEN_POKER,                                  // 用户开牌结果
	RC_CMD_GAME_RET,                                    // 通知游戏小结[结算](每局游戏完了都会小结算一次)
	RC_CMD_GAME_SUM,                                    // 通知解散大结[总结](退房成功或者游戏总局数到了就会总结)(不能以大结作为退房的标准)
	RC_CMD_CARD_ADD,                                    // 通知增加房卡(负数为减少)
	RC_CMD_CHAT_FAST,                                   // 用户快捷聊天
	RC_CMD_CHAT_VOICE,                                  // 用户语音聊天
	RC_CMD_TAKE_ON,                                     // 响应请求代开(代开者)
	RC_CMD_TAKE_OFF,                                    // 响应解散代开(代开者)
	RC_CMD_ROOM_KICK,                                   // 响应踢人结果(代开者or房主)
	RC_CMD_ROOM_WARN,                                   // 房主通知警告(房间里所有人)
};


class MsgGameFilter:public WWIMsgFilter,public Ref
{
public:
	MsgGameFilter();
	virtual ~MsgGameFilter();
	virtual bool init() { return true; }
	CREATE_FUNC(MsgGameFilter);
	virtual void onDataReceiver(int msgType,cocos2d::Ref* objectData ){};
	virtual std::vector<int> onFilters();
	virtual std::vector<int> onRootFilters();
	virtual bool onMessageReceived(int msgType, WWBuffer *oBuffer);
	virtual bool onRootMessageReceived(int msgType, int userId, int result,std::string reason, int reasonType);

private:
	void parseEnterRoom(WWBuffer *oBuffer);
	void parseJoinRoom(WWBuffer *oBuffer);
	void parseUserLost(WWBuffer *oBuffer);		//通知用户掉房
	void parseUserBack(WWBuffer *oBuffer);		//通知用户回房
	void parseGameBack(WWBuffer *oBuffer);		// 恢复游戏现场(这个是发给掉线后回来的那个用户，让他恢复游戏)
	void parseUserExit(WWBuffer *oBuffer);		//椅子号[如果椅子号等于自己，表示自己退出，收到后客户端应主动关闭房间套接字]
	void parseReady(WWBuffer *oBuffer);
	void parseRoomCancel(WWBuffer *oBuffer);
	void parseVoteCancel(WWBuffer *oBuffer);
	void parseRoomFate(WWBuffer *oBuffer);		// 退房最终结果(退房成功or失败)(退房成功之后就是解散房间)
	void parseRoomOver(WWBuffer *oBuffer);
	void parseGameStaues(WWBuffer *oBuffer);
	void parseSendPoker(WWBuffer *oBuffer);
	void parseQiangZ(WWBuffer *oBuffer);
	void parseZend(WWBuffer *oBuffer);
	void parseBet(WWBuffer *oBuffer);
	void parseOpenCard(WWBuffer *oBuffer);
	void parseGameResult(WWBuffer *oBuffer);
	void parseCostRoomCard(WWBuffer *oBuffer);

	void parseChatFast(WWBuffer *oBuffer);
    void parseChatVoice(WWBuffer *oBuffer);

	void parseTakeOn(WWBuffer *oBuffer);
	void parseTakeOff(WWBuffer *oBuffer);

	void parseRoomKick(WWBuffer *oBuffer);
	void parseRoomWarn(WWBuffer *oBuffer);

};

#endif /* MSGHALLDATAFILTER_H_ */
