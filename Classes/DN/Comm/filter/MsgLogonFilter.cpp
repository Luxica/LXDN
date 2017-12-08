#include "MsgLogonFilter.h"

USING_NS_CC;

static const std::vector< int > vfilters = {
	MSG_LOGON_GETWAY,
};

static const std::vector< int > vrootfilters = {
	99999,
};


MsgLogonFilter::MsgLogonFilter()
{

}

MsgLogonFilter::~MsgLogonFilter()
{

}

std::vector<int> MsgLogonFilter::onFilters()
{
	return vfilters;
}

std::vector<int> MsgLogonFilter::onRootFilters()
{
	return vrootfilters;
}

bool MsgLogonFilter::onRootMessageReceived(int msgType, int userId, int result, std::string reason, int reasonType)
{
	return true;
}

bool MsgLogonFilter::onMessageReceived(int msgType, WWBuffer * oBuffer)
{
	//log("MsgLogonFilter::onMessageReceived %d", msgType);
	Ref* dataObj = nullptr;
	switch(msgType){
	case MSG_LOGON_GETWAY:
		dataObj = oBuffer;
		break;
	default:
		return false;
	}
	onDataReceiver(msgType, dataObj);
	return true;
}