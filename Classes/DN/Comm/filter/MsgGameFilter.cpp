#include "MsgGameFilter.h"
#include "../../Scene/GameScene/data/QGameManager.h"

USING_NS_CC;


const int filters[] = { RC_CMD_ROOM_JOIN, RC_CMD_USER_JOIN, RC_CMD_USER_LOST, 
RC_CMD_USER_BACK, RC_CMD_USER_EXIT, RC_CMD_GAME_BACK, RC_CMD_USER_HAND, RC_CMD_SURE_BANKER,RC_CMD_ROOM_EXIT, 
RC_CMD_VOTE_EXIT, RC_CMD_ROOM_OVER, RC_CMD_GAME_STATE, RC_CMD_SEND_POKER, RC_CMD_VOTE_BANKER, RC_CMD_USER_BET, 
RC_CMD_OPEN_POKER, RC_CMD_GAME_RET, RC_CMD_GAME_SUM, RC_CMD_CARD_ADD, RC_CMD_CHAT_FAST, RC_CMD_CHAT_VOICE,
RC_CMD_TAKE_ON, RC_CMD_TAKE_OFF, RC_CMD_ROOM_KICK, RC_CMD_ROOM_WARN };
static const std::vector< int > vrootfilters = {
	99999,
};

MsgGameFilter::MsgGameFilter()
{
}

MsgGameFilter::~MsgGameFilter()
{
}

const std::vector<int> vfilters(filters, filters + sizeof(filters) / sizeof(int));

std::vector<int> MsgGameFilter::onFilters()
{
	return vfilters;
}

std::vector<int> MsgGameFilter::onRootFilters()
{
	return vrootfilters;
}

bool MsgGameFilter::onMessageReceived(int msgType, WWBuffer * oBuffer)
{
	log("MsgGameFilter::onMessageReceived %d", msgType);
	Ref* dataObj = nullptr;
	switch (msgType)
	{
		case RC_CMD_ROOM_JOIN:
		{
			parseEnterRoom(oBuffer);
			break;
		}
		case RC_CMD_USER_JOIN:
		{
			parseJoinRoom(oBuffer);
			break;
		}
		case RC_CMD_USER_HAND:
		{	
			parseReady(oBuffer);
			break;
		}
		case RC_CMD_USER_LOST:
		{
			parseUserLost(oBuffer);	//通知用户掉房
			break;
		}
		case RC_CMD_CHAT_FAST:
		{
			parseChatFast(oBuffer);
			break;
		}
            
        case RC_CMD_CHAT_VOICE:
        {
            parseChatVoice(oBuffer);
            break;
        }
        case RC_CMD_USER_BACK:
		{
			parseUserBack(oBuffer);  //通知用户回房
			break;
		}
		case RC_CMD_GAME_BACK:
		{
			parseGameBack(oBuffer);	// 恢复游戏现场(这个是发给掉线后回来的那个用户，让他恢复游戏)
			break;
		}
		case RC_CMD_USER_EXIT:		// 这个是用户退出去了(游戏没开始，又不是房主，用户自己可以退出去)
		{
			parseUserExit(oBuffer);
			break;
		}
		case RC_CMD_ROOM_EXIT:
		{
			parseRoomCancel(oBuffer);
			break;
		}
		case RC_CMD_VOTE_EXIT:
		{
			parseVoteCancel(oBuffer);
			break;
		}
		case RC_CMD_GAME_SUM:
		{
			parseRoomOver(oBuffer);
			break;
		}
		case RC_CMD_ROOM_OVER:
		{
			parseRoomFate(oBuffer);
			break;
		}
		case RC_CMD_GAME_STATE:
		{
			parseGameStaues(oBuffer);
			break;
		}
		case RC_CMD_SEND_POKER:
		{
			parseSendPoker(oBuffer);
			break;
		}
		case RC_CMD_VOTE_BANKER:
		{
			parseQiangZ(oBuffer);
			break;
		}
		case RC_CMD_SURE_BANKER:
		{
			parseZend(oBuffer);
			break;
		}
		case RC_CMD_USER_BET:
		{
			parseBet(oBuffer);
			break;
		}
		case RC_CMD_OPEN_POKER:
		{
			parseOpenCard(oBuffer);
			break;
		}
		case RC_CMD_GAME_RET:
		{
			parseGameResult(oBuffer);
			break;
		}
		case RC_CMD_CARD_ADD:
		{
			parseCostRoomCard(oBuffer);
			break;
		}
		case RC_CMD_TAKE_ON:
		{
			parseTakeOn(oBuffer);
			break;
		}
		case RC_CMD_TAKE_OFF:
		{
			parseTakeOff(oBuffer);
			break;
		}
		case RC_CMD_ROOM_KICK:
		{
			parseRoomKick(oBuffer);
			break;
		}
		case RC_CMD_ROOM_WARN:
		{
			parseRoomWarn(oBuffer);
			break;
		}
        default:
            return false;
	}
	onDataReceiver(msgType, dataObj);
	return true;
    
}

