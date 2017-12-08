
#include "WWMsgManager.h"
#include <algorithm>
#include "../net/LordNetManger.h"

using namespace cocos2d;

WWMsgManager::WWMsgManager():
m_filters(0)
{
    
}

WWMsgManager::~WWMsgManager()
{
    if(!m_filters.empty())
    {
        std::vector<WWIMsgFilter*>::iterator citr = m_filters.begin();
        std::vector<WWIMsgFilter*>::iterator cend = m_filters.end();
        for(; citr != cend; ++citr )
        {
            Ref* obj=dynamic_cast<Ref*>(*citr);
            
            CC_SAFE_RELEASE(obj);
        }
    }
    m_filters.clear();
}
void WWMsgManager::onMessage(WWBuffer *buffer)
{
    
    //CCLOG("WWMsgMananger::onMessage ");
    do
    {
        CC_BREAK_IF(!buffer);
        
        buffer->resetReaderIndex();
        
        //pass msg to lua first
        buffer->markReaderIndex();
        bool isDeal = false;
        int luamsgType = buffer->readInt();
        buffer->resetReaderIndex();
        
        if (luamsgType == 0)
        {
            buffer->markReaderIndex();
            buffer->readInt();
			int type = buffer->readInt();
            buffer->resetReaderIndex();
            
            buffer->markReaderIndex();
			buffer->readInt();
            WWBuffer* sbuffer = buffer->readData(buffer->getReadableSize());
            buffer->resetReaderIndex();
        }
        else
        {
			buffer->markReaderIndex();
			auto sbuffer = buffer->readData(buffer->getReadableSize());
			buffer->resetReaderIndex();
        }
        
        //if lua do not deal, pass to c++
        int msgType = buffer->readInt();
        WWBuffer * sbuffer = buffer->readData(buffer->getReadableSize());
        
        switch (msgType)
        {
                //通用消息
            case 0:
                onRootMessageReceived(msgType, sbuffer);
                break;
            default:
				log("WWMsgMananger::onMessage msgType=%d", msgType);
                onMessageReceived(msgType, sbuffer);
                break;
        }
        
    } while (0);
    
}
void WWMsgManager::onMessageReceived(const int msgType, WWBuffer *buffer)
{
    //循环检测发送
    bool deal = false;
    
    std::vector<WWIMsgFilter*>::iterator itfilter;
    //遍历filters 通过onFilters()获取 msgTypes 判读是否存在，存在则通过onMessageReceived发送
    for (itfilter = m_filters.begin(); itfilter != m_filters.end();
         ++itfilter)
    {
        std::vector<int> ifilters = (*itfilter)->onFilters();
        std::vector<int>::iterator it_msgType;
        it_msgType = std::find(ifilters.begin(), ifilters.end(), msgType);
        
        if (it_msgType != ifilters.end())
        {
            deal = (*itfilter)->onMessageReceived(msgType, buffer->copy());
            if (deal)
                return;
        }
    } 
}

void WWMsgManager::onRootMessageReceived(const int msgType, WWBuffer *buffer)
{
    //bool deal = false;
    //
    //int type = buffer->readChar3ToInt();
    //int userId = buffer->readInt();
    //int result = buffer->readChar();
    //std::string reason = buffer->readLengthAndString();
    //int reasonType = buffer->readShort();
    //
    //// 循环检测发送
    ////遍历filters 通过onRootFilters()获取 msgTypes 判读是否存在，存在则通过onRootMessageReceived发送
    //
    //std::vector<WWIMsgFilter*>::iterator itfilter;
    //for (itfilter = m_filters.begin(); itfilter != m_filters.end();
    //     ++itfilter)
    //{
    //    std::vector<int> rootfilters = (*itfilter)->onRootFilters();
    //    std::vector<int>::iterator it_msgType;
    //    
    //    
    //    it_msgType = std::find(rootfilters.begin(), rootfilters.end(), type);
    //    
    //    if (it_msgType != rootfilters.end())
    //    {
    //        deal = (*itfilter)->onRootMessageReceived(type, userId,
    //                                                  result, reason, reasonType);
    //        if (deal){
    //            return;
    //        }
    //    }
    //}
}

void WWMsgManager::sendMessage(const int &iType,WWBuffer *buffer)
{
	LordNetManger::getInstance()->send(iType,buffer);
}

void WWMsgManager::registerMsgFilter(WWIMsgFilter *filter)
{
	CCAssert(filter && dynamic_cast<Ref*>(filter), "should not null");
    do
    {
        //判断是否为空，是否继承WWObject
        CC_BREAK_IF(!filter && !dynamic_cast<Ref*>(filter));
        //遍历判断是否存在，存在则不加入
        std::vector<WWIMsgFilter*>::iterator itr;
        
        itr = std::find(m_filters.begin(), m_filters.end(), filter);
        
        if (itr != m_filters.end())
        {
            return;
        }
        
        Ref* pObj = dynamic_cast<Ref*>(filter);
        CC_SAFE_RETAIN(pObj);
        
        m_filters.insert(m_filters.begin(), filter);
        
    } while (0);
    
}

void WWMsgManager::unregisterMsgFilter(WWIMsgFilter *filter)
{
    CCAssert(filter && dynamic_cast<Ref*>(filter), "should not null");
    do
    {
        //判断是否为空，是否继承WWObject
        CC_BREAK_IF(!filter && !dynamic_cast<Ref*>(filter));
        
        std::vector<WWIMsgFilter*>::iterator itr;
        itr = std::find(m_filters.begin(), m_filters.end(), filter);
        
        if (itr != m_filters.end())
        {
            
            m_filters.erase(itr);
        }
        
        Ref* pObj = dynamic_cast<Ref*>(filter);
        CC_SAFE_RELEASE(pObj);
        
    } while (0);
    
}

void WWMsgManager::handleUnCatchMsg(std::vector<int> vecMsgType, WWIMsgFilter *pMsgFilter)
{
    //WWIMsgUnCatchFilter::getInstance()->handleUnCatchMsg(vecMsgType, pMsgFilter);
    
}

