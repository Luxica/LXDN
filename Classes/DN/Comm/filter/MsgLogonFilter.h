
#ifndef MSGLOGONDATAFILTER_H_
#define MSGLOGONDATAFILTER_H_
#include "cocos2d.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#include "ww-event.h"
#include "ww-net.h"
#else
#include "EventSystem/ww-event.h"
#include "NetSystem/ww-net.h"
#endif


const int MSG_LOGON_TYPE = 19;
const int MSG_LOGON_GETWAY = 20; 

class MsgLogonFilter: public WWIMsgFilter
{
public:
	MsgLogonFilter();
	virtual ~MsgLogonFilter();

	virtual void onDataReceiver(int msgType,cocos2d::Ref* objectData ){};
	virtual std::vector<int> onFilters();
	virtual std::vector<int> onRootFilters();
	virtual bool onMessageReceived(int msgType, WWBuffer *oBuffer);
	virtual bool onRootMessageReceived(int msgType, int userId, int result,std::string reason, int reasonType);
};

#endif /* MSGLOGONDATAFILTER_H_ */
