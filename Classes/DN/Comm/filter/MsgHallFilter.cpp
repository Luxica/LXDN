#include "MsgHallFilter.h"
#include "../../Scene/HallScene/data/QHallManager.h"
#include "../../Scene/GameScene/data/QGameManager.h"

USING_NS_CC;

const int filters[] = { LC_CMD_USR_LOGIN, LC_CMD_LOG_UID, LC_CMD_GET_ROOM, LC_CMD_SET_INFO, LC_CMD_GET_INFO, LC_CMD_ASK_ROOM
, LC_CMD_GET_RANK, LC_CMD_GET_RECD, LC_CMD_GET_TIMES, LC_CMD_GET_PLAY, LC_CMD_GET_NOTICE, LC_CMD_MSG_PAYS, LC_CMD_BND_CODE, LC_CMD_CAN_SHARE
, LC_CMD_DAY_SHARE, LC_CMD_GET_ONLINE, LC_CMD_GET_TAKE, LC_CMD_FND_ROOM, LC_CMD_FBK_SUBMIT };
static const std::vector< int > vrootfilters = {
	99999,
};

MsgHallFilter::MsgHallFilter()
{
}

MsgHallFilter::~MsgHallFilter()
{
}

const std::vector<int> vfilters(filters, filters + sizeof(filters) / sizeof(int));

std::vector<int> MsgHallFilter::onFilters()
{
	return vfilters;
}

std::vector<int> MsgHallFilter::onRootFilters()
{
	return vrootfilters;
}



bool MsgHallFilter::onMessageReceived(int msgType, WWBuffer * oBuffer)
{
	log("MsgHallFilter::onMessageReceived %d", msgType);
	Ref* dataObj = nullptr;
	switch (msgType)
	{
	case LC_CMD_USR_LOGIN:
		parseLogin(oBuffer);
        break;
	case LC_CMD_LOG_UID:
		parseLoginGame(oBuffer);
		break;
	case LC_CMD_GET_ROOM:
		dataObj = parseHallEnterRoom(oBuffer);
		break;
	case LC_CMD_SET_INFO:
		parseSetInfo(oBuffer);
		break;
	case LC_CMD_GET_INFO:
		parseGetInfo(oBuffer);
		break;
	case LC_CMD_ASK_ROOM:
		parseAskRoom(oBuffer);
		break;
	case LC_CMD_GET_RANK:
		parseGetRank(oBuffer);
		break;
	case LC_CMD_GET_RECD:
		parseGetRecord(oBuffer);
		break;
	case LC_CMD_GET_TIMES:
		break;
	case LC_CMD_GET_PLAY:
		break;
	case LC_CMD_GET_NOTICE:
		parseNotice(oBuffer);
		break;
    case LC_CMD_MSG_PAYS:
        parsePayResult(oBuffer);
        break;
    case LC_CMD_BND_CODE:
        parseBindCode(oBuffer);
        break;
	case LC_CMD_CAN_SHARE:
		parseCanShare(oBuffer);
		break;
	case LC_CMD_DAY_SHARE:
		parseDayShare(oBuffer);
		break;
	case LC_CMD_GET_ONLINE:
		parseGetOnline(oBuffer);
		break;
	case LC_CMD_GET_TAKE:
		parseGetTake(oBuffer);
		break;
	case LC_CMD_FND_ROOM:

		break;
	case LC_CMD_FBK_SUBMIT:
		parseFbkSubmit(oBuffer);
		break;
    default:
        return false;
	}
	onDataReceiver(msgType, dataObj);
	return true;
    
}

bool MsgHallFilter::onRootMessageReceived(int msgType, int userId, int result, std::string reason, int reasonType)
{
	return true;
}

void MsgHallFilter::parseLogin(WWBuffer *oBuffer)
{
	QHallManager::getInstance()->dealLogin(oBuffer);
}

void MsgHallFilter::parseLoginGame(WWBuffer *oBuffer)
{
	QGameManager::getInstance()->dealLoginGame(oBuffer);
}

Ref* MsgHallFilter::parseHallEnterRoom(WWBuffer *oBuffer)
{
	QHallManager::getInstance()->dealHallEnterRoom(oBuffer);
	return nullptr;
}

void MsgHallFilter::parseSetInfo(WWBuffer *oBuffer)
{
	QHallManager::getInstance()->dealSetInfo(oBuffer);
}

void MsgHallFilter::parseGetInfo(WWBuffer *oBuffer)
{
	QHallManager::getInstance()->dealGetInfo(oBuffer);
}

void MsgHallFilter::parseAskRoom(WWBuffer *oBuffer)
{
	QHallManager::getInstance()->dealAskRoom(oBuffer);
}

void MsgHallFilter::parseGetRank(WWBuffer *oBuffer)
{
	QHallManager::getInstance()->dealGetRank(oBuffer);
}

void MsgHallFilter::parseGetRecord(WWBuffer *oBuffer)
{
	QHallManager::getInstance()->dealGetRecord(oBuffer);
}

void MsgHallFilter::parseNotice(WWBuffer *oBuffer)
{
	QHallManager::getInstance()->dealNotice(oBuffer);
}

void MsgHallFilter::parsePayResult(WWBuffer *oBuffer)
{
    QHallManager::getInstance()->dealPayResult(oBuffer);
}

void MsgHallFilter::parseBindCode(WWBuffer *oBuffer)
{
    QHallManager::getInstance()->dealBindCode(oBuffer);
}
void MsgHallFilter::parseCanShare(WWBuffer *oBuffer)
{
	QHallManager::getInstance()->dealCanShare(oBuffer);
}
void MsgHallFilter::parseDayShare(WWBuffer *oBuffer)
{
	QHallManager::getInstance()->dealDayShare(oBuffer);
}

void MsgHallFilter::parseGetOnline(WWBuffer *oBuffer)
{
	QHallManager::getInstance()->dealGetOnline(oBuffer);
}

void MsgHallFilter::parseGetTake(WWBuffer *oBuffer)
{
	QHallManager::getInstance()->dealGetTake(oBuffer);
}

void MsgHallFilter::parseFbkSubmit(WWBuffer *oBuffer)
{
	QHallManager::getInstance()->dealFbkSubmit(oBuffer);
}

