#include "MsgLogonRequest.h"
#include "../../../NetSystem/ww-net.h"
#include "../../../EventSystem/ww-event.h"

USING_NS_CC;
MsgLogonRequest::MsgLogonRequest()
{

}

MsgLogonRequest::~MsgLogonRequest()
{

}

void MsgLogonRequest::sendGetType() 
{
	log("MsgLogonRequest::sendGetType");
	WWBuffer wBuffer;
	wBuffer.writeInt(9);
	WWMsgManager::getInstance()->sendMessage(WWNetwork::eLoginNet, &wBuffer);

}

void MsgLogonRequest::sendGetWay()
{
	log("MsgLogonRequest::sendGetWay");
	sendGetType();

	WWBuffer wBuffer;
	wBuffer.writeInt(10);
	WWMsgManager::getInstance()->sendMessage(WWNetwork::eLoginNet, &wBuffer);
}
