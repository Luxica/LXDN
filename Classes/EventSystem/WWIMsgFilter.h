#ifndef WWIMSGCALLBACK_H_
#define WWIMSGCALLBACK_H_
#include "cocos2d.h"
#include "../NetSystem/WWBuffer.h"

/**
 *消息过滤器的基类
 *用于MsgManager中注册获取分发消息
 */
class WWIMsgFilter{
public :
	WWIMsgFilter();
	virtual ~WWIMsgFilter();
	//响应消息过滤器
	virtual std::vector<int>  onFilters()=0;
	//响应消息回调
	virtual bool onMessageReceived( int msgType,WWBuffer *oBuffer)=0;
	//通用消息过滤器
	virtual std::vector<int>  onRootFilters()=0;
	//通用消息回调
	virtual bool onRootMessageReceived( int msgType,int userId,int result,std::string reason,int reasonType)=0;
	virtual void onDataReceiver(int msgType,cocos2d::Ref* objectData )=0;//过滤器中所有的消息通过此方法分发到场景中，根据type转型为相应的数据结构
};



#endif /* WWIMSGCALLBAKCK_H_ */
