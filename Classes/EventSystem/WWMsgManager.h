

#ifndef WWMSGMANAGER_H_
#define WWMSGMANAGER_H_

#include "cocos2d.h"
#include "../NetSystem/WWBuffer.h"
#include "../WWMacros.h"
#include "WWIMsgFilter.h"
/**
 * 消息分发管理类（主要用于网络消息分发：目前已经集成到NetManager中）
 */

class WWMsgManager :public cocos2d::Ref
{
public:
    virtual ~WWMsgManager();
    SINGLE_INSTANCE_FUNC(WWMsgManager);
    
    //发送网络消息
    void sendMessage(const int &iType,WWBuffer *buffer);
    //接收网络消息
    void onMessage(WWBuffer *buffer);
    
    //注册消息过滤器
    void registerMsgFilter(WWIMsgFilter *filter);
    //反注册消息过滤器
    void unregisterMsgFilter(WWIMsgFilter *filter);
    
    //处理未捕获的消息
    void handleUnCatchMsg(std::vector<int> vecMsgType, WWIMsgFilter *pMsgFilter);
private:
    WWMsgManager();
    
    void onMessageReceived(const int msgType,WWBuffer *buffer);
    void onRootMessageReceived(const int msgType,WWBuffer *buffer);

private:
    std::vector<WWIMsgFilter* > m_filters;


};

#endif /* WWMSGMANAGER_H_ */
