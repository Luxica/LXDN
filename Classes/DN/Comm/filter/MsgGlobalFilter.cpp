
#include "MsgGlobalFilter.h"

const int filters[] ={ MSG_PORP_ISSUE_NOTIFY};
static const std::vector< int > vrootfilters = {
	99999,
};
USING_NS_CC;

MsgGlobalFilter::MsgGlobalFilter()
{
    //Director::getInstance()->getEventDispatcher()->addCustomEventListener("requestMatchMsgFilter", CC_CALLBACK_1(MsgGlobalFilter::handleMatchMsgFilter, this));
}

MsgGlobalFilter::~MsgGlobalFilter()
{

}

bool MsgGlobalFilter::init()
{
	return true;
}

const std::vector<int> vfilters(filters, filters + sizeof(filters) / sizeof(int));

std::vector<int> MsgGlobalFilter::onFilters()
{
	return vfilters;
}

std::vector<int> MsgGlobalFilter::onRootFilters()
{
	return vrootfilters;
}

bool MsgGlobalFilter::onMessageReceived(int msgType, WWBuffer * oBuffer)
{
	log("MsgGlobalFilter::onMessageReceived %d", msgType);
	switch (msgType)
	{
        case MSG_PORP_ISSUE_NOTIFY:
            parsePropIssue(oBuffer);
            return true;
            break;
        default:
            return false;
	}

	return false;
}

bool MsgGlobalFilter::onRootMessageReceived(int msgType, int userId, int result, std::string reason, int reasonType)
{
	return true;
}

Ref* MsgGlobalFilter::parsePropIssue(WWBuffer *oBuffer)
{
	log("parsePropIssue111111");
	return nullptr;
}

