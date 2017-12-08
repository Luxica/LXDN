
#ifndef MSGLOGONREQUEST_H_
#define MSGLOGONREQUEST_H_
#include "cocos2d.h"

class WWBuffer;

class MsgLogonRequest
{
public:
	MsgLogonRequest();
	virtual ~MsgLogonRequest();

	static void sendGetType();

	static void sendGetWay();//銮峰彇ip port
};

#endif /* MSGLOGONREQUEST_H_ */
