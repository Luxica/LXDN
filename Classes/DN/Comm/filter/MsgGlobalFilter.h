
#ifndef MSGGLOBALFILTER_H_
#define MSGGLOBALFILTER_H_

#include "cocos2d.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#include "ww-event.h"
#include "ww-net.h"
#else
#include "EventSystem/ww-event.h"
#include "NetSystem/ww-net.h"
#endif

const int MSG_PORP_ISSUE_NOTIFY = 0x010137;

class MsgGlobalFilter: public WWIMsgFilter, public cocos2d::Ref
{
public:
	MsgGlobalFilter();

	virtual ~MsgGlobalFilter();

	CREATE_FUNC(MsgGlobalFilter);

	bool init();

	virtual void onDataReceiver(int msgType,cocos2d::Ref* objectData ){};

	virtual std::vector<int> onFilters();
	virtual std::vector<int> onRootFilters();
	virtual bool onMessageReceived(int msgType, WWBuffer *oBuffer);

	virtual bool onRootMessageReceived(int msgType, int userId, int result,std::string reason, int reasonType);

	cocos2d::Ref* parsePropIssue(WWBuffer *oBuffer);
};

#endif /* MSGGLOBALFILTER_H_ */