bool MsgGameFilter::onRootMessageReceived(int msgType, int userId, int result, std::string reason, int reasonType)
{
	return true;
}

void MsgGameFilter::parseEnterRoom(WWBuffer *oBuffer)
{
	QGameManager::getInstance()->dealEnterRoom(oBuffer);
}

void MsgGameFilter::parseJoinRoom(WWBuffer *oBuffer)
{
	QGameManager::getInstance()->dealJoinRoom(oBuffer);
}

void MsgGameFilter::parseUserLost(WWBuffer *oBuffer)	//通知用户掉房
{
	QGameManager::getInstance()->dealUserLost(oBuffer);
}

void MsgGameFilter::parseUserBack(WWBuffer *oBuffer)  //通知用户回房
{
	QGameManager::getInstance()->dealUserBack(oBuffer);
}

void MsgGameFilter::parseGameBack(WWBuffer *oBuffer)	// 恢复游戏现场(这个是发给掉线后回来的那个用户，让他恢复游戏)
{
	QGameManager::getInstance()->dealGameBack(oBuffer);
}

void MsgGameFilter::parseUserExit(WWBuffer *oBuffer)
{
	QGameManager::getInstance()->dealUserExit(oBuffer);
}

void MsgGameFilter::parseReady(WWBuffer *oBuffer)
{
	QGameManager::getInstance()->dealReady(oBuffer);
}

void MsgGameFilter::parseRoomCancel(WWBuffer *oBuffer)
{
	QGameManager::getInstance()->dealRoomCancel(oBuffer);
}

void MsgGameFilter::parseVoteCancel(WWBuffer *oBuffer)
{
	QGameManager::getInstance()->dealVoteCancel(oBuffer);
}

void MsgGameFilter::parseRoomFate(WWBuffer *oBuffer)
{
	QGameManager::getInstance()->dealRoomFate(oBuffer);
}

void MsgGameFilter::parseRoomOver(WWBuffer *oBuffer)
{
	QGameManager::getInstance()->dealRoomOver(oBuffer);
}

void MsgGameFilter::parseGameStaues(WWBuffer *oBuffer)
{
	QGameManager::getInstance()->dealGameStatues(oBuffer);
}

void MsgGameFilter::parseSendPoker(WWBuffer *oBuffer)
{
	QGameManager::getInstance()->dealPoker(oBuffer);
}

void MsgGameFilter::parseQiangZ(WWBuffer *oBuffer)
{
	QGameManager::getInstance()->dealQZhuang(oBuffer);
}

void MsgGameFilter::parseZend(WWBuffer *oBuffer)
{
	QGameManager::getInstance()->dealZhuangEnd(oBuffer);
}

void MsgGameFilter::parseBet(WWBuffer *oBuffer)
{
	QGameManager::getInstance()->dealBet(oBuffer);
}

void MsgGameFilter::parseOpenCard(WWBuffer *oBuffer)
{
	QGameManager::getInstance()->dealOpenCard(oBuffer);
}

void MsgGameFilter::parseGameResult(WWBuffer *oBuffer)
{
	QGameManager::getInstance()->dealGameResult(oBuffer);
}

void MsgGameFilter::parseCostRoomCard(WWBuffer *oBuffer)
{
	QGameManager::getInstance()->dealCostRoomCardNum(oBuffer);
}

void MsgGameFilter::parseChatFast(WWBuffer *oBuffer)
{
	QGameManager::getInstance()->dealChatFast(oBuffer);
}

void MsgGameFilter::parseChatVoice(WWBuffer *oBuffer)
{
    QGameManager::getInstance()->dealChatVoice(oBuffer);
}

void MsgGameFilter::parseTakeOn(WWBuffer *oBuffer)
{
	QGameManager::getInstance()->dealTakeOn(oBuffer);
}

void MsgGameFilter::parseTakeOff(WWBuffer *oBuffer)
{
	QGameManager::getInstance()->dealTakeOff(oBuffer);
}

void MsgGameFilter::parseRoomKick(WWBuffer *oBuffer)
{
	QGameManager::getInstance()->dealRoomKick(oBuffer);
}

void MsgGameFilter::parseRoomWarn(WWBuffer *oBuffer)
{
	QGameManager::getInstance()->dealRoomWarn(oBuffer);
}

